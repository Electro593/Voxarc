/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal u32
UI_EncodePositionInPercent(r32 X, r32 Y, u32 Z)
{
    return (Z<<24)|((u32)(Y*0xFFF)<<12)|((u32)(X*0xFFF)<<0);
}

internal u32
UI_EncodePositionInPixels(r32 X, r32 Y, s08 Z, v2u32 Size)
{
    return (Z<<24)|
           (((u32)(Y/Size.Y*0xFFF)&0xFFF)<<12)|
           (((u32)(X/Size.X*0xFFF)&0xFFF)<< 0);
}

internal ui_node *
UI_GetNode(ui *UI,
           u16 Index)
{
    if(!UI || Index == -1) return NULL;
    return (ui_node*)UI->Nodes->Data + Index;
}

internal void
UI_Init(ui *UI,
        heap *Heap,
        u32 *Program,
        assetpack Assetpack,
        v2u32 ViewportSize)
{
    Mesh_Init(&UI->Mesh, Heap, Program, MESH_HAS_TEXTURES|MESH_IS_FOR_UI);
    UI->NodeCount = 0;
    UI->Nodes = Heap_Allocate(Heap, sizeof(ui_node));
    UI->Assetpack = Assetpack;
    UI->ViewportSize = ViewportSize;
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
    
    u32 VertexSize = UI->Mesh.VertexSize;
    
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
            Tag = Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)(u64)Char);
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
    
    if(!(Node->Flags & UI_NODE_INITIALIZED)) {
        Node->Object.Indices = Heap_Allocate(Heap, PrintedChars*sizeof(u32)*6);
        Node->Object.Vertices = Heap_Allocate(Heap, PrintedChars*VertexSize*4);
    } else {
        Heap_Resize(Node->Object.Indices, PrintedChars*sizeof(u32)*6);
        Heap_Resize(Node->Object.Vertices, PrintedChars*VertexSize*4);
    }
    
    u32 *Indices = (u32*)Node->Object.Indices->Data;
    pt_vertex *Vertices = (vptr)Node->Object.Vertices->Data;
    
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

// TODO: Make a "font" that stores a char cache, and keep that in style

internal void
_UI_NextLine(ui_node_string *String,
             u32 Index)
{
    String->LineCount += 1;
    if(String->LineCount*sizeof(u32) > String->Lines->Size)
        Heap_Resize(String->Lines, MAX(String->LineCount*sizeof(u32), String->Lines->Size*2));
    ((u32*)String->Lines->Data)[String->LineCount-1] = Index;
}

internal ui_node_string
UI_CreateNodeString(heap *Heap,
                    ui_node_style Style,
                    string String,
                    assetpack Assetpack)
{
    ui_node_string Result;
    Result.LineCount = 0;
    Result.String = String;
    Result.Size = (v2u32){0};
    Result.Lines = Heap_Allocate(Heap, 0);
    Result.PrintableCount = 0;
    
    if(String.Length == 0) return Result;
    
    _UI_NextLine(&Result, 0);
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Assetpack, TAG_FONT_DEF);
    Assert(Tag);
    assetpack_font *Font = Tag->ValueP;
    r32 Y = Style.FontSize;
    r32 AdvanceY = (Font->LineGap + 1) * Style.FontSize;
    
    r32 X = 0;
    r32 ToAdvance;
    assetpack_texture *CharCache[95] = {0};
    Tag = Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)' ');
    Assert(Tag && Tag->AssetCount);
    CharCache[0] = &(Tag->Assets[0])->Texture;
    
    for(u32 I = 0; I < String.Length; I++) {
        c08 C = String.Text[I];
        
        if(C >= ' ' && C <= '~') {
            if(!CharCache[C-' ']) {
                Tag = Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)(u64)C);
                Assert(Tag && Tag->AssetCount);
                CharCache[C-' '] = &(Tag->Assets[0])->Texture;
            }
            
            assetpack_texture *Asset = CharCache[C-' '];
            
            ToAdvance = Asset->AdvanceX * Style.FontSize;
            
            if(C != ' ') Result.PrintableCount++;
        } else if(C == '\t') {
            ToAdvance = CharCache[0]->AdvanceX * Style.FontSize * 4;
        } else if(C == '\n') {
            if(Y + AdvanceY > Style.Size.Y) break;
            Y += AdvanceY;
            
            _UI_NextLine(&Result, I+1);
            X = 0;
            
            continue;
        } else if(C == '\r')
            continue;
        else if(!C)
            break;
        else Assert(FALSE, "Found unsupported character while creating node string");
        
        if(X + ToAdvance > Style.Size.X) {
            if(Y + AdvanceY > Style.Size.Y) break;
            Y += AdvanceY;
            
            _UI_NextLine(&Result, I);
            X = ToAdvance;
        } else {
            X += ToAdvance;
        }
    }
    
    Heap_Resize(Result.Lines, Result.LineCount*sizeof(u32));
    
    if(Result.LineCount == 1)
        Result.Size.X = X;
    else
        Result.Size.X = Style.Size.X;
    
    Result.Size.Y = Y;
    
    return Result;
}

