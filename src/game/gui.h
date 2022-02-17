/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum gui_node_type {
    GUI_TEXT,
} gui_node_type;

typedef struct gui_text {
    string Text;
} gui_text;

typedef struct gui_node {
    v2r32 Offset;
    u32 ChildCount;
    struct gui_node *Children;
    
    gui_node_type Type;
    union {
        gui_text Text;
    };
} gui_node;

typedef struct gui_text_box {
    string Text;
    
    font *Font;
} gui_text_box;