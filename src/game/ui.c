/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal ui_node * UI_GetNode(ui *UI, u16 Index) { return (ui_node*)UI->Nodes->Data + Index; }

internal ui_node *
_UI_CreateNode(ui *UI)
{
    UI->NodeCount++;
    if(UI->Nodes->Size < UI->NodeCount*sizeof(ui_node))
        Heap_Resize(UI->Nodes, UI->NodeCount*sizeof(ui_node)*2);
    
    ui_node *NewNode = UI_GetNode(UI, UI->NodeCount-1);
    Mem_Set(NewNode, 0, sizeof(ui_node));
    NewNode->Index = UI->NodeCount-1;
    
    return NewNode;
}

internal void
UI_Init(ui *UI,
        mesh *Mesh,
        heap *Heap)
{
    UI->Mesh = Mesh;
    
    UI->NodeCount = 1;
    
    UI->Nodes = Heap_Allocate(Heap, UI->NodeCount*sizeof(ui_node));
    
    Mem_Set(UI->Nodes->Data, 0, sizeof(ui_node));
}

internal void
UI_AddChild(ui *UI,
           u16 ParentIndex,
           u16 ChildIndex)
{
    Assert(UI);
    
    ui_node *ParentNode = UI_GetNode(UI, ParentIndex);
    ui_node *ChildNode = UI_GetNode(UI, ChildIndex);
    
    ChildNode->Parent = ParentNode->Index;
    ChildNode->NextSibling = ParentNode->FirstChild;
    ParentNode->FirstChild = ChildNode->Index;
    ParentNode->ChildCount++;
}

internal void
UI_AddSibling(ui *UI,
              u16 PrevSiblingIndex,
              u16 SiblingIndex)
{
    Assert(UI);
    
    ui_node *PrevSiblingNode = UI_GetNode(UI, PrevSiblingIndex);
    ui_node *SiblingNode = UI_GetNode(UI, SiblingIndex);
    ui_node *ParentNode = UI_GetNode(UI, SiblingNode->Parent);
    
    SiblingNode->NextSibling = PrevSiblingNode->NextSibling;
    PrevSiblingNode->NextSibling = SiblingNode->Index;
}