internal mesh_object
UI_CreateNodeObject(mesh *Mesh,
                    ui_node_string String,
                    ui_node_style Style,
                    assetpack Assetpack,
                    v2u32 ViewSize)
{
    mesh_object Object;
    
    u32 VertexCount = /*4 +*/ String.PrintableCount*4;
    u32 IndexCount  = /*6 +*/ String.PrintableCount*6;
    heap *Heap = Heap_GetHeap(Mesh->Vertices);
    Object.Vertices = Heap_Allocate(Heap, VertexCount * Mesh->VertexSize);
    Object.Indices  = Heap_Allocate(Heap, IndexCount  * sizeof(u32));
    
    //
    // Make the border
    //
    u32 Color = (Style.BackgroundColor.X<<18)|
                (Style.BackgroundColor.Y<<10)|
                (Style.BackgroundColor.Z<< 2);
    pt_vertex *Vertices = (pt_vertex*)Object.Vertices->Data;
    // Vertices[0].Position = UI_EncodePositionInPercent(0, 0, Style.ZIndex);
    // Vertices[1].Position = UI_EncodePositionInPercent(0, 1, Style.ZIndex);
    // Vertices[2].Position = UI_EncodePositionInPercent(1, 1, Style.ZIndex);
    // Vertices[3].Position = UI_EncodePositionInPercent(1, 0, Style.ZIndex);
    // Vertices[0].Texture = 0x80000000 | Color | 0b00;
    // Vertices[1].Texture = 0x80000000 | Color | 0b10;
    // Vertices[2].Texture = 0x80000000 | Color | 0b11;
    // Vertices[3].Texture = 0x80000000 | Color | 0b01;
    // Vertices += 4;
    
    u32 IndexPattern[] = {0,1,2,0,2,3};
    u32 *Indices = (u32*)Object.Indices->Data;
    // Mem_Cpy(Indices, IndexPattern, sizeof(IndexPattern));
    // Indices += 6;
    
    //
    // Make the string
    //
    if(String.String.Length != 0) {
        assetpack_tag *Tag = Assetpack_FindFirstTag(Assetpack, TAG_FONT_DEF);
        Assert(Tag);
        assetpack_font *Font = Tag->ValueP;
        r32 Y = String.Size.Y - Font->Ascent*Style.FontSize;
        r32 AdvanceY = (Font->LineGap + 1) * Style.FontSize;
        
        assetpack_texture *CharCache[95] = {0};
        Tag = Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)' ');
        Assert(Tag && Tag->AssetCount);
        CharCache[0] = &(Tag->Assets[0])->Texture;
        
        u32 LineIndex = 0;
        u32 *Lines = (u32*)String.Lines->Data;
        
        r32 X = 0;
        u32 PrintedIndex = 0;
        for(u32 I = 0; I < String.String.Length; I++) {
            if(LineIndex+1 < String.LineCount && I == Lines[LineIndex+1]) {
                Y -= AdvanceY;
                X = 0;
                LineIndex++;
            }
            
            c08 C = String.String.Text[I];
            
            if(C >= ' ' && C <= '~') {
                if(!CharCache[C-' ']) {
                    Tag = Assetpack_FindExactTag(Assetpack, TAG_CODEPOINT, (vptr)(u64)C);
                    Assert(Tag && Tag->AssetCount);
                    CharCache[C-' '] = &(Tag->Assets[0])->Texture;
                }
                
                assetpack_texture *Asset = CharCache[C-' '];
                
                r32 PX = X + Asset->Bearing.X*Style.FontSize;
                r32 PY = Y + Asset->Bearing.Y*Style.FontSize;
                r32 SX = Asset->SizeR.X*Style.FontSize;
                r32 SY = Asset->SizeR.Y*Style.FontSize;
                Vertices[0].Position = UI_EncodePositionInPixels(PX,    PY,    Style.ZIndex-1, Style.Size);
                Vertices[1].Position = UI_EncodePositionInPixels(PX,    PY+SY, Style.ZIndex-1, Style.Size);
                Vertices[2].Position = UI_EncodePositionInPixels(PX+SX, PY+SY, Style.ZIndex-1, Style.Size);
                Vertices[3].Position = UI_EncodePositionInPixels(PX+SX, PY,    Style.ZIndex-1, Style.Size);
                Vertices[0].Texture = ((C-' ')<<2) | 0b00;
                Vertices[1].Texture = ((C-' ')<<2) | 0b10;
                Vertices[2].Texture = ((C-' ')<<2) | 0b11;
                Vertices[3].Texture = ((C-' ')<<2) | 0b01;
                Vertices += 4;
                
                for(u32 J = 0; J < 6; J++)
                    Indices[J] = IndexPattern[J] + /*4 + */4*PrintedIndex;
                Indices += 6;
                PrintedIndex++;
                
                X += Asset->AdvanceX * Style.FontSize;
            } else if(C == '\t') {
                X += CharCache[0]->AdvanceX * Style.FontSize * 4;
            } else if(!C) break;
        }
        
        Y -= -Font->Descent * Style.FontSize;
    }
    
    Object.TranslationMatrix = M4x4r32_Translation(-1, 1-(r32)String.Size.Y/Style.Size.Y, 0.5f);
    Object.ScalingMatrix = M4x4r32_Scaling(1/4095.0f*Style.Size.X/ViewSize.X*2, 1/4095.0f*Style.Size.Y/ViewSize.Y*2, 1);
    Object.RotationMatrix = M4x4r32_I;
    
    return Object;
}

