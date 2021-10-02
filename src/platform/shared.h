/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PLATFORM_SHARED_H_
#define PLATFORM_SHARED_H_

// typedef enum data_type
// {
//     DATA_TYPE_VPTR,
//     DATA_TYPE_S08,
//     DATA_TYPE_S16,
//     DATA_TYPE_S32,
//     DATA_TYPE_S64,
//     DATA_TYPE_U08,
//     DATA_TYPE_U16,
//     DATA_TYPE_U32,
//     DATA_TYPE_U64,
//     DATA_TYPE_V3S32,
//     DATA_TYPE_V3U32,
// } data_type;

typedef enum keycode
{
    KEYCODE_UNKNOWN         = 0x00,
    KEYCODE_ESCAPE          = 0x01,
    KEYCODE_SPACE           = 0x02,
    KEYCODE_TAB             = 0x03,
    KEYCODE_ENTER           = 0x04,
    KEYCODE_INSERT          = 0x05,
    KEYCODE_BACKSPACE       = 0x06,
    KEYCODE_DELETE          = 0x07,
    KEYCODE_0               = 0x08,
    KEYCODE_1               = 0x09,
    KEYCODE_2               = 0x0A,
    KEYCODE_3               = 0x0B,
    KEYCODE_4               = 0x0C,
    KEYCODE_5               = 0x0D,
    KEYCODE_6               = 0x0E,
    KEYCODE_7               = 0x0F,
    KEYCODE_8               = 0x10,
    KEYCODE_9               = 0x11,
    KEYCODE_A               = 0x12,
    KEYCODE_B               = 0x13,
    KEYCODE_C               = 0x14,
    KEYCODE_D               = 0x15,
    KEYCODE_E               = 0x16,
    KEYCODE_F               = 0x17,
    KEYCODE_G               = 0x18,
    KEYCODE_H               = 0x19,
    KEYCODE_I               = 0x1A,
    KEYCODE_J               = 0x1B,
    KEYCODE_K               = 0x1C,
    KEYCODE_L               = 0x1D,
    KEYCODE_M               = 0x1E,
    KEYCODE_N               = 0x1F,
    KEYCODE_O               = 0x20,
    KEYCODE_P               = 0x21,
    KEYCODE_Q               = 0x22,
    KEYCODE_R               = 0x23,
    KEYCODE_S               = 0x24,
    KEYCODE_T               = 0x25,
    KEYCODE_U               = 0x26,
    KEYCODE_V               = 0x27,
    KEYCODE_W               = 0x28,
    KEYCODE_X               = 0x29,
    KEYCODE_Y               = 0x2A,
    KEYCODE_Z               = 0x2B,
    KEYCODE_GRAVE           = 0x2C,
    KEYCODE_MINUS           = 0x2D,
    KEYCODE_EQUAL           = 0x2E,
    KEYCODE_LEFT_BRACKET    = 0x2F,
    KEYCODE_RIGHT_BRACKET   = 0x30,
    KEYCODE_BACKSLASH       = 0x31,
    KEYCODE_SEMICOLON       = 0x32,
    KEYCODE_APOSTROPHE      = 0x33,
    KEYCODE_COMMA           = 0x34,
    KEYCODE_PERIOD          = 0x35,
    KEYCODE_SLASH           = 0x36,
    KEYCODE_WORLD_1         = 0x37,
    KEYCODE_WORLD_2         = 0x38,
    KEYCODE_NUMPAD_0        = 0x39,
    KEYCODE_NUMPAD_1        = 0x3A,
    KEYCODE_NUMPAD_2        = 0x3B,
    KEYCODE_NUMPAD_3        = 0x3C,
    KEYCODE_NUMPAD_4        = 0x3D,
    KEYCODE_NUMPAD_5        = 0x3E,
    KEYCODE_NUMPAD_6        = 0x3F,
    KEYCODE_NUMPAD_7        = 0x40,
    KEYCODE_NUMPAD_8        = 0x41,
    KEYCODE_NUMPAD_9        = 0x42,
    KEYCODE_NUMPAD_DECIMAL  = 0x43,
    KEYCODE_NUMPAD_DIVIDE   = 0x44,
    KEYCODE_NUMPAD_MULTIPLY = 0x45,
    KEYCODE_NUMPAD_SUBTRACT = 0x46,
    KEYCODE_NUMPAD_ADD      = 0x47,
    KEYCODE_NUMPAD_ENTER    = 0x48,
    KEYCODE_NUMPAD_EQUAL    = 0x49,
    KEYCODE_F1              = 0x4A,
    KEYCODE_F2              = 0x4B,
    KEYCODE_F3              = 0x4C,
    KEYCODE_F4              = 0x4D,
    KEYCODE_F5              = 0x4E,
    KEYCODE_F6              = 0x4F,
    KEYCODE_F7              = 0x50,
    KEYCODE_F8              = 0x51,
    KEYCODE_F9              = 0x52,
    KEYCODE_F10             = 0x53,
    KEYCODE_F11             = 0x54,
    KEYCODE_F12             = 0x55,
    KEYCODE_F13             = 0x56,
    KEYCODE_F14             = 0x57,
    KEYCODE_F15             = 0x58,
    KEYCODE_F16             = 0x59,
    KEYCODE_F17             = 0x5A,
    KEYCODE_F18             = 0x5B,
    KEYCODE_F19             = 0x5C,
    KEYCODE_F20             = 0x5D,
    KEYCODE_F21             = 0x5E,
    KEYCODE_F22             = 0x5F,
    KEYCODE_F23             = 0x60,
    KEYCODE_F24             = 0x61,
    KEYCODE_PRINT_SCREEN    = 0x62,
    KEYCODE_PAUSE           = 0x63,
    KEYCODE_CAPS_LOCK       = 0x64,
    KEYCODE_SCROLL_LOCK     = 0x65,
    KEYCODE_NUM_LOCK        = 0x66,
    KEYCODE_LEFT_SHIFT      = 0x67,
    KEYCODE_LEFT_CONTROL    = 0x68,
    KEYCODE_LEFT_ALT        = 0x69,
    KEYCODE_LEFT_SUPER      = 0x6A,
    KEYCODE_RIGHT_SHIFT     = 0x6B,
    KEYCODE_RIGHT_CONTROL   = 0x6C,
    KEYCODE_RIGHT_ALT       = 0x6D,
    KEYCODE_RIGHT_SUPER     = 0x6E,
    KEYCODE_MENU            = 0x6F,
    KEYCODE_ARROW_LEFT      = 0x70,
    KEYCODE_ARROW_UP        = 0x71,
    KEYCODE_ARROW_RIGHT     = 0x72,
    KEYCODE_ARROW_DOWN      = 0x73,
    KEYCODE_PAGE_UP         = 0x74,
    KEYCODE_PAGE_DOWN       = 0x75,
    KEYCODE_HOME            = 0x76,
    KEYCODE_END             = 0x77,
    
    KEYCODE_FIRST           = KEYCODE_UNKNOWN,
    KEYCODE_LAST            = KEYCODE_END,
} keycode;