internal u16
UI_CreateStringNode(ui *UI,
                    string String,
                    v2r32 Pos,
                    v2r32 Size,
                    r32 FontHeight,
                    assetpack Assetpack)
{
    ui_node *Node = _UI_CreateNode(UI);
    heap *Heap = Heap_GetHeap(UI->Nodes);
    
    Node->Type = UI_NODE_TYPE_STRING;
    Node->StringData.String = String;
    
    u32 VertexSize = UI->Mesh->VertexSize;
    
    assetpack_texture *Cache['~'-' '+1] = {0};
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Assetpack, TAG_FONT_DEF);
    Assert(Tag);
    assetpack_font *Font = Tag->ValueP;
    
    u32 LineCount = 0;
    heap_handle *Lines = Heap_Allocate(Heap, LineCount*sizeof(u32));
    Node->StringData.Lines = Lines;
    r32 StringLength = String.Length;
    
    r32 UnitAdvance = Font->Ascent-Font->Descent+Font->LineGap;
    
    v2r32 Scale = {FontHeight, FontHeight};
    v2r32 P = {Pos.X,Pos.Y+Size.Y-Font->Ascent*Scale.Y};
    v2r32 A = {0,UnitAdvance*Scale.Y};
    u32 PrintedChars = 0;
    r32 MaxWidth = 0;
    r32 CurrWidth = 0;
    r32 HighestAscent = 0;
    r32 LowestDescent = 0;
    u32 LastBreakChar = 0;
    r32 WidthSinceBreak = 0;
    r32 AdvanceSinceBreak = 0;
    for(u32 C = 0; C < StringLength; C++) {
        c08 Char = String.Text[C];
        
        if(!Char || Char == '\n' || Char == '\t') continue;
        
        b08 IsBreakChar = Char == ' ' || Char == '-';
        if(IsBreakChar && P.X+A.X < Pos.X+Size.X) {
            LastBreakChar = C+1;
            AdvanceSinceBreak = 0;
            WidthSinceBreak = 0;
        }
        
        if(!Cache[Char-' ']) {
            Tag = _Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)(u64)Char);
            Assert(Tag && Tag->AssetCount);
            Cache[Char-' '] = &(Tag->Assets[0])->Texture;
        }
        
        assetpack_texture *Asset = Cache[Char-' '];
        A.X = Asset->AdvanceX*Scale.X;
        
        if(Asset->Bearing.Y < LowestDescent)
            LowestDescent = Asset->Bearing.Y;
        if(Asset->Bearing.Y+Asset->SizeR.Y > HighestAscent)
            HighestAscent = Asset->Bearing.Y+Asset->SizeR.Y;
        
        if(P.X + A.X > Pos.X+Size.X) {
            P.Y -= A.Y;
            if(P.Y < Pos.Y) {
                StringLength = C;
                break;
            }
            
            LineCount++;
            if(LineCount * sizeof(u32) > Lines->Size)
                Heap_Resize(Lines, LineCount*2*sizeof(u32));
            
            if(AdvanceSinceBreak+A.X <= Size.X) {
                ((u32*)Lines->Data)[LineCount-1] = LastBreakChar;
                if(CurrWidth-WidthSinceBreak > MaxWidth) MaxWidth = CurrWidth-WidthSinceBreak;
                CurrWidth = WidthSinceBreak;
                P.X = Pos.X + AdvanceSinceBreak;
            } else {
                ((u32*)Lines->Data)[LineCount-1] = C;
                if(CurrWidth > MaxWidth) MaxWidth = CurrWidth;
                CurrWidth = 0;
                P.X = Pos.X;
            }
        }
        
        P.X += A.X;
        AdvanceSinceBreak += A.X;
        CurrWidth += Asset->AdvanceX;
        WidthSinceBreak += Asset->AdvanceX;
        
        if(!Asset->Size.X || !Asset->Size.Y) continue;
        
        PrintedChars++;
    }
    if(CurrWidth > MaxWidth) MaxWidth = CurrWidth;
    
    r32 HeightDiff = LineCount*UnitAdvance + HighestAscent-LowestDescent;
    Scale.X = 2/MaxWidth;
    Scale.Y = 2/HeightDiff;
    
    r32 DescentDiff = Font->Descent-LowestDescent;
    
    Node->Object.TranslationMatrix = M4x4r32_Translation(FontHeight/Scale.X+Pos.X, FontHeight/Scale.Y+Pos.Y+Size.Y-FontHeight*HeightDiff, 0);
    Node->Object.ScalingMatrix = M4x4r32_Scaling(FontHeight/Scale.X, FontHeight/Scale.Y, 1);
    Node->Object.RotationMatrix = M4x4r32_Rotation(R32_PI/6, (v3r32){0,0,1});
    
    Node->Object.Indices = Heap_Allocate(Heap, PrintedChars*sizeof(u32)*6);
    Node->Object.Vertices = Heap_Allocate(Heap, PrintedChars*VertexSize*4);
    
    u32 *Indices = (u32*)Node->Object.Indices->Data;
    struct {
        u32 Position;
        u32 Texture;
    } *Vertices = (vptr)Node->Object.Vertices->Data;
    
    A.Y = UnitAdvance*Scale.Y;
    P = (v2r32){-1,1-HighestAscent*Scale.Y};
    u32 LineIndex = 0;
    u32 V = 0;
    for(u32 C = 0; C < StringLength; C++) {
        c08 Char = String.Text[C];
        
        if(!Char || Char == '\n' || Char == '\t') continue;
        
        assetpack_texture *Asset = Cache[Char-' '];
        A.X = Asset->AdvanceX*Scale.X;
        v2r32 B = V2r32_Mul(Asset->Bearing, Scale);
        v2r32 S = V2r32_Mul(Asset->SizeR, Scale);
        
        if(LineCount && C == ((u32*)Lines->Data)[LineIndex]) {
            P.X = -1;
            P.Y -= A.Y;
            LineIndex++;
        }
        
        if(Asset->Size.X && Asset->Size.Y) {
            Indices[6*V+0] = 4*V+0;
            Indices[6*V+1] = 4*V+1;
            Indices[6*V+2] = 4*V+2;
            Indices[6*V+3] = 4*V+0;
            Indices[6*V+4] = 4*V+2;
            Indices[6*V+5] = 4*V+3;
            
            v3r32 Positions[4] = {
                {P.X+B.X,     P.Y+B.Y,     0},
                {P.X+B.X,     P.Y+B.Y+S.Y, 0},
                {P.X+B.X+S.X, P.Y+B.Y+S.Y, 0},
                {P.X+B.X+S.X, P.Y+B.Y,     0},
            };
            
            Vertices[4*V+0].Position = Mesh_EncodePosition(Positions[0]);
            Vertices[4*V+0].Texture = ((Char-32)<<2) | 0b00;
            Vertices[4*V+1].Position = Mesh_EncodePosition(Positions[1]);
            Vertices[4*V+1].Texture = ((Char-32)<<2) | 0b10;
            Vertices[4*V+2].Position = Mesh_EncodePosition(Positions[2]);
            Vertices[4*V+2].Texture = ((Char-32)<<2) | 0b11;
            Vertices[4*V+3].Position = Mesh_EncodePosition(Positions[3]);
            Vertices[4*V+3].Texture = ((Char-32)<<2) | 0b01;
            
            V++;
        }
        
        P.X += A.X;
    }
    
    return Node->Index;
}