internal u16
UI_CreateNode(ui *UI,
              string String,
              ui_node_style Style)
{
    heap *Heap = Heap_GetHeap(UI->Nodes);
    
    b08 IsRoot = UI->NodeCount == 0;
    u16 NodeIndex = UI->NodeCount;
    
    UI->NodeCount++;
    if(UI->Nodes->Size < UI->NodeCount*sizeof(ui_node))
        Heap_Resize(UI->Nodes, UI->NodeCount*sizeof(ui_node)*2);
    
    ui_node *Node = UI_GetNode(UI, NodeIndex);
    Node->Index = NodeIndex;
    Node->ChildCount = 0;
    Node->Children = NULL;
    Node->Parent = UI_NULL_INDEX;
    Node->Flags = 0;
    Node->Style = Style;
    
    Node->String = UI_CreateNodeString(Heap, Style, String, UI->Assetpack);
    Node->Object = UI_CreateNodeObject(&UI->Mesh, Node->String, Style, UI->Assetpack, UI->ViewportSize);
    
    Node->Flags |= UI_NODE_INITIALIZED;
    if(IsRoot) Node->Flags |= UI_NODE_LINKED;
    
    return Node->Index;
}

internal void
UI_AddChild(ui *UI,
            u16 ParentIndex,
            u16 ChildIndex)
{
    ui_node *Child = UI_GetNode(UI, ChildIndex);
    ui_node *Parent = UI_GetNode(UI, ParentIndex);
    Assert(Child && (Child->Flags & UI_NODE_INITIALIZED));
    Assert(Parent && (Parent->Flags & UI_NODE_INITIALIZED));
    
    Parent->ChildCount++;
    
    if(Parent->ChildCount == 1)
        Parent->Children = Heap_Allocate(Heap_GetHeap(UI->Nodes), sizeof(u16));
    else if(Parent->ChildCount*sizeof(u16) > Parent->Children->Size)
        Heap_Resize(Parent->Children, Parent->ChildCount*sizeof(u16));
    ((u16*)Parent->Children->Data)[Parent->ChildCount-1] = ChildIndex;
    
    Child->Parent = ParentIndex;
    Child->Flags |= UI_NODE_LINKED;
}

internal void
UI_Update(ui *UI)
{
    Stack_Push();
    
    u32 ToAdd=0;
    for(u32 I = 0; I < UI->NodeCount; I++) {
        ui_node *Node = ((ui_node*)UI->Nodes->Data)+I;
        
        if(!(Node->Flags & UI_NODE_LINKED)) continue;
        if((Node->Flags & UI_NODE_LINKED) && !(Node->Flags & UI_NODE_ADDED))
            ToAdd++;
    }
    
    mesh_object **ObjectsToAdd = Stack_Allocate(ToAdd*sizeof(mesh_object*));
    
    u32 AddedObjectIndex = 0;
    for(u32 I = 0; I < UI->NodeCount; I++) {
        ui_node *Node = ((ui_node*)UI->Nodes->Data)+I;
        
        if(!(Node->Flags & UI_NODE_LINKED)) continue;
        if((Node->Flags & UI_NODE_LINKED) && !(Node->Flags & UI_NODE_ADDED)) {
            ObjectsToAdd[AddedObjectIndex] = &Node->Object;
            Node->Flags |= UI_NODE_ADDED;
            AddedObjectIndex++;
        }
    }
    
    Mesh_AddObjects(&UI->Mesh, ToAdd, ObjectsToAdd);
    
    Mesh_Update(&UI->Mesh);
    Stack_Pop();
}

internal void
UI_Draw(ui *UI)
{
    Mesh_Draw(&UI->Mesh);
}

//TODO: Consider a geometry shader, that takes an asset index and a
//      starting position and creates the character vertices

// internal mesh_object
// MakeUIRectObject(heap *Heap, v2u32 Pos, v2u32 Size, ui_style Style)
// {
//     mesh_object Object;
    
//     Object.Indices = Heap_Allocate(Heap, 4*sizeof(u32));
//     Object.Vertices = Heap_Allocate(Heap, 4*sizeof(u32));
// }