/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum ui_node_type {
    UI_NODE_TYPE_NONE=0,
    UI_NODE_TYPE_STRING,
} ui_node_type;

typedef enum ui_node_flags {
    UI_NODE_MATRIX_UPDATED = 1,
} ui_node_flags;

typedef struct ui_string_node {
    string String;
    HEAP(u32*) Lines;
} ui_string_node;

typedef struct ui_node {
    u16 Index;
    u16 ChildCount;
    u16 FirstChild;
    u16 NextSibling;
    u16 Parent;
    
    b08 Flags;
    
    mesh_object Object;
    v2r32 Offset;
    v2r32 Size;
    
    ui_node_type Type;
    union {
        ui_string_node StringData;
    };
} ui_node;

typedef struct ui {
    mesh *Mesh;
    
    u32 NodeCount;
    HEAP(ui_node*) Nodes;
} ui;

typedef struct ui_propagation_frame {
    m4x4r32 Transform;
    u16 NodeIndex;
} ui_propagation_frame;