typedef enum scancode
{
    SCANCODE_UNKNOWN = 0x00,
} scancode;

typedef enum button
{
    BUTTON_1      = 0x00,
    BUTTON_2      = 0x01,
    BUTTON_3      = 0x02,
    BUTTON_4      = 0x03,
    BUTTON_5      = 0x04,
    BUTTON_6      = 0x05,
    BUTTON_7      = 0x06,
    BUTTON_8      = 0x07,
    
    BUTTON_LEFT   = BUTTON_1,
    BUTTON_RIGHT  = BUTTON_2,
    BUTTON_MIDDLE = BUTTON_3,
    BUTTON_LAST   = BUTTON_8,
} button;

typedef enum file_mode
{
    FileMode_Read,
    FileMode_Write,
    FileMode_Append,
} file_mode;

typedef enum execution_state
{
    EXECUTION_PAUSED  = 0x00,
    EXECUTION_RUNNING = 0x01,
    EXECUTION_ENDED   = 0x02,
 } execution_state;

typedef enum window_flag_bit
{
    WINDOW_MINIMIZED    = 0x01,
    WINDOW_MAXIMIZED    = 0x02,
} window_flag_bit;
typedef flag08 window_flags;

typedef enum focus_state
{
    FOCUS_NONE   = 0x00,
    FOCUS_CLIENT = 0x01,
    FOCUS_FRAME  = 0x02,
} focus_state;

typedef enum key_state
{
    KEY_RELEASED = 0x00,
    KEY_PRESSED  = 0x01,
    KEY_REPEATED = 0x02,
} key_state;

typedef enum button_state
{
    BUTTON_RELEASED = 0x00,
    BUTTON_PRESSED  = 0x01,
} button_state;

//NOTE: Platform-to-game flags
typedef enum update_flag_bit
{
    UPDATE_SIZE     = 0x01,
    UPDATE_RELOADED = 0x02,
} update_flag_bit;
typedef flag08 update_flags;

//NOTE: If platform-specific abilities can be used
typedef enum support_flags_bit
{
    SUPPORT_RAW_INPUT_BIT = 0x01,
} support_flags_bit;
typedef flag08 support_flags;

typedef enum cursor_flag_bit
{
    CURSOR_DISABLED_BIT = 0x01,
    CURSOR_RAW_BIT      = 0x02,
    CURSOR_ENTERED_BIT  = 0x04,
} cursor_flag_bit;
typedef flag08 cursor_flags;

typedef struct file_data
{
    u64 FileSize;
    str FileName;
    str RelDir;
} file_data;

typedef void (*platform_work_queue_callback)(vptr Parameter);

typedef struct platform_work_queue_entry
{
    platform_work_queue_callback Function;
    vptr Parameter;
} platform_work_queue_entry;

typedef struct platform_work_queue
{
    volatile u32 ExecuteCursor;
    volatile u32 ExecutedEntryCount;
    volatile u32 CreateCursor;
    volatile u32 CreatedEntryCount;
    
#define MAX_WORK_QUEUE_ENTRIES 256
    hvptr Entries; //platform_work_queue_entry
    
    vptr Semaphore;
} platform_work_queue;

typedef struct platform_state
{
    execution_state ExecutionState;
    focus_state FocusState;
    update_flags UpdateFlags;
    support_flags SupportFlags;
    window_flags WindowFlags;
    cursor_flags CursorFlags;
    
    v2u32 Size;
    
    u08 Keycodes[512];
    u16 Scancodes[KEYCODE_LAST + 1];
    chr KeyNames[KEYCODE_LAST + 1][5];
    
    vptr LoaderState;
    
    u64 AllocationSize;
    u64 AllocationUsed;
    mem AllocationBase;
} platform_state;

typedef struct platform_exports platform_exports;

#endif