internal void
UI_FreeNode(ui *UI,
            u32 Index)
{
    ui_node *Node = UI_GetNode(UI, Index);
    ui_node *Parent = UI_GetNode(UI, Node->Parent);
    
    if(Parent->FirstChild == Node->Index)
        Parent->FirstChild = Node->NextSibling;
    else {
        ui_node *Sibling = UI_GetNode(UI, Parent->FirstChild);
        while(Sibling->NextSibling != Node->Index)
            Sibling = UI_GetNode(UI, Sibling->NextSibling);
        Sibling->NextSibling = Node->NextSibling;
    }
    
    // Mesh_FreeObject();
    Heap_Free(Node->Object.Indices);
    Heap_Free(Node->Object.Vertices);
    
    switch(Node->Type) {
        case UI_NODE_TYPE_STRING: {
            Heap_Free(Node->StringData.Lines);
            String_Free(Node->StringData.String);
        } break;
    }
}

//TODO: Consider a geometry shader, that takes an asset index and a
//      starting position and creates the character vertices

// internal void
// UI_UpdateMatrices(ui *UI)
// {
//     Stack_Push();
    
//     ui_propagation_frame *NodeStack = Stack_GetCursor();
    
//     u32 StackCount = 0;
//     NodeStack[StackCount++] = (ui_propagation_frame){M4x4r32_I, 0};
    
//     while(StackCount) {
//         ui_propagation_frame Frame = NodeStack[--StackCount];
//         ui_node *Node = UI_GetNode(UI, Frame.NodeIndex);
        
//         v4x4r32 OffsetMat = M4x4r32_Transformation2D(Node->Offset);
//         Node->Matrix = M4x4r32_Mul(Frame.Transform, OffsetMat);
        
//         // TODO actually change the mesh
        
//         if(Node->NextSibling) {
//             OffsetMat = M4x4r32_Transformation2D((v2r32){0,Node->Offset.Y+Node->Size.Y});
//             v4r32 Transform = M4x4r32_Mul(Frame.Transform, OffsetMat);
//             NodeStack[StackCount++] = (ui_propagation_frame){
//                 Transform,
//                 Node->NextSibling
//             };
//         }
//         if(Node->FirstChild) {
//             NodeStack[StackCount++] = (ui_propagation_frame){
//                 Node->Matrix;
//                 Node->FirstChild
//             };
//         }
//     }
    
//     Stack_Pop();
// }