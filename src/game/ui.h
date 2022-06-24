/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define UI_NULL_INDEX ((u16)-1)

typedef enum ui_node_type {
    UI_NODE_TYPE_NONE=0,
    UI_NODE_TYPE_STRING,
} ui_node_type;

typedef enum ui_node_flags {
    UI_NODE_INITIALIZED = 0x01,
    UI_NODE_LINKED      = 0x02,
    UI_NODE_ADDED       = 0x04,
    UI_NODE_UPDATED     = 0x08,
    UI_NODE_HIDDEN      = 0x10,
} ui_node_flags;

typedef struct ui_node_style {
    v2u32 Size;
    u32 ZIndex;
    r32 FontSize;
    v3u08 Color;
    v3u08 BackgroundColor;
} ui_node_style;

typedef struct ui_node_string {
    string String;
    v2u32 Size;
    HEAP(u32*) Lines;
    u32 LineCount;
    u32 PrintableCount;
} ui_node_string;

typedef struct ui_node {
    u16 Index;
    u16 ChildCount;
    HEAP(u16*) Children;
    u16 Parent;
    
    u16 Flags;
    
    ui_node_style Style;
    ui_node_string String;
    
    mesh_object Object;
} ui_node;

typedef struct ui {
    mesh Mesh;
    assetpack Assetpack;
    
    u32 NodeCount;
    HEAP(ui_node*) Nodes;
    
    v2u32 ViewportSize;
} ui;

typedef struct ui_propagation_frame {
    m4x4r32 Transform;
    u16 NodeIndex;
} ui_propagation_frame;



// typedef struct ui_style {
//     v4u08 BorderRadius;
//     v4u08 BackgroundColor;
//     v4u08 TextColor;
// } ui_style;