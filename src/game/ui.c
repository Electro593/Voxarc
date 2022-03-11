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
    
    r32 UnitAdvance = Font->Ascent-Font->Descent+Font->LineGap;
    
    v2r32 Scale = {FontHeight, FontHeight};
    v2r32 P = {-1,1-Font->Ascent*Scale.Y};
    v2r32 A = {0,UnitAdvance*Scale.Y};
    u32 PrintedChars = 0;
    r32 MaxWidth = 0;
    r32 CurrWidth = 0;
    r32 HighestAscent = 0;
    r32 LowestDescent = 0;
    u32 LastBreakChar = 0;
    r32 WidthSinceBreak;
    r32 AdvanceSinceBreak = 0;
    for(u32 C = 0; C < String.Length; C++) {
        c08 Char = String.Text[C];
        r32 Multiplier = 1;
        
        if(Char == '\n' || Char == ' ' || Char == '-') {
            LastBreakChar = C;
            AdvanceSinceBreak = 0;
            WidthSinceBreak = 0;
        }
        
        if(Char == '\n') {
            LineCount++;
            P.X = -1 + AdvanceSinceBreak;
            if(LineCount * sizeof(u32) > Lines->Size)
                Heap_Resize(Lines, LineCount*2*sizeof(u32));
            ((u32*)Lines->Data)[LineCount-1] = LastBreakChar+1;
            
            if(CurrWidth > MaxWidth) MaxWidth = CurrWidth;
            CurrWidth = WidthSinceBreak;
            
            continue;
        }
        if(Char == '\t') {
            Char = ' ';
            Multiplier = 4;
        }
        if(Char == 0) continue;
        
        if(!Cache[Char-' ']) {
            Tag = _Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)(u64)Char);
            Assert(Tag && Tag->AssetCount);
            Cache[Char-' '] = &(Tag->Assets[0])->Texture;
        }
        
        assetpack_texture *Asset = Cache[Char-' '];
        A.X = Asset->AdvanceX*Scale.X*Multiplier;
        
        if(Asset->Bearing.Y < LowestDescent)
            LowestDescent = Asset->Bearing.Y;
        if(Asset->Bearing.Y+Asset->SizeR.Y > HighestAscent)
            HighestAscent = Asset->Bearing.Y+Asset->SizeR.Y;
        
        if(P.X + A.X > 1) {
            P.X = -1 + AdvanceSinceBreak;
            LineCount++;
            if(LineCount * sizeof(u32) > Lines->Size)
                Heap_Resize(Lines, LineCount*2*sizeof(u32));
            
            if(AdvanceSinceBreak <= 1) {
                ((u32*)Lines->Data)[LineCount-1] = LastBreakChar+1;
            } else {
                ((u32*)Lines->Data)[LineCount-1] = C;
            }
            
            if(CurrWidth > MaxWidth) MaxWidth = CurrWidth;
            CurrWidth = WidthSinceBreak;
        }
        
        P.X += A.X;
        CurrWidth += Asset->AdvanceX;
        
        AdvanceSinceBreak += A.X;
        WidthSinceBreak += Asset->AdvanceX;
        
        PrintedChars++;
    }
    if(CurrWidth > MaxWidth) MaxWidth = CurrWidth;
    
    r32 HeightDiff = LineCount*UnitAdvance + HighestAscent-LowestDescent;
    Scale.X = 2/MaxWidth;
    Scale.Y = 2/HeightDiff;
    
    v2r32 D = {LineCount ? 1+P.X : 2,
               LineCount*A.Y+(Font->Ascent-Font->Descent)*Scale.Y};
    
    // Node->Object.Matrix.V[0] = (v4r32){D.X/2, 0,     0, -D.X/2};
    // Node->Object.Matrix.V[1] = (v4r32){0,     D.Y/2, 0,  D.Y/2};
    // Node->Object.Matrix.V[2] = (v4r32){0,     0,     1,  0};
    // Node->Object.Matrix.V[3] = (v4r32){0,     0,     0,  1};
    Node->Object.Matrix.V[0] = (v4r32){1, 0, 0, 0};
    Node->Object.Matrix.V[1] = (v4r32){0, 1, 0, 0};
    Node->Object.Matrix.V[2] = (v4r32){0, 0, 1, 0};
    Node->Object.Matrix.V[3] = (v4r32){0, 0, 0, 1};
    
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
    for(u32 C = 0; C < String.Length; C++) {
        c08 Char = String.Text[C];
        r32 Multiplier = 1;
        
        if(Char == '\n') continue;
        if(Char == '\0') continue;
        if(Char == '\t') {
            Multiplier = 4;
            Char = ' ';
        }
        
        Indices[6*V+0] = 4*V+0;
        Indices[6*V+1] = 4*V+1;
        Indices[6*V+2] = 4*V+2;
        Indices[6*V+3] = 4*V+0;
        Indices[6*V+4] = 4*V+2;
        Indices[6*V+5] = 4*V+3;
        
        assetpack_texture *Asset = Cache[Char-' '];
        A.X = Asset->AdvanceX*Multiplier*Scale.X;
        v2r32 B = V2r32_Mul(Asset->Bearing, Scale);
        v2r32 S = V2r32_Mul(Asset->SizeR, Scale);
        S.X *= Multiplier;
        
        if(LineCount && C == ((u32*)Lines->Data)[LineIndex]) {
            P.X = -1;
            P.Y -= A.Y;
            LineIndex++;
        }
        
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
        
        P.X += A.X;
        V++;
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