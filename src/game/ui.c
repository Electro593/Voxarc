/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal ui_node *
UI_GetNode(ui *UI,
           u16 Index)
{
    if(!UI || Index == -1) return NULL;
    return (ui_node*)UI->Nodes->Data + Index;
}

internal void
UI_Init(ui *UI,
        mesh *Mesh,
        heap *Heap,
        v3u08 Color)
{
    UI->Mesh = Mesh;
    UI->NodeCount = 1;
    UI->Nodes = Heap_Allocate(Heap, sizeof(ui_node));
    
    ui_node *Node = UI_GetNode(UI, 0);
    Node->Index = 0;
    Node->ChildCount = 0;
    Node->FirstChild = UI_NULL_INDEX;
    Node->NextSibling = UI_NULL_INDEX;
    Node->Parent = UI_NULL_INDEX;
    Node->Flags = 0;
    
    u32 C = (Color.X<<18)|(Color.Y<<10)|(Color.Z<<2);
    Node->Object.Vertices = Heap_Allocate(Heap, Mesh->VertexSize*4);
    struct {
        u32 Position;
        u32 Texture;
    } *Vertices = (vptr)Node->Object.Vertices->Data;
    Vertices[0].Position = Mesh_EncodePosition((v3r32){-1,-1,0});
    Vertices[1].Position = Mesh_EncodePosition((v3r32){-1, 0,0});
    Vertices[2].Position = Mesh_EncodePosition((v3r32){ 0, 0,0});
    Vertices[3].Position = Mesh_EncodePosition((v3r32){ 0,-1,0});
    Vertices[0].Texture = 0x80000000 | C | 0b00;
    Vertices[1].Texture = 0x80000000 | C | 0b10;
    Vertices[2].Texture = 0x80000000 | C | 0b11;
    Vertices[3].Texture = 0x80000000 | C | 0b01;
    
    Node->Object.Indices = Heap_Allocate(Heap, sizeof(u32)*6);
    u32 *Indices = (u32*)Node->Object.Indices->Data;
    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 0;
    Indices[4] = 2;
    Indices[5] = 3;
    
    Node->Object.TranslationMatrix = M4x4r32_I;
    Node->Object.ScalingMatrix = M4x4r32_I;
    Node->Object.RotationMatrix = M4x4r32_I;
    
    Mesh_AddObjects(Mesh, 1, &Node->Object, &Node->ObjectIndex);
    Mesh_Update(Mesh);
    
    Node->Size = (v2r32){2,2};
}

internal void
UI_UpdateNodeString(ui *UI,
                    ui_node *Node,
                    string String,
                    v2r32 Pos,
                    v2r32 Size,
                    r32 FontHeight,
                    assetpack Assetpack)
{
    heap *Heap = Heap_GetHeap(UI->Nodes);
    
    u32 VertexSize = UI->Mesh->VertexSize;
    
    assetpack_texture *Cache['~'-' '+1] = {0};
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Assetpack, TAG_FONT_DEF);
    Assert(Tag);
    assetpack_font *Font = Tag->ValueP;
    
    u32 LineCount = 0;
    heap_handle *Lines = Heap_Allocate(Heap, LineCount*sizeof(u32));
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
    Node->Object.RotationMatrix = M4x4r32_I;
    
    if(Node->Flags & UI_NODE_INITIALIZED) {
        Heap_Resize(Node->Object.Indices, PrintedChars*sizeof(u32)*6);
        Heap_Resize(Node->Object.Vertices, PrintedChars*VertexSize*4);
    } else {
        Node->Object.Indices = Heap_Allocate(Heap, PrintedChars*sizeof(u32)*6);
        Node->Object.Vertices = Heap_Allocate(Heap, PrintedChars*VertexSize*4);
    }
    
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
    
    Heap_Free(Lines);
}

internal void
UI_CreateNodeObject(mesh *Mesh,
                    mesh_object *Object,
                    string String,
                    v3u08 Color,
                    v2r32 Size)
{
    
    Node->ObjectIndex = 0;
    Node->Object.Vertices = NULL;
    Node->Object.Indices = NULL;
    Node->Object.TranslationMatrix = M4x4r32_I;
    Node->Object.ScalingMatrix = M4x4r32_I;
    Node->Object.RotationMatrix = M4x4r32_I;
}

internal u16
UI_CreateNode(ui *UI,
              string String,
              v3u08 Color,
              v2r32 Size,
              u16 ParentIndex,
              u16 InsertIndex)
{
    UI->NodeCount++;
    if(UI->Nodes->Size < UI->NodeCount*sizeof(ui_node))
        Heap_Resize(UI->Nodes, UI->NodeCount*sizeof(ui_node)*2);
    ui_node *Node = UI_GetNode(UI, UI->NodeCount-1);
    Node->Index = UI->NodeCount-1;
    
    Node->ChildCount = 0;
    Node->FirstChild = UI_NULL_INDEX;
    Node->Flags = 0;
    Node->Size = Size;
    Node->Parent = ParentIndex;
    
    ui_node *Parent = UI_GetNode(UI, ParentIndex);
    ui_node *Sibling = UI_GetNode(UI, Parent->FirstChild);
    if(InsertIndex == 0) {
        Node->NextSibling = Parent->FirstChild;
        Parent->FirstChild = Node->Index;
    } else {
        for(u32 I = 1; I < InsertIndex && Sibling->NextSibling != UI_NULL_NODE; I++)
            Sibling = UI_GetNode(UI, Sibling->NextSibling);
        Node->NextSibling = Sibling->NextSibling;
        Sibling->NextSibling = Node->Index;
    }
    Parent->ChildCount++;
    
    UI_CreateNodeObject(UI->Mesh, &Node->Object, String, Color, Size);
    
    return Node->Index;
}

//TODO: Consider a geometry shader, that takes an asset index and a
//      starting position and creates the character vertices
