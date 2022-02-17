/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DECLARE_HANDLE(Name) \
    struct Name##__{int Unused;}; \
    typedef struct Name##__ *Name;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

#define MAKEINTRESOURCEA(N) ((chr*)((u64)((u16)(N))))
#define MAKEINTRESOURCEW(N) ((wchr*)((u64)((u16)(N))))

#define IDC_ARROW_A MAKEINTRESOURCEA(32512)
#define IDC_ARROW_W MAKEINTRESOURCEW(32512)

#define GET_XBUTTON_WPARAM(WParam) HIGH_16(WParam)
#define XBUTTON1 0x0001
#define XBUTTON2 0x0002

#define GET_WHEEL_DELTA_WPARAM(WParam) ((s16)HIGH_16(WParam))
#define WHEEL_DELTA 120

#define GET_X_LPARAM(LParam) ((s32)(s16)LOW_16(LParam))
#define GET_Y_LPARAM(LParam) ((s32)(s16)HIGH_16(LParam))

#define EXCEPTION_MAXIMUM_PARAMETERS 15

#define CW_USEDEFAULT ((s32)0x80000000)

#define INVALID_HANDLE_VALUE ((vptr)(s64)-1)

#define FILE_ATTRIBUTE_NORMAL 0x80

#define WPF_ASYNCWINDOWPLACEMENT 0x0004

#define CS_VREDRAW  0x0001
#define CS_HREDRAW  0x0002
#define CS_OWNDC    0x0020
#define CS_SAVEBITS 0x0800

#define GWL_EXSTYLE (-20)
#define GWL_STYLE   (-16)

#define LWA_ALPHA 0x00000002

#define HID_USAGE_PAGE_GENERIC ((u16)0x01)

#define HID_USAGE_GENERIC_MOUSE ((u16)0x02)

#define HKEY_CURRENT_USER ((win32_key)(u64)((s32)0x80000001))

#define ERROR_SUCCESS          0L
#define ERROR_FILE_NOT_FOUND   2L
#define ERROR_PATH_NOT_FOUND   3L
#define ERROR_ALREADY_EXISTS 183L

#define SPI_GETWORKAREA 0x0030

#define READ_CONTROL  0x00020000L
#define SYNCHRONIZE   0x00100000L
#define GENERIC_WRITE 0x40000000L
#define GENERIC_READ  0x80000000L

#define PS_INSIDEFRAME 6

#define STANDARD_RIGHTS_READ READ_CONTROL

#define REG_DWORD 4UL

#define KEY_QUERY_VALUE        0x0001
#define KEY_ENUMERATE_SUB_KEYS 0x0008
#define KEY_NOTIFY             0x0010
#define KEY_READ               ((STANDARD_RIGHTS_READ | \
                                 KEY_QUERY_VALUE | \
                                 KEY_ENUMERATE_SUB_KEYS | \
                                 KEY_NOTIFY) & \
                                (~SYNCHRONIZE))

#define KF_EXTENDED 0x0100
#define KF_UP       0x8000

#define WMSZ_LEFT        1
#define WMSZ_RIGHT       2
#define WMSZ_TOP         3
#define WMSZ_TOPLEFT     4
#define WMSZ_TOPRIGHT    5
#define WMSZ_BOTTOM      6
#define WMSZ_BOTTOMLEFT  7
#define WMSZ_BOTTOMRIGHT 8

#define WM_DESTROY         0x0002
#define WM_MOVE            0x0003
#define WM_SIZE            0x0005
#define WM_ACTIVATE        0x0006
#define WM_SETFOCUS        0x0007
#define WM_KILLFOCUS       0x0008
#define WM_PAINT           0x000F
#define WM_CLOSE           0x0010
#define WM_QUIT            0x0012
#define WM_ERASEBKGND      0x0014
#define WM_MOUSEACTIVATE   0x0021
#define WM_GETMINMAXINFO   0x0024
#define WM_INPUTLANGCHANGE 0x0051
#define WM_NCMOUSEMOVE     0x00A0
#define WM_NCLBUTTONDOWN   0x00A1
#define WM_NCLBUTTONUP     0x00A2
#define WM_DISPLAYCHANGE   0x007E
#define WM_INPUT           0x00FF
#define WM_KEYDOWN         0x0100
#define WM_KEYUP           0x0101
#define WM_CHAR            0x0102
#define WM_SYSKEYDOWN      0x0104
#define WM_SYSKEYUP        0x0105
#define WM_SYSCHAR         0x0106
#define WM_UNICHAR         0x0109
#define WM_SYSCOMMAND      0x0112
#define WM_MOUSEMOVE       0x0200
#define WM_LBUTTONDOWN     0x0201
#define WM_LBUTTONUP       0x0202
#define WM_RBUTTONDOWN     0x0204
#define WM_RBUTTONUP       0x0205
#define WM_MBUTTONDOWN     0x0207
#define WM_MBUTTONUP       0x0208
#define WM_MOUSEWHEEL      0x020A
#define WM_XBUTTONDOWN     0x020B
#define WM_XBUTTONUP       0x020C
#define WM_MOUSEHWHEEL     0x020E
#define WM_ENTERMENULOOP   0x0211
#define WM_EXITMENULOOP    0x0212
#define WM_SIZING          0x0214
#define WM_CAPTURECHANGED  0x0215
#define WM_ENTERSIZEMOVE   0x0231
#define WM_EXITSIZEMOVE    0x0232
#define WM_MOUSELEAVE      0x02A3

#define UNICODE_NOCHAR     0xFFFF

#define SIZE_RESTORED  0
#define SIZE_MINIMIZED 1
#define SIZE_MAXIMIZED 2

#define WA_ACTIVE 1

#define MOUSE_MOVE_ABSOLUTE 1

#define MONITOR_DEFAULTTONEAREST 0x00000002

#define WS_OVERLAPPED  0x00000000L
#define WS_MAXIMIZEBOX 0x00010000L
#define WS_MINIMIZEBOX 0x00020000L
#define WS_THICKFRAME  0x00040000L
#define WS_SYSMENU     0x00080000L
#define WS_BORDER      0x00800000L
#define WS_CAPTION     0x00C00000L
#define WS_VISIBLE     0x10000000L
#define WS_CHILD       0x40000000L
#define WS_POPUP       0x80000000L
#define WS_SIZEBOX     WS_THICKFRAME
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED  | \
                             WS_CAPTION     | \
                             WS_SYSMENU     | \
                             WS_THICKFRAME  | \
                             WS_MINIMIZEBOX | \
                             WS_MAXIMIZEBOX)

#define WS_EX_TOPMOST     0x00000008L
#define WS_EX_TRANSPARENT 0x00000020L
#define WS_EX_WINDOWEDGE  0x00000100L
#define WS_EX_CLIENTEDGE  0x00000200L
#define WS_EX_LAYERED     0x00080000L
#define WS_EX_NOACTIVATE  0x08000000L
#define WS_EX_OVERLAPPEDWINDOW (WS_EX_WINDOWEDGE | \
                                WS_EX_CLIENTEDGE)

#define MAPVK_VK_TO_VSC 0
#define MAPVK_VSC_TO_VK 1

#define COLOR_HIGHLIGHT 13
#define COLOR_HOTLIGHT  26

#define SM_CXSCREEN    0
#define SM_CYSCREEN    1
#define SM_CXFRAME    32
#define SM_CXMINTRACK 34
#define SM_CYMINTRACK 35
#define SM_CXMAXTRACK 59
#define SM_CYMAXTRACK 60

#define MEM_COMMIT  0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_RELEASE 0x00008000

#define RDW_INVALIDATE    0x0001
#define RDW_INTERNALPAINT 0x0002
#define RDW_ERASE         0x0004
#define RDW_ALLCHILDREN   0x0080
#define RDW_UPDATENOW     0x0100
#define RDW_FRAME         0x0400

#define SWP_NOACTIVATE 0x0010

#define STANDARD_RIGHTS_REQUIRED 0x000F0000L
#define SYNCHRONIZE              0x00100000L

#define SEMAPHORE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | \
                              SYNCHRONIZE | \
                              0x3)

#define SW_HIDE            0
#define SW_MAXIMIZE        3
#define SW_SHOWMINNOACTIVE 7
#define SW_SHOWNA          8
#define SW_RESTORE         9

#define PAGE_READWRITE 0x04

#define RIDEV_REMOVE 0x00000001

#define RID_INPUT 0x10000003

#define MAX_PATH 260

#define CREATE_NEW    1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3

#define FILE_SHARE_READ 0x00000001

#define TME_LEAVE 0x00000002

#define CP_UTF8 65001

#define BS_NULL   1
#define BS_HOLLOW BS_NULL

#define PFD_TYPE_RGBA               0
#define PFD_MAIN_PLANE              0
#define PFD_DOUBLEBUFFER   0x00000001
#define PFD_DRAW_TO_WINDOW 0x00000004
#define PFD_SUPPORT_OPENGL 0x00000020

#define PM_NOREMOVE 0x0000
#define PM_REMOVE   0x0001

#define SC_SIZE    0xF000
#define SC_MOVE    0xF010
#define SC_KEYMENU 0xF100

#define INFINITE 0xFFFFFFFF

#define HWND_TOPMOST ((win32_window)-1)
#define HWND_DESKTOP ((win32_window)0)

#define HTCLIENT       1
#define HTCAPTION      2
#define HTLEFT        10
#define HTRIGHT       11
#define HTTOP         12
#define HTTOPLEFT     13
#define HTTOPRIGHT    14
#define HTBOTTOM      15
#define HTBOTTOMLEFT  16
#define HTBOTTOMRIGHT 17

#if 1
#define VK_LBUTTON                         0x01
#define VK_RBUTTON                         0x02
#define VK_CANCEL                          0x03
#define VK_MBUTTON                         0x04
#define VK_XBUTTON1                        0x05
#define VK_XBUTTON2                        0x06
//      RESERVED                           0x07
#define VK_BACK                            0x08
#define VK_TAB                             0x09
//      RESERVED                           0x0A
//      RESERVED                           0x0B
#define VK_CLEAR                           0x0C
#define VK_RETURN                          0x0D
//      UNASSIGNED                         0x0E
//      UNASSIGNED                         0x0F
#define VK_SHIFT                           0x10
#define VK_CONTROL                         0x11
#define VK_MENU                            0x12
#define VK_PAUSE                           0x13
#define VK_CAPITAL                         0x14
#define VK_KANA                            0x15
#define VK_HANGEUL                         0x15
#define VK_HANGUL                          0x15
//      UNASSIGNED                         0x16
#define VK_JUNJA                           0x17
#define VK_FINAL                           0x18
#define VK_HANJA                           0x19
#define VK_KANJI                           0x19
//      UNASSIGNED                         0x1A
#define VK_ESCAPE                          0x1B
#define VK_CONVERT                         0x1C
#define VK_NONCONVERT                      0x1D
#define VK_ACCEPT                          0x1E
#define VK_MODECHANGE                      0x1F
#define VK_SPACE                           0x20
#define VK_PRIOR                           0x21
#define VK_NEXT                            0x22
#define VK_END                             0x23
#define VK_HOME                            0x24
#define VK_LEFT                            0x25
#define VK_UP                              0x26
#define VK_RIGHT                           0x27
#define VK_DOWN                            0x28
#define VK_SELECT                          0x29
#define VK_PRINT                           0x2A
#define VK_EXECUTE                         0x2B
#define VK_SNAPSHOT                        0x2C
#define VK_INSERT                          0x2D
#define VK_DELETE                          0x2E
#define VK_HELP                            0x2F
#define VK_0                               0x30
#define VK_1                               0x31
#define VK_2                               0x32
#define VK_3                               0x33
#define VK_4                               0x34
#define VK_5                               0x35
#define VK_6                               0x36
#define VK_7                               0x37
#define VK_8                               0x38
#define VK_9                               0x39
//      UNASSIGNED                         0x3A
//      UNASSIGNED                         0x3B
//      UNASSIGNED                         0x3C
//      UNASSIGNED                         0x3D
//      UNASSIGNED                         0x3E
//      UNASSIGNED                         0x3F
//      UNASSIGNED                         0x40
#define VK_A                               0x41
#define VK_B                               0x42
#define VK_C                               0x43
#define VK_D                               0x44
#define VK_E                               0x45
#define VK_F                               0x46
#define VK_G                               0x47
#define VK_H                               0x48
#define VK_I                               0x49
#define VK_J                               0x4A
#define VK_K                               0x4B
#define VK_L                               0x4C
#define VK_M                               0x4D
#define VK_N                               0x4E
#define VK_O                               0x4F
#define VK_P                               0x50
#define VK_Q                               0x51
#define VK_R                               0x52
#define VK_S                               0x53
#define VK_T                               0x54
#define VK_U                               0x55
#define VK_V                               0x56
#define VK_W                               0x57
#define VK_X                               0x58
#define VK_Y                               0x59
#define VK_Z                               0x5A
#define VK_LWIN                            0x5B
#define VK_RWIN                            0x5C
#define VK_APPS                            0x5D
//      RESERVED                           0x5E
#define VK_SLEEP                           0x5F
#define VK_NUMPAD0                         0x60
#define VK_NUMPAD1                         0x61
#define VK_NUMPAD2                         0x62
#define VK_NUMPAD3                         0x63
#define VK_NUMPAD4                         0x64
#define VK_NUMPAD5                         0x65
#define VK_NUMPAD6                         0x66
#define VK_NUMPAD7                         0x67
#define VK_NUMPAD8                         0x68
#define VK_NUMPAD9                         0x69
#define VK_MULTIPLY                        0x6A
#define VK_ADD                             0x6B
#define VK_SEPARATOR                       0x6C
#define VK_SUBTRACT                        0x6D
#define VK_DECIMAL                         0x6E
#define VK_DIVIDE                          0x6F
#define VK_F1                              0x70
#define VK_F2                              0x71
#define VK_F3                              0x72
#define VK_F4                              0x73
#define VK_F5                              0x74
#define VK_F6                              0x75
#define VK_F7                              0x76
#define VK_F8                              0x77
#define VK_F9                              0x78
#define VK_F10                             0x79
#define VK_F11                             0x7A
#define VK_F12                             0x7B
#define VK_F13                             0x7C
#define VK_F14                             0x7D
#define VK_F15                             0x7E
#define VK_F16                             0x7F
#define VK_F17                             0x80
#define VK_F18                             0x81
#define VK_F19                             0x82
#define VK_F20                             0x83
#define VK_F21                             0x84
#define VK_F22                             0x85
#define VK_F23                             0x86
#define VK_F24                             0x87
#define VK_NAVIGATION_VIEW                 0x88
#define VK_NAVIGATION_MENU                 0x89
#define VK_NAVIGATION_UP                   0x8A
#define VK_NAVIGATION_DOWN                 0x8B
#define VK_NAVIGATION_LEFT                 0x8C
#define VK_NAVIGATION_RIGHT                0x8D
#define VK_NAVIGATION_ACCEPT               0x8E
#define VK_NAVIGATION_CANCEL               0x8F
#define VK_NUMLOCK                         0x90
#define VK_SCROLL                          0x91
#define VK_OEM_NEC_EQUAL                   0x92
#define VK_OEM_FJ_JISHO                    0x92
#define VK_OEM_FJ_MASSHOU                  0x93
#define VK_OEM_FJ_TOUROKU                  0x94
#define VK_OEM_FJ_LOYA                     0x95
#define VK_OEM_FJ_ROYA                     0x96
//      UNASSIGNED                         0x97
//      UNASSIGNED                         0x98
//      UNASSIGNED                         0x99
//      UNASSIGNED                         0x9A
//      UNASSIGNED                         0x9B
//      UNASSIGNED                         0x9C
//      UNASSIGNED                         0x9D
//      UNASSIGNED                         0x9E
//      UNASSIGNED                         0x9F
#define VK_LSHIFT                          0xA0
#define VK_RSHIFT                          0xA1
#define VK_LCONTROL                        0xA2
#define VK_RCONTROL                        0xA3
#define VK_LMENU                           0xA4
#define VK_RMENU                           0xA5
#define VK_BROWSER_BACK                    0xA6
#define VK_BROWSER_FORWARD                 0xA7
#define VK_BROWSER_REFRESH                 0xA8
#define VK_BROWSER_STOP                    0xA9
#define VK_BROWSER_SEARCH                  0xAA
#define VK_BROWSER_FAVORITES               0xAB
#define VK_BROWSER_HOME                    0xAC
#define VK_VOLUME_MUTE                     0xAD
#define VK_VOLUME_DOWN                     0xAE
#define VK_VOLUME_UP                       0xAF
#define VK_MEDIA_NEXT_TRACK                0xB0
#define VK_MEDIA_PREV_TRACK                0xB1
#define VK_MEDIA_STOP                      0xB2
#define VK_MEDIA_PLAY_PAUSE                0xB3
#define VK_LAUNCH_MAIL                     0xB4
#define VK_LAUNCH_MEDIA_SELECT             0xB5
#define VK_LAUNCH_APP1                     0xB6
#define VK_LAUNCH_APP2                     0xB7
//      RESERVED                           0xB8
//      RESERVED                           0xB9
#define VK_OEM_1                           0xBA
#define VK_OEM_PLUS                        0xBB
#define VK_OEM_COMMA                       0xBC
#define VK_OEM_MINUS                       0xBD
#define VK_OEM_PERIOD                      0xBE
#define VK_OEM_2                           0xBF
#define VK_OEM_3                           0xC0
#define VK_ABNT_C1                         0xC1
#define VK_ABNT_C2                         0xC2
#define VK_GAMEPAD_A                       0xC3
#define VK_GAMEPAD_B                       0xC4
#define VK_GAMEPAD_X                       0xC5
#define VK_GAMEPAD_Y                       0xC6
#define VK_GAMEPAD_RIGHT_SHOULDER          0xC7
#define VK_GAMEPAD_LEFT_SHOULDER           0xC8
#define VK_GAMEPAD_LEFT_TRIGGER            0xC9
#define VK_GAMEPAD_RIGHT_TRIGGER           0xCA
#define VK_GAMEPAD_DPAD_UP                 0xCB
#define VK_GAMEPAD_DPAD_DOWN               0xCC
#define VK_GAMEPAD_DPAD_LEFT               0xCD
#define VK_GAMEPAD_DPAD_RIGHT              0xCE
#define VK_GAMEPAD_MENU                    0xCF
#define VK_GAMEPAD_VIEW                    0xD0
#define VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON  0xD1
#define VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON 0xD2
#define VK_GAMEPAD_LEFT_THUMBSTICK_UP      0xD3
#define VK_GAMEPAD_LEFT_THUMBSTICK_DOWN    0xD4
#define VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT   0xD5
#define VK_GAMEPAD_LEFT_THUMBSTICK_LEFT    0xD6
#define VK_GAMEPAD_RIGHT_THUMBSTICK_UP     0xD7
#define VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN   0xD8
#define VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT  0xD9
#define VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT   0xDA
#define VK_OEM_4                           0xDB
#define VK_OEM_5                           0xDC
#define VK_OEM_6                           0xDD
#define VK_OEM_7                           0xDE
#define VK_OEM_8                           0xDF
//      RESERVED                           0xE0
#define VK_OEM_AX                          0xE1
#define VK_OEM_102                         0xE2
#define VK_ICO_HELP                        0xE3
#define VK_ICO_00                          0xE4
#define VK_PROCESSKEY                      0xE5
#define VK_ICO_CLEAR                       0xE6
#define VK_PACKET                          0xE7
//      UNASSIGNED                         0xE8
#define VK_OEM_RESET                       0xE9
#define VK_OEM_JUMP                        0xEA
#define VK_OEM_PA1                         0xEB
#define VK_OEM_PA2                         0xEC
#define VK_OEM_PA3                         0xED
#define VK_OEM_WSCTRL                      0xEE
#define VK_OEM_CUSEL                       0xEF
#define VK_OEM_ATTN                        0xF0
#define VK_OEM_FINISH                      0xF1
#define VK_OEM_COPY                        0xF2
#define VK_OEM_AUTO                        0xF3
#define VK_OEM_ENLW                        0xF4
#define VK_OEM_BACKTAB                     0xF5
#define VK_ATTN                            0xF6
#define VK_CRSEL                           0xF7
#define VK_EXSEL                           0xF8
#define VK_EREOF                           0xF9
#define VK_PLAY                            0xFA
#define VK_ZOOM                            0xFB
#define VK_NONAME                          0xFC
#define VK_PA1                             0xFD
#define VK_OEM_CLEAR                       0xFE
#define VK__none_                          0xFF
#endif

#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_FULL_ACCELERATION_ARB               0x2027
#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_SAMPLE_BUFFERS_ARB                  0x2041
#define WGL_SAMPLES_ARB                         0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x00000002

typedef vptr win32_handle;
typedef win32_handle win32_instance;
typedef win32_handle win32_icon;
typedef win32_handle win32_cursor;
typedef win32_handle win32_brush;
typedef win32_handle win32_window;
typedef win32_handle win32_menu;
typedef win32_handle win32_device_context;
typedef win32_handle win32_raw_input_handle;
typedef win32_handle win32_region;
typedef win32_handle win32_key;
typedef win32_handle win32_pen;
typedef win32_handle win32_gdi_object;
typedef win32_handle win32_monitor;
typedef win32_handle win32_opengl_render_context;
typedef win32_instance win32_module;
typedef u32 win32_color_ref;

typedef s64 (_API_ENTRY *func_WindowProc)(win32_window Window, u32 Message, u64 WParam, s64 LParam);
typedef b08 (_API_ENTRY *func_EnumCallback)(win32_window Window, s64 LParam);
typedef u32 (_API_ENTRY *func_ThreadStartRoutine)(vptr ThreadParameter);

typedef enum win32_exception_disposition {
    WIN32_EXCEPTION_CONTINUE_EXECUTION,
    WIN32_EXCEPTION_CONTINUE_SEARCH,
    WIN32_EXCEPTION_NESTED_EXCEPTION,
    WIN32_EXCEPTION_COLLIDED_UNWIND
} win32_exception_disposition;

typedef enum win32_ldr_dll_load_reason {
    LoadReasonStaticDependency           = 0,
    LoadReasonStaticForwarderDependency  = 1,
    LoadReasonDynamicForwarderDependency = 2,
    LoadReasonDelayloadDependency        = 3,
    LoadReasonDynamicLoad                = 4,
    LoadReasonAsImageLoad                = 5,
    LoadReasonAsDataLoad                 = 6,
    LoadReasonEnclavePrimary             = 7,
    LoadReasonEnclaveDependency          = 8,
    LoadReasonUnknown                    = -1
} win32_ldr_dll_load_reason;

typedef enum win32_ldr_ddag_state {
    LdrModulesMerged                  = -5,
    LdrModulesInitError               = -4,
    LdrModulesSnapError               = -3,
    LdrModulesUnloaded                = -2,
    LdrModulesUnloading               = -1,
    LdrModulesPlaceHolder             = 0,
    LdrModulesMapping                 = 1,
    LdrModulesMapped                  = 2,
    LdrModulesWaitingForDependencies  = 3,
    LdrModulesSnapping                = 4,
    LdrModulesSnapped                 = 5,
    LdrModulesCondensed               = 6,
    LdrModulesReadyToInit             = 7,
    LdrModulesInitializing            = 8,
    LdrModulesReadyToRun              = 9
} win32_ldr_ddag_state;

typedef struct win32_window_class_a
{
    u32 Style;
    func_WindowProc WindowCallback;
    s32 WindowClassExtraBytes;
    s32 WindowInstanceExtraBytes;
    win32_instance Instance;
    win32_icon Icon;
    win32_cursor Cursor;
    win32_brush Background;
    chr *MenuName;
    chr *ClassName;
} win32_window_class_a;

typedef struct win32_window_class_w
{
    u32 Style;
    func_WindowProc WindowCallback;
    s32 WindowClassExtraBytes;
    s32 WindowInstanceExtraBytes;
    win32_instance Instance;
    win32_icon Icon;
    win32_cursor Cursor;
    win32_brush Background;
    wchr *MenuName;
    wchr *ClassName;
} win32_window_class_w;

typedef struct win32_rect
{
    s32 Left;
    s32 Top;
    s32 Right;
    s32 Bottom;
} win32_rect;

typedef struct win32_point
{
    s32 X;
    s32 Y;
} win32_point;

typedef struct win32_overlapped
{
    u64 Internal;
    u64 InternalHigh;
    union
    {
        struct
        {
            u32 Offset;
            u32 OffsetHigh;
        };
        vptr Pointer;
    };
    win32_handle Event;
} win32_overlapped;

typedef union win32_large_integer
{
    u32 LowPart;
    s32 HighPart;
    struct {
        u32 LowPart;
        s32 HighPart;
    } U;
    s64 QuadPart;
} win32_large_integer;

typedef union win32_ularge_integer
{
    u32 LowPart;
    u32 HighPart;
    struct {
        u32 LowPart;
        u32 HighPart;
    } U;
    u64 QuadPart;
} win32_ularge_integer;

typedef struct win32_security_attributes
{
    u32 Length;
    vptr SecurityDescriptor;
    b08 InheritHandle;
} win32_security_attributes;

typedef struct win32_pixel_format_descriptor
{
    u16 Size;
    u16 Version;
    u32 Flags;
    u08 PixelType;
    u08 ColorBits;
    u08 RedBits;
    u08 RedShift;
    u08 GreenBits;
    u08 GreenShift;
    u08 BlueBits;
    u08 BlueShift;
    u08 AlphaBits;
    u08 AlphaShift;
    u08 AccumBits;
    u08 AccumRedBits;
    u08 AccumGreenBits;
    u08 AccumBlueBits;
    u08 AccumAlphaBits;
    u08 DepthBits;
    u08 StencilBits;
    u08 AuxBuffers;
    u08 LayerType;
    u08 Reserved;
    u32 LayerMask;
    u32 VisibleMask;
    u32 DamageMask;
} win32_pixel_format_descriptor;

typedef struct win32_message
{
    win32_window Window;
    u32 Message;
    u64 WParam;
    s64 LParam;
    u32 Time;
    win32_point Point;
    u32 Private;
} win32_message;

typedef struct win32_filetime
{
    u32 LowDateTime;
    u32 HighDateTime;
} win32_filetime;

typedef struct win32_find_data_a
{
    u32 FileAttributes;
    win32_filetime CreationTime;
    win32_filetime LastAccessTime;
    win32_filetime LastWriteTime;
    u32 FileSizeHigh;
    u32 FileSizeLow;
    u32 Reserved0;
    u32 Reserved1;
    chr FileName[MAX_PATH];
    chr AlternateFileName[14];
} win32_find_data_a;

typedef struct win32_find_data_w
{
    u32 FileAttributes;
    win32_filetime CreationTime;
    win32_filetime LastAccessTime;
    win32_filetime LastWriteTime;
    u32 FileSizeHigh;
    u32 FileSizeLow;
    u32 Reserved0;
    u32 Reserved1;
    wchr FileName[MAX_PATH];
    wchr AlternateFileName[14];
} win32_find_data_w;

typedef struct win32_string
{
    u16 Length;
    u16 MaximumLength;
    u08 _Padding0_[4];
    chr *Buffer;
} win32_string;

typedef struct win32_unicode_string
{
    u16 Length;
    u16 MaximumLength;
    u08 _Padding0_[4];
    wchr *Buffer;
} win32_unicode_string;

typedef struct win32_exception_record
{
    u32 ExceptionCode;
    u32 ExceptionFlags;
    struct win32_exception_record *ExceptionRecord;
    vptr ExceptionAddress;
    u32 NumberParameters;
    u64 ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} win32_exception_record;

typedef struct win32_m128a
{
    u64 Low;
    s64 High;
} win32_m128a;

typedef struct win32_xsave_format
{
    u16 ControlWord;
    u16 StatusWord;
    u08 TagWord;
    u08 Reserved1;
    u16 ErrorOpcode;
    u32 ErrorOffset;
    u16 ErrorSelector;
    u16 Reserved2;
    u32 DataOffset;
    u16 DataSelector;
    u16 Reserved3;
    u32 MxCsr;
    u32 MxCsr_Mask;
    win32_m128a FloatRegisters[8];
    win32_m128a XmmRegisters[16];
    u08 Reserved4[96];
} win32_xsave_format;

typedef struct win32_context
{
    u64 P1Home;
    u64 P2Home;
    u64 P3Home;
    u64 P4Home;
    u64 P5Home;
    u64 P6Home;
    u32 ContextFlags;
    u32 MxCsr;
    u16 SegCs;
    u16 SegDs;
    u16 SegEs;
    u16 SegFs;
    u16 SegGs;
    u16 SegSs;
    u32 EFlags;
    u64 Dr0;
    u64 Dr1;
    u64 Dr2;
    u64 Dr3;
    u64 Dr6;
    u64 Dr7;
    u64 Rax;
    u64 Rcx;
    u64 Rdx;
    u64 Rbx;
    u64 Rsp;
    u64 Rbp;
    u64 Rsi;
    u64 Rdi;
    u64 R8;
    u64 R9;
    u64 R10;
    u64 R11;
    u64 R12;
    u64 R13;
    u64 R14;
    u64 R15;
    u64 Rip;
    union
    {
        win32_xsave_format FltSave;
        struct
        {
            win32_m128a Header[2];
            win32_m128a Legacy[8];
            win32_m128a Xmm0;
            win32_m128a Xmm1;
            win32_m128a Xmm2;
            win32_m128a Xmm3;
            win32_m128a Xmm4;
            win32_m128a Xmm5;
            win32_m128a Xmm6;
            win32_m128a Xmm7;
            win32_m128a Xmm8;
            win32_m128a Xmm9;
            win32_m128a Xmm10;
            win32_m128a Xmm11;
            win32_m128a Xmm12;
            win32_m128a Xmm13;
            win32_m128a Xmm14;
            win32_m128a Xmm15;
        };
    };
    win32_m128a VectorRegister[26];
    u64 VectorControl;
    u64 DebugControl;
    u64 LastBranchToRip;
    u64 LastBranchFromRip;
    u64 LastExceptionToRip;
    u64 LastExceptionFromRip;
} win32_context;

typedef win32_exception_disposition _API_ENTRY
func_Win32ExceptionRoutine(win32_exception_record *ExceptionRecord,
                            vptr EstablisherFrame,
                            win32_context *ContextRecord,
                            vptr DispatcherContext);
typedef func_Win32ExceptionRoutine *Win32ExceptionRoutine;

typedef struct win32_exception_registration_record
{
    struct win32_exception_registration_record *Next;
    Win32ExceptionRoutine Handler;
} win32_exception_registration_record;

typedef struct win32_nt_tib
{
    win32_exception_registration_record *ExceptionList;
    vptr StackBase;
    vptr StackLimit;
    vptr SubSystemTib;
    union {
        vptr FiberData;
        u32 Version;
    };
    vptr ArbitraryUserPointer;
    struct win32_nt_tib *Self;
} win32_nt_tib;

typedef struct win32_client_id
{
    win32_handle UniqueProcess;
    win32_handle UniqueThread;
} win32_client_id;

typedef struct win32_single_list_entry
{
    struct win32_single_list_entry *Next;
} win32_single_list_entry;

typedef struct win32_list_entry
{
    struct win32_list_entry *Flink;
    struct win32_list_entry *Blink;
} win32_list_entry;

typedef struct win32_rtl_activation_context_stack_frame
{
    struct win32_rtl_activation_context_stack_frame *Previous;
    struct win32_activation_context *ActivationContext;
    u32 Flags;
    u08 _Padding0_[4];
} win32_rtl_activation_context_stack_frame;

typedef struct win32_activation_context_stack
{
    win32_rtl_activation_context_stack_frame *ActiveFrame;
    win32_list_entry FrameListCache;
    u32 Flags;
    u32 NextCookieSequenceNumber;
    u32 StackId;
    u08 _Padding0_[4];
} win32_activation_context_stack;

typedef struct win32_gdi_teb_batch
{
    struct
    {
        u32 Offset              : 31;
        u32 HasRenderingCommand :  1;
    };
    u64 HDC;
    u32 Buffer[310];
} win32_gdi_teb_batch;

typedef struct win32_guid
{
    u32 Data1;
    u16 Data2;
    u16 Data3;
    u08 Data4[8];
} win32_guid;

typedef struct win32_processor_number
{
    u16 Group;
    u08 Number;
    u08 Reserved;
} win32_processor_number;

typedef struct win32_teb_active_frame_context
{
    u32 Flags;
    u08 _Padding0_[4];
    chr *FrameName;
} win32_teb_active_frame_context;

typedef struct win32_teb_active_frame
{
    u32 Flags;
    u08 _Padding0_[4];
    struct win32_teb_active_frame *Previous;
    win32_teb_active_frame_context *Context;
} win32_teb_active_frame;

typedef struct win32_curdir
{
    win32_unicode_string DosPath;
    vptr Handle;
} win32_curdir;

typedef struct win32_rtl_drive_letter_curdir
{
    u16 Flags;
    u16 Length;
    u32 TimeStamp;
    win32_string DosPath;
} win32_rtl_drive_letter_curdir;

typedef struct win32_rtl_user_process_parameters
{
    u32 MaximumLength;
    u32 Length;
    u32 Flags;
    u32 DebugFlags;
    vptr ConsoleHandle;
    u32 ConsoleFlags;
    u08 _Padding0_[4];
    vptr StandardInput;
    vptr StandardOutput;
    vptr StandardError;
    win32_curdir CurrentDirectory;
    win32_unicode_string DllPath;
    win32_unicode_string ImagePathName;
    win32_unicode_string CommandLine;
    vptr Environment;
    u32 StartingX;
    u32 StartingY;
    u32 CountX;
    u32 CountY;
    u32 CountCharsX;
    u32 CountCharsY;
    u32 FillAttribute;
    u32 WindowFlags;
    u32 ShowWindowFlags;
    u08 _Padding1_[4];
    win32_unicode_string WindowTitle;
    win32_unicode_string DesktopInfo;
    win32_unicode_string ShellInfo;
    win32_unicode_string RuntimeData;
    win32_rtl_drive_letter_curdir CurrentDirectores[32];
    u64 EnvironmentSize;
    u64 EnvironmentVersion;
    vptr PackageDependencyData;
    u32 ProcessGroupId;
    u32 LoaderThreads;
    win32_unicode_string RedirectionDllName;
    win32_unicode_string HeapPartitionName;
    u64 DefaultThreadpoolCpuSetMasks;
    u32 DefaultThreadpoolCpuSetMaskCount;
    u08 _Padding2_[4];
} win32_rtl_user_process_parameters;

typedef struct win32_rtl_critical_section_debug
{
    u16 Type;
    u16 CreatorBackTraceIndex;
    u08 _Padding0_[4];
    struct win32_rtl_critical_section *CriticalSection;
    win32_list_entry ProcessLocksList;
    u32 EntryCount;
    u32 ContentionCount;
    u32 Flags;
    u16 CreatorBackTraceIndexHigh;
    u16 SpareUSHORT;
} win32_rtl_critical_section_debug;

typedef struct win32_rtl_critical_section
{
    win32_rtl_critical_section_debug *DebugInfo;
    s32 LockCount;
    s32 RecursionCount;
    vptr OwningThread;
    vptr LockSemaphore;
    u64 SpinCount;
} win32_rtl_critical_section;

typedef union win32_slist_header
{
    u64 Alignment;
    u64 Region;
    struct
    {
        u64 Depth     : 16;
        u64 Sequence  : 48;
        u64 Reserved  :  4;
        u64 NextEntry : 60;
    } HeaderX64;
} win32_slist_header;

typedef struct win32_leap_second_data
{
    u08 Enabled;
    u08 _Padding0_[3];
    u32 Count;
    win32_large_integer Data[1];
} win32_leap_second_data;

typedef struct win32_rtl_balanced_node
{
    union
    {
        struct win32_rtl_balanced_node *Children[2];
        struct
        {
            struct win32_rtl_balanced_node *Left;
            struct win32_rtl_balanced_node *Right;
        };
    };
    union
    {
        struct
        {
            u08 Red     : 1;
            u08 Balance : 2;
        };
        u64 ParentValue;
    };
} win32_rtl_balanced_node;

typedef struct win32_ldrp_cslist
{
    win32_single_list_entry *Tail;
} win32_ldrp_cslist;

typedef struct win32_ldr_service_tag_record
{
    struct win32_ldr_service_tag_record *Next;
    u32 ServiceTag;
    u08 _Padding0_[4];
} win32_ldr_service_tag_record;

typedef struct win32_ldr_ddag_node
{
    win32_list_entry Modules;
    win32_ldr_service_tag_record *ServiceTagList;
    u32 LoadCount;
    u32 LoadWhileUnloadingCount;
    u32 LowestLink;
    u08 _Padding0_[4];
    win32_ldrp_cslist Dependencies;
    win32_ldrp_cslist IncomingDependencies;
    win32_ldr_ddag_state State;
    u08 _Padding1_[4];
    win32_single_list_entry CondenseLink;
    u32 PreorderNumber;
    u08 _Padding2_[4];
} win32_ldr_ddag_node;

typedef struct win32_ldr_data_table_entry
{
    win32_list_entry InLoadOrderLinks;
    win32_list_entry InMemoryOrderLinks;
    win32_list_entry InInitializationOrderLinks;
    vptr DllBase;
    vptr EntryPoint;
    u32 SizeOfImage;
    u08 _Padding0_[4];
    win32_unicode_string FullDllName;
    win32_unicode_string BaseDllName;
    union
    {
        u08 FlagGroup[4];
        u32 Flags;
        struct
        {
            u32 PackagedBinary          : 1;
            u32 MarkedForRemoval        : 1;
            u32 ImageDll                : 1;
            u32 LoadNotificationsSent   : 1;
            u32 TelemetryEntryProcessed : 1;
            u32 ProcessStaticImport     : 1;
            u32 InLegacyLists           : 1;
            u32 InIndexes               : 1;
            u32 ShimDll                 : 1;
            u32 InExceptionTable        : 1;
            u32 ReservedFlags1          : 2;
            u32 LoadInProgress          : 1;
            u32 LoadConfigProcessed     : 1;
            u32 EntryProcessed          : 1;
            u32 ProtectDelayLoad        : 1;
            u32 ReservedFlags3          : 2;
            u32 DontCallForThreads      : 1;
            u32 ProcessAttachCalled     : 1;
            u32 ProcessAttachFailed     : 1;
            u32 CorDeferredValidate     : 1;
            u32 CorImage                : 1;
            u32 DontRelocate            : 1;
            u32 CorILOnly               : 1;
            u32 ChpeImage               : 1;
            u32 ReservedFlags5          : 2;
            u32 Redirected              : 1;
            u32 ReservedFlags6          : 2;
            u32 CompatDatabaseProcessed : 1;
        };
    };
    u16 ObsoleteLoadCount;
    u16 TlsIndex;
    win32_list_entry HashLinks;
    u32 TimeDateStamp;
    u08 _Padding1_[4];
    struct win32_activation_context *EntryPointActivationContext;
    vptr Lock;
    win32_ldr_ddag_node *DdagNode;
    win32_list_entry NodeModuleLink;
    struct win32_ldrp_load_context *LoadContext;
    vptr ParentDllBase;
    vptr SwitchBackContext;
    win32_rtl_balanced_node BaseAddressIndexNode;
    win32_rtl_balanced_node MappingInfoIndexNode;
    u64 OriginalBase;
    u08 _Padding2_[4];
    win32_large_integer LoadTime;
    u32 BaseNameHashValue;
    win32_ldr_dll_load_reason LoadReason;
    u32 ImplicitPathOptions;
    u32 ReferenceCount;
    u32 DependentLoadFlags;
    u08 SigningLevel;
    u08 _Padding3_[3];
} win32_ldr_data_table_entry;

typedef struct win32_peb_ldr_data
{
    u32 Length;
    u08 Initialized;
    u08 _Padding0_[3];
    vptr SsHandle;
    win32_list_entry InLoadOrderModuleList;
    win32_list_entry InMemoryOrderModuleList;
    win32_list_entry InInitializationOrderModuleList;
    vptr EntryInProgress;
    u08 ShutdownInProgress;
    u08 _Padding1_[7];
    vptr ShutdownThreadId;
} win32_peb_ldr_data;

typedef struct win32_peb
{
    u08 InheritedAddressSpace;
    u08 ReadImageFileExecOptions;
    u08 BeingDebugged;
    union
    {
        u08 BitField;
        struct
        {
            u08 ImageUsesLargePages          : 1;
            u08 IsProtectedProcess           : 1;
            u08 IsImageDynamicallyRelocated  : 1;
            u08 SkipPatchingUser32Forwarders : 1;
            u08 IsPackagedProcess            : 1;
            u08 IsAppContainer               : 1;
            u08 IsProtectedProcessLight      : 1;
            u08 IsLongPathAwareProcess       : 1;
        };
    };
    u08 _Padding0_[4];
    vptr Mutant;
    vptr ImageBaseAddress;
    win32_peb_ldr_data *Ldr;
    win32_rtl_user_process_parameters *ProcessParameters;
    vptr SubSystemData;
    vptr ProcessHeap;
    win32_rtl_critical_section *FastPebLock;
    win32_slist_header *AtlThunkSListPtr;
    vptr IFEOKey;
    union
    {
        u32 CrossProcessFlags;
        struct
        {
            u32 ProcessInJob               :  1;
            u32 ProcessInitializing        :  1;
            u32 ProcessUsingVEH            :  1;
            u32 ProcessUsingVCH            :  1;
            u32 ProcessUsingFTH            :  1;
            u32 ProcessPreviouslyThrottled :  1;
            u32 ProcessCurrentlyThrottled  :  1;
            u32 ProcessImagesHotPatched    :  1;
            u32 ReservedBits0              : 24;
        };
    };
    u08 _Padding1_[4];
    union
    {
        vptr KernelCallbackTable;
        vptr UserSharedInfoPtr;
    };
    u32 SystemReserved;
    u32 AtlThunkSListPtr32;
    vptr ApiSetMap;
    u32 TlsExpansionCounter;
    u08 _Padding2_[4];
    vptr TlsBitmap;
    u32 TlsBitmapBits[2];
    vptr ReadOnlySharedMemoryBase;
    vptr SharedData;
    vptr *ReadOnlyStaticServerData;
    vptr AnsiCodePageData;
    vptr OemCodePageData;
    vptr UnicodeCaseTableData;
    u32 NumberOfProcessors;
    u32 NtGlobalFlag;
    u08 _Padding3_[4];
    win32_large_integer CriticalSectionTimeout;
    u64 HeapSegmentReserve;
    u64 HeapSegmentCommit;
    u64 HeapDeCommitTotalFreeThreshold;
    u64 HeapDeCommitFreeBlockThreshold;
    u32 NumberOfHeaps;
    u32 MaximumNumberOfHeaps;
    vptr *ProcessHeaps;
    vptr GdiSharedHandleTable;
    vptr ProcessStarterHelper;
    u32 GdiDCAttributeList;
    u08 _Padding4_[4];
    win32_rtl_critical_section *LoaderLock;
    u32 OSMajorVersion;
    u32 OSMinorVersion;
    u16 OSBuildNumber;
    u16 OSCSDVersion;
    u32 OSPlatformId;
    u32 ImageSubsystem;
    u32 ImageSubsystemMajorVersion;
    u32 ImageSubsystemMinorVersion;
    u08 _Padding5_[4];
    u64 ActiveProcessAffinityMask;
    u32 GdiHandleBuffer[60];
    void (_API_ENTRY *PostProcessInitRoutine)(void);
    vptr TlsExpansionBitmap;
    u32 TlsExpansionBitmapBits[32];
    u32 SessionId;
    u08 _Padding6_[4];
    win32_ularge_integer AppCompatFlags;
    win32_ularge_integer AppCompatFlagsUser;
    vptr ShimData;
    vptr AppCompatInfo;
    win32_unicode_string CSDVersion;
    struct win32_activation_context_data *ActivationContextData;
    struct win32_assembly_storage_map *ProcessAssemblyStorageMap;
    struct win32_activation_context_data *SystemDefaultActivationContextData;
    struct win32_assembly_storage_map *SystemAssemblyStorageMap;
    u64 MinimumStackCommit;
    vptr SparePointers[4];
    u32 SpareUlongs[5];
    u08 _Padding7_[4];
    vptr WerRegistrationData;
    vptr WerShipAssertPtr;
    vptr Unused;
    vptr ImageHeaderHash;
    union
    {
        u32 TracingFlags;
        struct
        {
            u32 HeapTracingEnabled      :  1;
            u32 CritSecTracingEnabled   :  1;
            u32 LibLoaderTracingEnabled :  1;
            u32 SpareTracingBits        : 29;
        };
    };
    u08 _Padding8_[4];
    u64 CsrServerReadOnlySharedMemoryBase;
    u64 TppWorkerpListLock;
    win32_list_entry TppWorkerpList;
    vptr WaitOnAddressHashTable[128];
    vptr TelemetryCoverageHeader;
    u32 CloudFileFlags;
    u32 CloudFileDiagFlags;
    chr PlaceholderCompatibilityMode;
    chr PlaceholderCompatibilityModeReserved[7];
    win32_leap_second_data *LeapSecondData;
    union
    {
        u32 LeapSecondFlags;
        struct
        {
            u32 SixtySecondEnabled :  1;
            u32 Reserved           : 31;
        };
    };
    u32 NtGlobalFlag2;
} win32_peb;

typedef struct win32_teb
{
    win32_nt_tib NtTib;
    vptr EnvironmentPointer;
    win32_client_id ClientId;
    vptr ActiveRpcHandle;
    vptr ThreadLocalStoragePointer;
    win32_peb *ProcessEnvironmentBlock;
    u32 LastErrorValue;
    u32 CountOfOwnedCriticalSections;
    vptr CsrClientThread;
    vptr Win32ThreadInfo;
    u32 User32Reserved[26];
    u32 UserReserved[5];
    u08 _Padding0_[4];
    vptr WOW32Reserved;
    u32 CurrentLocale;
    u32 FpSoftwareStatusRegister;
    vptr ReservedForDebuggerInstrumentation[16];
    vptr SystemReserved1[30];
    chr PlaceholderCompatibilityMode;
    u08 PlaceholderHydrationAlwaysExplicit;
    chr PlaceholderReserved[10];
    u32 ProxiedProcessId;
    win32_activation_context_stack _ActivationStack;
    u08 WorkingOnBehalfOfTicket[8];
    s32 ExceptionCode;
    u08 _Padding1_[4];
    win32_activation_context_stack *ActivationContextStackPointer;
    u64 InstrumentationCallbackSp;
    u64 InstrumentationCallbackPreviousPc;
    u64 InstrumentationCallbackPreviousSp;
    u32 TxFsContext;
    u08 InstrumentationCallbackDisabled;
    u08 UnalignedLoadStoreExceptions;
    u08 _Padding2_[2];
    win32_gdi_teb_batch GdiTebBatch;
    win32_client_id RealClientId;
    vptr GdiCachedProcessHandle;
    u32 GdiClientPID;
    u32 GdiClientTID;
    vptr GdiThreadLocalInfo;
    u64 Win32ClientInfo[62];
    vptr GLDispatchTable[223];
    u64 GLReserved1[29];
    vptr GLReserved2;
    vptr GLSectionInfo;
    vptr GLSection;
    vptr GLTable;
    vptr GLCurrentRC;
    vptr GLContext;
    u32 LastStatusValue;
    u08 _Padding3_[4];
    win32_unicode_string StaticUnicodeString;
    wchr StaticUnicodeBuffer[261];
    u08 _Padding4_[6];
    vptr DeallocationStack;
    vptr TlsSlots[64];
    win32_list_entry TlsLinks;
    vptr Vdm;
    vptr ReservedForNtRpc;
    vptr DbgSsReserved[2];
    u32 HardErrorMode;
    u08 _Padding5_[4];
    vptr Instrumentation[11];
    win32_guid ActivityId;
    vptr SubProcessTag;
    vptr PerflibData;
    vptr EtwTraceData;
    vptr WinSockData;
    u32 GdiBatchCount;
    union
    {
        win32_processor_number CurrentIdealProcessor;
        u32 IdealProcessorValue;
        struct
        {
            u08 ReservedPad0;
            u08 ReservedPad1;
            u08 ReservedPad2;
            u08 IdealProcessor;
        };
    };
    u32 GuaranteedStackBytes;
    u08 _Padding6_[4];
    vptr ReservedForPerf;
    vptr ReservedForOle;
    u32 WaitingOnLoaderLock;
    u08 _Padding7_[4];
    vptr SavedPriorityState;
    u64 ReservedForCodeCoverage;
    vptr ThreadPoolData;
    vptr *TlsExpansionSlots;
    vptr DeallocationBStore;
    vptr BStoreLimit;
    u32 MuiGeneration;
    u32 IsImpersonating;
    vptr NlsCache;
    vptr ShimData;
    u32 HeapData;
    u08 _Padding8_[4];
    vptr CurrentTransactionHandle;
    win32_teb_active_frame *ActiveFrame;
    vptr FlsData;
    vptr PreferredLanguages;
    vptr UserPrefLanguages;
    u32 MuiImpersonation;
    union
    {
        u16 CrossTebFlags;
        u16 SpareCrossTebBits;
    };
    union
    {
        u16 SameTebFlags;
        struct
        {
            u16 SafeThunkCall        : 1;
            u16 InDebugPrint         : 1;
            u16 HasFiberData         : 1;
            u16 SkipThreadAttach     : 1;
            u16 WerInShipAssertCode  : 1;
            u16 RanProcessInit       : 1;
            u16 ClonedThread         : 1;
            u16 SuppressDebugMsg     : 1;
            u16 DisableUserStackWalk : 1;
            u16 RtlExceptionAttached : 1;
            u16 InitialThread        : 1;
            u16 SessionAware         : 1;
            u16 LoadOwner            : 1;
            u16 LoaderWorker         : 1;
            u16 SkipLoaderInit       : 1;
            u16 SpareSameTebBits     : 1;
        };
    };
    vptr TxnScopeEnterCallback;
    vptr TxnScopeExitCallback;
    vptr TxnScopeContext;
    u32 LockCount;
    s32 WowTebOffset;
    vptr ResourceRetValue;
    vptr ReservedForWdf;
    u64 ReservedForCrt;
    win32_guid EffectiveContainerId;
} win32_teb;

typedef struct win32_image_dos_header
{
    u16 e_magic;    // Magic number
    u16 e_cblp;     // Bytes on last page of file
    u16 e_cp;       // Pages in file
    u16 e_crlc;     // Relocations
    u16 e_cparhdr;  // Size of header in paragraphs
    u16 e_minalloc; // Minimum extra paragraphs needed
    u16 e_maxalloc; // Maximum extra paragraphs needed
    u16 e_ss;       // Initial (relative) SS value
    u16 e_sp;       // Initial SP value
    u16 e_csum;     // Checksum
    u16 e_ip;       // Initial IP value
    u16 e_cs;       // Initial (relative) CS value
    u16 e_lfarlc;   // File address of relocation table
    u16 e_ovno;     // Overlay number
    u16 e_res[4];   // Reserved words
    u16 e_oemid;    // OEM identifier (for e_oeminfo)
    u16 e_oeminfo;  // OEM information; e_oemid specific
    u16 e_res2[10]; // Reserved words
    s32 e_lfanew;   // File address of new exe header
} win32_image_dos_header;

typedef struct win32_image_file_header
{
    u16 Machine;
    u16 NumberOfSections;
    u32 TimeDateStamp;
    u32 PointerToSymbolTable;
    u32 NumberOfSymbols;
    u16 SizeOfOptionalHeader;
    u16 Characteristics;
} win32_image_file_header;

typedef struct win32_image_data_directory
{
    u32 VirtualAddress;
    u32 Size;
} win32_image_data_directory;

typedef struct win32_image_optional_header
{
    u16 Magic;
    u08 MajorLinkerVersion;
    u08 MinorLinkerVersion;
    u32 SizeOfCode;
    u32 SizeOfInitializedData;
    u32 SizeOfUninitializedData;
    u32 AddressOfEntryPoint;
    u32 BaseOfCode;
    u64 ImageBase;
    u32 SectionAlignment;
    u32 FileAlignment;
    u16 MajorOperatingSystemVersion;
    u16 MinorOperatingSystemVersion;
    u16 MajorImageVersion;
    u16 MinorImageVersion;
    u16 MajorSubsystemVersion;
    u16 MinorSubsystemVersion;
    u32 Win32VersionValue;
    u32 SizeOfImage;
    u32 SizeOfHeaders;
    u32 CheckSum;
    u16 Subsystem;
    u16 DllCharacteristics;
    u64 SizeOfStackReserve;
    u64 SizeOfStackCommit;
    u64 SizeOfHeapReserve;
    u64 SizeOfHeapCommit;
    u32 LoaderFlags;
    u32 NumberOfRvaAndSizes;
    win32_image_data_directory ExportTable;
    win32_image_data_directory ImportTable;
    win32_image_data_directory ResourceTable;
    win32_image_data_directory ExceptionTable;
    win32_image_data_directory CertificateTable;
    win32_image_data_directory BaseRelocationTable;
    win32_image_data_directory Debug;
    win32_image_data_directory Architecture;
    win32_image_data_directory GlobalPtr;
    win32_image_data_directory TLSTable;
    win32_image_data_directory LoadConfigTable;
    win32_image_data_directory BoundImport;
    win32_image_data_directory IAT;
    win32_image_data_directory DelayImportDescriptor;
    win32_image_data_directory CLRRuntimeHeader;
    win32_image_data_directory ReservedTable;
} win32_image_optional_header;

typedef struct win32_image_nt_headers
{
    u32 Signature;
    win32_image_file_header FileHeader;
    win32_image_optional_header OptionalHeader;
} win32_image_nt_headers;

typedef struct win32_image_section_header
{
    u08 Name[8];
    union
    {
        u32 PhysicalAddress;
        u32 VirtualSize;
    } Misc;
    u32 VirtualAddress;
    u32 SizeOfRawData;
    u32 PointerToRawData;
    u32 PointerToRelocations;
    u32 PointerToLinenumbers;
    u16 NumberOfRelocations;
    u16 NumberOfLinenumbers;
    u32 Characteristics;
} win32_image_section_header;

typedef struct win32_image_export_directory
{
    u32 Characteristics;
    u32 TimeDateStamp;
    u16 MajorVersion;
    u16 MinorVersion;
    u32 Name;
    u32 Base;
    u32 NumberOfFunctions;
    u32 NumberOfNames;
    u32 AddressOfFunctions;
    u32 AddressOfNames;
    u32 AddressOfNameOrdinals;
} win32_image_export_directory;

typedef struct win32_display_device_w
{
    u32 CountBytes;
    wchr DeviceName[32];
    wchr DeviceString[128];
    u32 StateFlags;
    wchr DeviceID[128];
    wchr DeviceKey[128];
} win32_display_device_w;

typedef struct win32_raw_input_device
{
    u16 UsagePage;
    u16 Usage;
    u32 Flags;
    win32_window Target;
} win32_raw_input_device;

typedef struct win32_track_mouse_event
{
    u32 Size;
    u32 Flags;
    win32_window WindowTrack;
    u32 HoverTime;
} win32_track_mouse_event;

typedef struct win32_raw_input_header
{
    u32 Type;
    u32 Size;
    win32_handle Device;
    u64 WParam;
} win32_raw_input_header;

typedef struct win32_raw_mouse
{
    u16 Flags;
    union
    {
        u32 Buttons;
        struct
        {
            u16 ButtonFlags;
            u16 ButtonData;
        };
    };
    u32 RawButtons;
    s32 LastX;
    s32 LastY;
    u32 ExtraInformation;
} win32_raw_mouse;

typedef struct win32_raw_keyboard
{
    u16 MakeCode;
    u16 Flags;
    u16 Reserved;
    u16 VKey;
    u32 Message;
    u32 ExtraInformation;
} win32_raw_keyboard;

typedef struct win32_raw_hid
{
    u32 SizeHID;
    u32 Count;
    u08 RawData[1];
} win32_raw_hid;

typedef struct win32_raw_input
{
    win32_raw_input_header Header;
    union
    {
        win32_raw_mouse Mouse;
        win32_raw_keyboard Keyboard;
        win32_raw_hid HID;
    } Data;
} win32_raw_input;

typedef struct win32_min_max_info
{
    win32_point Reserved;
    win32_point MaxSize;
    win32_point MaxPosition;
    win32_point MinTrackSize;
    win32_point MaxTrackSize;
} win32_min_max_info;

typedef struct win32_window_placement
{
    u32 Length;
    u32 Flags;
    u32 ShowCmd;
    win32_point MinPosition;
    win32_point MaxPosition;
    win32_rect NormalPosition;
} win32_window_placement;

typedef struct win32_window_info
{
    u32 Size;
    win32_rect WindowRect;
    win32_rect ClientRect;
    u32 Style;
    u32 ExStyle;
    u32 WindowStatus;
    u32 WindowBordersX;
    u32 WindowBordersY;
    u16 WindowTypeAtom;
    u16 CreatorVersion;
} win32_window_info;

typedef struct win32_log_brush
{
    u32 Style;
    win32_color_ref Color;
    u64 Hatch;
} win32_log_brush;

typedef struct win32_monitor_info
{
    u32 Size;
    win32_rect MonitorRect;
    win32_rect WorkRect;
    u32 Flags;
} win32_monitor_info;

#define WIN32__PROCS \
    PROC(Gdi32,    b08,                         DeleteObject,               Win32_DeleteObject,               win32_gdi_object Object) \
    PROC(Gdi32,    b08,                         Rectangle,                  Win32_Rectangle,                  win32_device_context DeviceContext, s32 Left, s32 Top, s32 Right, s32 Bottom) \
    PROC(Gdi32,    b08,                         SetPixelFormat,             Win32_SetPixelFormat,             win32_device_context DeviceContext, s32 Format, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    PROC(Gdi32,    b08,                         SwapBuffers,                Win32_SwapBuffers,                win32_device_context DeviceContext) \
    PROC(Gdi32,    s32,                         ChoosePixelFormat,          Win32_ChoosePixelFormat,          win32_device_context DeviceContext, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    PROC(Gdi32,    s32,                         DescribePixelFormat,        Win32_DescribePixelFormat,        win32_device_context DeviceContext, s32 PixelFormat, u32 BytesCount, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    PROC(Gdi32,    win32_brush,                 CreateBrushIndirect,        Win32_CreateBrushIndirect,        win32_log_brush *LogBrush) \
    PROC(Gdi32,    win32_gdi_object,            SelectObject,               Win32_SelectObject,               win32_device_context DeviceContext, win32_gdi_object Object) \
    PROC(Gdi32,    win32_pen,                   CreatePen,                  Win32_CreatePen,                  s32 Style, s32 Width, win32_color_ref Color) \
    PROC(Kernel32, b08,                         CloseHandle,                Win32_CloseHandle,                win32_handle Object) \
    PROC(Kernel32, b08,                         CopyFileA,                  Win32_CopyFileA,                  chr *ExistingFileName, chr *NewFileName, b08 FailIfExists) \
    PROC(Kernel32, b08,                         CopyFileW,                  Win32_CopyFileW,                  wchr *ExistingFileName, wchr *NewFileName, b08 FailIfExists) \
    PROC(Kernel32, b08,                         CreateDirectoryA,           Win32_CreateDirectoryA,           chr *PathName, win32_security_attributes *SecurityAttributes) \
    PROC(Kernel32, b08,                         FindClose,                  Win32_FindClose,                  win32_handle FindFile) \
    PROC(Kernel32, b08,                         FindNextFileA,              Win32_FindNextFileA,              win32_handle FindFile, win32_find_data_a *FindFileData) \
    PROC(Kernel32, b08,                         FindNextFileW,              Win32_FindNextFileW,              win32_handle FindFile, win32_find_data_w *FindFileData) \
    PROC(Kernel32, b08,                         FreeLibrary,                Win32_FreeLibrary,                win32_module LibraryModule) \
    PROC(Kernel32, b08,                         GetFileSizeEx,              Win32_GetFileSizeEx,              win32_handle File, win32_large_integer *FileSize) \
    PROC(Kernel32, b08,                         QueryPerformanceCounter,    Win32_QueryPerformanceCounter,    win32_large_integer *PerformanceCount) \
    PROC(Kernel32, b08,                         ReadFile,                   Win32_ReadFile,                   win32_handle File, vptr Buffer, u32 NumberOfBytesToRead, u32 *NumberOfBytesRead, win32_overlapped *OVerlapped) \
    PROC(Kernel32, b08,                         ReleaseSemaphore,           Win32_ReleaseSemaphore,           win32_handle Semaphore, s32 ReleaseCount, s32 *PreviousCount) \
    PROC(Kernel32, b08,                         VirtualFree,                Win32_VirtualFree,                vptr Address, u64 Size, u32 FreeType) \
    PROC(Kernel32, b08,                         WriteFile,                  Win32_WriteFile,                  win32_handle File, vptr Buffer, u32 NumberOfBytesToWrite, u32 *NumberOfBytesWritten, win32_overlapped *Overlapped) \
    PROC(Kernel32, s32,                         CompareFileTime,            Win32_CompareFileTime,            win32_filetime *FileTime1, win32_filetime *FileTime2) \
    PROC(Kernel32, s32,                         WideCharToMultiByte,        Win32_WideCharToMultiByte,        u32 CodePage, u32 Flags, wchr *WideCharStr, s32 WideCharCount, chr *MultiByteStr, s32 MultiByteCount, chr *DefualtChar, b08 *UsedDefaultChar) \
    PROC(Kernel32, u32,                         GetLastError,               Win32_GetLastError,               void) \
    PROC(Kernel32, u32,                         WaitForSingleObjectEx,      Win32_WaitForSingleObjectEx,      win32_handle Handle, u32 Milliseconds, b08 Alertable) \
    PROC(Kernel32, void,                        ExitProcess,                Win32_ExitProcess,                u32 ExitCode) \
    PROC(Kernel32, void,                        OutputDebugStringA,         Win32_OutputDebugStringA,         chr *OutputString) \
    PROC(Kernel32, vptr,                        HeapAlloc,                  Win32_HeapAlloc,                  win32_handle Heap, u32 Flags, u64 Bytes) \
    PROC(Kernel32, vptr,                        TlsGetValue,                Win32_TlsGetValue,                u32 TlsIndex) \
    PROC(Kernel32, vptr,                        VirtualAlloc,               Win32_VirtualAlloc,               vptr Address, u64 Size, u32 AllocationType, u32 Protect) \
    PROC(Kernel32, win32_handle,                CreateFileA,                Win32_CreateFileA,                chr *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, win32_handle TemplateFile) \
    PROC(Kernel32, win32_handle,                CreateFileW,                Win32_CreateFileW,                wchr *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, win32_handle TemplateFile) \
    PROC(Kernel32, win32_handle,                CreateSemaphoreExW,         Win32_CreateSemaphoreExW,         win32_security_attributes *SemaphoreAttributes, s32 InitialCount, s32 MaximumCount, wchr *Name, u32 Flags, u32 DesiredAccess) \
    PROC(Kernel32, win32_handle,                CreateThread,               Win32_CreateThread,               win32_security_attributes *ThreadAttributes, u64 StackSize, func_ThreadStartRoutine StartAddress, vptr Parameter, u32 CreationFlags, u32 *ThreadId) \
    PROC(Kernel32, win32_handle,                FindFirstFileA,             Win32_FindFirstFileA,             chr *FileName, win32_find_data_a *FindFileData) \
    PROC(Kernel32, win32_handle,                FindFirstFileW,             Win32_FindFirstFileW,             wchr *FileName, win32_find_data_w *FindFileData) \
    PROC(Kernel32, win32_handle,                GetProcessHeap,             Win32_GetProcessHeap,             void) \
    PROC(Kernel32, win32_module,                GetModuleHandleA,           Win32_GetModuleHandleA,           chr *ModuleName) \
    PROC(Kernel32, win32_module,                GetModuleHandleW,           Win32_GetModuleHandleW,           wchr *ModuleName) \
    PROC(Kernel32, win32_module,                LoadLibraryA,               Win32_LoadLibraryA,               chr *LibraryFileName) \
    PROC(Kernel32, win32_module,                LoadLibraryW,               Win32_LoadLibraryW,               wchr *LibraryFileName) \
    PROC(Kernel32, fptr,                        GetProcAddress,             Win32_GetProcAddress,             win32_module Module, chr *ProcName) \
    PROC(Opengl32, b08,                         wglDeleteContext,           WGL_DeleteContext,                win32_opengl_render_context RenderContext) \
    PROC(Opengl32, b08,                         wglMakeCurrent,             WGL_MakeCurrent,                  win32_device_context DeviceContext, win32_opengl_render_context RenderContext) \
    PROC(Opengl32, win32_opengl_render_context, wglCreateContext,           WGL_CreateContext,                win32_device_context DeviceContext) \
    PROC(Opengl32, fptr,                        wglGetProcAddress,          WGL_GetProcAddress,               chr *Process) \
    PROC(User32,   b08,                         AdjustWindowRectEx,         Win32_AdjustWindowRectEx,         win32_rect *Rect, u32 Style, b08 Menu, u32 ExStyle) \
    PROC(User32,   b08,                         BringWindowToTop,           Win32_BringWindowToTop,           win32_window Window) \
    PROC(User32,   b08,                         ClientToScreen,             Win32_ClientToScreen,             win32_window Window, win32_point *Point) \
    PROC(User32,   b08,                         ClipCursor,                 Win32_ClipCursor,                 win32_rect *Rect) \
    PROC(User32,   b08,                         DestroyWindow,              Win32_DestroyWindow,              win32_window Window) \
    PROC(User32,   b08,                         EnumWindows,                Win32_EnumWindows,                func_EnumCallback EnumCallback, s64 LParam) \
    PROC(User32,   b08,                         GetClientRect,              Win32_GetClientRect,              win32_window Window, win32_rect *Rect) \
    PROC(User32,   b08,                         GetCursorPos,               Win32_GetCursorPos,               win32_point *Point) \
    PROC(User32,   b08,                         GetMonitorInfoW,            Win32_GetMonitorInfoW,            win32_monitor Monitor, win32_monitor_info *MonitorInfo) \
    PROC(User32,   b08,                         GetWindowInfo,              Win32_GetWindowInfo,              win32_window Window, win32_window_info *WindowInfo) \
    PROC(User32,   b08,                         GetWindowPlacement,         Win32_GetWindowPlacement,         win32_window Window, win32_window_placement *WindowPlacement) \
    PROC(User32,   b08,                         GetWindowRect,              Win32_GetWindowRect,              win32_window Window, win32_rect *Rect) \
    PROC(User32,   b08,                         IsIconic,                   Win32_IsIconic,                   win32_window Window) \
    PROC(User32,   b08,                         IsWindowVisible,            Win32_IsWindowVisible,            win32_window Window) \
    PROC(User32,   b08,                         IsZoomed,                   Win32_IsZoomed,                   win32_window Window) \
    PROC(User32,   b08,                         OffsetRect,                 Win32_OffsetRect,                 win32_rect *Rect, s32 DX, s32 DY) \
    PROC(User32,   b08,                         PeekMessageA,               Win32_PeekMessageA,               win32_message *Message, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    PROC(User32,   b08,                         PeekMessageW,               Win32_PeekMessageW,               win32_message *Message, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    PROC(User32,   b08,                         RedrawWindow,               Win32_RedrawWindow,               win32_window Window, win32_rect *RectUpdate, win32_region RegionUpdate, u32 Flags) \
    PROC(User32,   b08,                         RegisterRawInputDevices,    Win32_RegisterRawInputDevices,    win32_raw_input_device *RawInputDevices, u32 NumDevices, u32 Size) \
    PROC(User32,   b08,                         ReleaseCapture,             Win32_ReleaseCapture,             void) \
    PROC(User32,   b08,                         ScreenToClient,             Win32_ScreenToClient,             win32_window Window, win32_point *Point) \
    PROC(User32,   b08,                         SetCursorPos,               Win32_SetCursorPos,               s32 X, s32 Y) \
    PROC(User32,   b08,                         SetForegroundWindow,        Win32_SetForegroundWindow,        win32_window Window) \
    PROC(User32,   b08,                         SetLayeredWindowAttributes, Win32_SetLayeredWindowAttributes, win32_window Window, win32_color_ref ColorRefKey, u08 Alpha, u32 Flags) \
    PROC(User32,   b08,                         SetPropA,                   Win32_SetPropA,                   win32_window Window, chr *String, win32_handle Data) \
    PROC(User32,   b08,                         SetPropW,                   Win32_SetPropW,                   win32_window Window, wchr *String, win32_handle Data) \
    PROC(User32,   b08,                         SetWindowPlacement,         Win32_SetWindowPlacement,         win32_window Window, win32_window_placement *WindowPlacement) \
    PROC(User32,   b08,                         SetWindowPos,               Win32_SetWindowPos,               win32_window Window, win32_window WindowInsertAfter, s32 X, s32 Y, s32 CX, s32 CY, u32 Flags) \
    PROC(User32,   b08,                         ShowWindow,                 Win32_ShowWindow,                 win32_window Window, s32 CmdShow) \
    PROC(User32,   b08,                         SystemParametersInfoW,      Win32_SystemParametersInfoW,      u32 Action, u32 UParam, vptr VParam, u32 WinIni) \
    PROC(User32,   b08,                         TrackMouseEvent,            Win32_TrackMouseEvent,            win32_track_mouse_event *EventTrack) \
    PROC(User32,   b08,                         TranslateMessage,           Win32_TranslateMessage,           win32_message *Message) \
    PROC(User32,   b08,                         WaitMessage,                Win32_WaitMessage,                void) \
    PROC(User32,   s16,                         GetKeyState,                Win32_GetKeyState,                s32 VirtualKey) \
    PROC(User32,   s32,                         GetSystemMetrics,           Win32_GetSystemMetrics,           s32 Index) \
    PROC(User32,   s32,                         GetWindowLongW,             Win32_GetWindowLongW,             win32_window Window, s32 Index) \
    PROC(User32,   s32,                         ReleaseDC,                  Win32_ReleaseDC,                  win32_window Window, win32_device_context DeviceContext) \
    PROC(User32,   s32,                         ShowCursor,                 Win32_ShowCursor,                 b08 Show) \
    PROC(User32,   s32,                         ToUnicode,                  Win32_ToUnicode,                  u32 VirtualKey, u32 Scancode, u08 *KeyState, wchr *Buffer, s32 BufferSize, u32 Flags) \
    PROC(User32,   s32,                         MapWindowPoints,            Win32_MapWindowPoints,            win32_window From, win32_window To, win32_point *Points, u32 PointsCount) \
    PROC(User32,   s64,                         DefWindowProcA,             Win32_DefWindowProcA,             win32_window Window, u32 Message, u64 WParam, s64 LParam) \
    PROC(User32,   s64,                         DefWindowProcW,             Win32_DefWindowProcW,             win32_window Window, u32 Message, u64 WParam, s64 LParam) \
    PROC(User32,   s64,                         DispatchMessageA,           Win32_DispatchMessageA,           win32_message *Message) \
    PROC(User32,   s64,                         DispatchMessageW,           Win32_DispatchMessageW,           win32_message *Message) \
    PROC(User32,   s64,                         GetMessageTime,             Win32_GetMessageTime,             void) \
    PROC(User32,   s64,                         SendMessageW,               Win32_SendMessageW,               win32_window Window, u32 Message, u64 WParam, s64 LParam) \
    PROC(User32,   u16,                         RegisterClassA,             Win32_RegisterClassA,             win32_window_class_a *WindowClass) \
    PROC(User32,   u16,                         RegisterClassW,             Win32_RegisterClassW,             win32_window_class_w *WindowClass) \
    PROC(User32,   u32,                         GetRawInputData,            Win32_GetRawInputData,            win32_raw_input_handle RawInput, u32 Command, vptr Data, u32 *Size, u32 SizeHeader) \
    PROC(User32,   u32,                         GetSysColor,                Win32_GetSysColor,                s32 Index) \
    PROC(User32,   u32,                         GetWindowThreadProcessId,   Win32_GetWindowThreadProcessId,   win32_window Window, u32 *ProcessId) \
    PROC(User32,   u32,                         MapVirtualKeyW,             Win32_MapVirtualKeyW,             u32 Code, u32 MapType) \
    PROC(User32,   win32_brush,                 GetSysColorBrush,           Win32_GetSysColorBrush,           s32 Index) \
    PROC(User32,   win32_cursor,                LoadCursorA,                Win32_LoadCursorA,                win32_instance Instance, chr *CursorName) \
    PROC(User32,   win32_cursor,                LoadCursorW,                Win32_LoadCursorW,                win32_instance Instance, wchr *CursorName) \
    PROC(User32,   win32_cursor,                SetCursor,                  Win32_SetCursor,                  win32_cursor Cursor) \
    PROC(User32,   win32_device_context,        GetDC,                      Win32_GetDC,                      win32_window Window) \
    PROC(User32,   win32_handle,                GetPropA,                   Win32_GetPropA,                   win32_window Window, chr *String) \
    PROC(User32,   win32_handle,                GetPropW,                   Win32_GetPropW,                   win32_window Window, wchr *String) \
    PROC(User32,   win32_menu,                  GetMenu,                    Win32_GetMenu,                    win32_window Window) \
    PROC(User32,   win32_monitor,               MonitorFromPoint,           Win32_MonitorFromPoint,           win32_point Point, u32 Flags) \
    PROC(User32,   win32_window,                CreateWindowExA,            Win32_CreateWindowExA,            u32 ExtendedStyle, chr *ClassName, chr *WindowName, u32 Style, s32 X, s32 Y, s32 Width, s32 Height, win32_window Parent, win32_menu Menu, win32_instance Instance, vptr Param) \
    PROC(User32,   win32_window,                CreateWindowExW,            Win32_CreateWindowExW,            u32 ExtendedStyle, wchr *ClassName, wchr *WindowName, u32 Style, s32 X, s32 Y, s32 Width, s32 Height, win32_window Parent, win32_menu Menu, win32_instance Instance, vptr Param) \
    PROC(User32,   win32_window,                GetActiveWindow,            Win32_GetActiveWindow,            void) \
    PROC(User32,   win32_window,                GetCapture,                 Win32_GetCapture,                 void) \
    PROC(User32,   win32_window,                GetDesktopWindow,           Win32_GetDesktopWindow,           void) \
    PROC(User32,   win32_window,                GetForegroundWindow,        Win32_GetForegroundWindow,        void) \
    PROC(User32,   win32_window,                GetParent,                  Win32_GetParent,                  win32_window Window) \
    PROC(User32,   win32_window,                GetShellWindow,             Win32_GetShellWindow,             void) \
    PROC(User32,   win32_window,                SetCapture,                 Win32_SetCapture,                 win32_window Window) \
    PROC(User32,   win32_window,                SetFocus,                   Win32_SetFocus,                   win32_window Window) \

#define WIN32_FUNCS \
    IMPORT(Gdi32,    b08,                         DeleteObject,               Win32_DeleteObject,               win32_gdi_object Object) \
    IMPORT(Gdi32,    b08,                         Rectangle,                  Win32_Rectangle,                  win32_device_context DeviceContext, s32 Left, s32 Top, s32 Right, s32 Bottom) \
    IMPORT(Gdi32,    b08,                         SetPixelFormat,             Win32_SetPixelFormat,             win32_device_context DeviceContext, s32 Format, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    IMPORT(Gdi32,    b08,                         SwapBuffers,                Win32_SwapBuffers,                win32_device_context DeviceContext) \
    IMPORT(Gdi32,    s32,                         ChoosePixelFormat,          Win32_ChoosePixelFormat,          win32_device_context DeviceContext, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    IMPORT(Gdi32,    s32,                         DescribePixelFormat,        Win32_DescribePixelFormat,        win32_device_context DeviceContext, s32 PixelFormat, u32 BytesCount, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    IMPORT(Gdi32,    win32_brush,                 CreateBrushIndirect,        Win32_CreateBrushIndirect,        win32_log_brush *LogBrush) \
    IMPORT(Gdi32,    win32_gdi_object,            SelectObject,               Win32_SelectObject,               win32_device_context DeviceContext, win32_gdi_object Object) \
    IMPORT(Gdi32,    win32_pen,                   CreatePen,                  Win32_CreatePen,                  s32 Style, s32 Width, win32_color_ref Color) \
    IMPORT(Kernel32, b08,                         CloseHandle,                Win32_CloseHandle,                win32_handle Object) \
    IMPORT(Kernel32, b08,                         CopyFileA,                  Win32_CopyFileA,                  chr *ExistingFileName, chr *NewFileName, b08 FailIfExists) \
    IMPORT(Kernel32, b08,                         CopyFileW,                  Win32_CopyFileW,                  wchr *ExistingFileName, wchr *NewFileName, b08 FailIfExists) \
    IMPORT(Kernel32, b08,                         CreateDirectoryA,           Win32_CreateDirectoryA,           chr *PathName, win32_security_attributes *SecurityAttributes) \
    IMPORT(Kernel32, b08,                         FindClose,                  Win32_FindClose,                  win32_handle FindFile) \
    IMPORT(Kernel32, b08,                         FindNextFileA,              Win32_FindNextFileA,              win32_handle FindFile, win32_find_data_a *FindFileData) \
    IMPORT(Kernel32, b08,                         FindNextFileW,              Win32_FindNextFileW,              win32_handle FindFile, win32_find_data_w *FindFileData) \
    IMPORT(Kernel32, b08,                         FreeLibrary,                Win32_FreeLibrary,                win32_module LibraryModule) \
    IMPORT(Kernel32, b08,                         GetFileSizeEx,              Win32_GetFileSizeEx,              win32_handle File, win32_large_integer *FileSize) \
    IMPORT(Kernel32, b08,                         QueryPerformanceCounter,    Win32_QueryPerformanceCounter,    win32_large_integer *PerformanceCount) \
    IMPORT(Kernel32, b08,                         ReadFile,                   Win32_ReadFile,                   win32_handle File, vptr Buffer, u32 NumberOfBytesToRead, u32 *NumberOfBytesRead, win32_overlapped *OVerlapped) \
    IMPORT(Kernel32, b08,                         ReleaseSemaphore,           Win32_ReleaseSemaphore,           win32_handle Semaphore, s32 ReleaseCount, s32 *PreviousCount) \
    IMPORT(Kernel32, b08,                         VirtualFree,                Win32_VirtualFree,                vptr Address, u64 Size, u32 FreeType) \
    IMPORT(Kernel32, b08,                         WriteFile,                  Win32_WriteFile,                  win32_handle File, vptr Buffer, u32 NumberOfBytesToWrite, u32 *NumberOfBytesWritten, win32_overlapped *Overlapped) \
    IMPORT(Kernel32, s32,                         CompareFileTime,            Win32_CompareFileTime,            win32_filetime *FileTime1, win32_filetime *FileTime2) \
    IMPORT(Kernel32, s32,                         WideCharToMultiByte,        Win32_WideCharToMultiByte,        u32 CodePage, u32 Flags, wchr *WideCharStr, s32 WideCharCount, chr *MultiByteStr, s32 MultiByteCount, chr *DefualtChar, b08 *UsedDefaultChar) \
    IMPORT(Kernel32, u32,                         GetLastError,               Win32_GetLastError,               void) \
    IMPORT(Kernel32, u32,                         WaitForSingleObjectEx,      Win32_WaitForSingleObjectEx,      win32_handle Handle, u32 Milliseconds, b08 Alertable) \
    IMPORT(Kernel32, void,                        ExitProcess,                Win32_ExitProcess,                u32 ExitCode) \
    IMPORT(Kernel32, void,                        OutputDebugStringA,         Win32_OutputDebugStringA,         chr *OutputString) \
    IMPORT(Kernel32, vptr,                        HeapAlloc,                  Win32_HeapAlloc,                  win32_handle Heap, u32 Flags, u64 Bytes) \
    IMPORT(Kernel32, vptr,                        TlsGetValue,                Win32_TlsGetValue,                u32 TlsIndex) \
    IMPORT(Kernel32, vptr,                        VirtualAlloc,               Win32_VirtualAlloc,               vptr Address, u64 Size, u32 AllocationType, u32 Protect) \
    IMPORT(Kernel32, win32_handle,                CreateFileA,                Win32_CreateFileA,                chr *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, win32_handle TemplateFile) \
    IMPORT(Kernel32, win32_handle,                CreateFileW,                Win32_CreateFileW,                wchr *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, win32_handle TemplateFile) \
    IMPORT(Kernel32, win32_handle,                CreateSemaphoreExW,         Win32_CreateSemaphoreExW,         win32_security_attributes *SemaphoreAttributes, s32 InitialCount, s32 MaximumCount, wchr *Name, u32 Flags, u32 DesiredAccess) \
    IMPORT(Kernel32, win32_handle,                CreateThread,               Win32_CreateThread,               win32_security_attributes *ThreadAttributes, u64 StackSize, func_ThreadStartRoutine StartAddress, vptr Parameter, u32 CreationFlags, u32 *ThreadId) \
    IMPORT(Kernel32, win32_handle,                FindFirstFileA,             Win32_FindFirstFileA,             chr *FileName, win32_find_data_a *FindFileData) \
    IMPORT(Kernel32, win32_handle,                FindFirstFileW,             Win32_FindFirstFileW,             wchr *FileName, win32_find_data_w *FindFileData) \
    IMPORT(Kernel32, win32_handle,                GetProcessHeap,             Win32_GetProcessHeap,             void) \
    IMPORT(Kernel32, win32_module,                GetModuleHandleA,           Win32_GetModuleHandleA,           chr *ModuleName) \
    IMPORT(Kernel32, win32_module,                GetModuleHandleW,           Win32_GetModuleHandleW,           wchr *ModuleName) \
    IMPORT(Kernel32, win32_module,                LoadLibraryA,               Win32_LoadLibraryA,               chr *LibraryFileName) \
    IMPORT(Kernel32, win32_module,                LoadLibraryW,               Win32_LoadLibraryW,               wchr *LibraryFileName) \
    IMPORT(Kernel32, fptr,                        GetProcAddress,             Win32_GetProcAddress,             win32_module Module, chr *ProcName) \
    IMPORT(Opengl32, b08,                         wglDeleteContext,           WGL_DeleteContext,                win32_opengl_render_context RenderContext) \
    IMPORT(Opengl32, b08,                         wglMakeCurrent,             WGL_MakeCurrent,                  win32_device_context DeviceContext, win32_opengl_render_context RenderContext) \
    IMPORT(Opengl32, win32_opengl_render_context, wglCreateContext,           WGL_CreateContext,                win32_device_context DeviceContext) \
    IMPORT(Opengl32, fptr,                        wglGetProcAddress,          WGL_GetProcAddress,               chr *Process) \
    IMPORT(User32,   b08,                         AdjustWindowRectEx,         Win32_AdjustWindowRectEx,         win32_rect *Rect, u32 Style, b08 Menu, u32 ExStyle) \
    IMPORT(User32,   b08,                         BringWindowToTop,           Win32_BringWindowToTop,           win32_window Window) \
    IMPORT(User32,   b08,                         ClientToScreen,             Win32_ClientToScreen,             win32_window Window, win32_point *Point) \
    IMPORT(User32,   b08,                         ClipCursor,                 Win32_ClipCursor,                 win32_rect *Rect) \
    IMPORT(User32,   b08,                         DestroyWindow,              Win32_DestroyWindow,              win32_window Window) \
    IMPORT(User32,   b08,                         EnumWindows,                Win32_EnumWindows,                func_EnumCallback EnumCallback, s64 LParam) \
    IMPORT(User32,   b08,                         GetClientRect,              Win32_GetClientRect,              win32_window Window, win32_rect *Rect) \
    IMPORT(User32,   b08,                         GetCursorPos,               Win32_GetCursorPos,               win32_point *Point) \
    IMPORT(User32,   b08,                         GetMonitorInfoW,            Win32_GetMonitorInfoW,            win32_monitor Monitor, win32_monitor_info *MonitorInfo) \
    IMPORT(User32,   b08,                         GetWindowInfo,              Win32_GetWindowInfo,              win32_window Window, win32_window_info *WindowInfo) \
    IMPORT(User32,   b08,                         GetWindowPlacement,         Win32_GetWindowPlacement,         win32_window Window, win32_window_placement *WindowPlacement) \
    IMPORT(User32,   b08,                         GetWindowRect,              Win32_GetWindowRect,              win32_window Window, win32_rect *Rect) \
    IMPORT(User32,   b08,                         IsIconic,                   Win32_IsIconic,                   win32_window Window) \
    IMPORT(User32,   b08,                         IsWindowVisible,            Win32_IsWindowVisible,            win32_window Window) \
    IMPORT(User32,   b08,                         IsZoomed,                   Win32_IsZoomed,                   win32_window Window) \
    IMPORT(User32,   b08,                         OffsetRect,                 Win32_OffsetRect,                 win32_rect *Rect, s32 DX, s32 DY) \
    IMPORT(User32,   b08,                         PeekMessageA,               Win32_PeekMessageA,               win32_message *Message, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    IMPORT(User32,   b08,                         PeekMessageW,               Win32_PeekMessageW,               win32_message *Message, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    IMPORT(User32,   b08,                         RedrawWindow,               Win32_RedrawWindow,               win32_window Window, win32_rect *RectUpdate, win32_region RegionUpdate, u32 Flags) \
    IMPORT(User32,   b08,                         RegisterRawInputDevices,    Win32_RegisterRawInputDevices,    win32_raw_input_device *RawInputDevices, u32 NumDevices, u32 Size) \
    IMPORT(User32,   b08,                         ReleaseCapture,             Win32_ReleaseCapture,             void) \
    IMPORT(User32,   b08,                         ScreenToClient,             Win32_ScreenToClient,             win32_window Window, win32_point *Point) \
    IMPORT(User32,   b08,                         SetCursorPos,               Win32_SetCursorPos,               s32 X, s32 Y) \
    IMPORT(User32,   b08,                         SetForegroundWindow,        Win32_SetForegroundWindow,        win32_window Window) \
    IMPORT(User32,   b08,                         SetLayeredWindowAttributes, Win32_SetLayeredWindowAttributes, win32_window Window, win32_color_ref ColorRefKey, u08 Alpha, u32 Flags) \
    IMPORT(User32,   b08,                         SetPropA,                   Win32_SetPropA,                   win32_window Window, chr *String, win32_handle Data) \
    IMPORT(User32,   b08,                         SetPropW,                   Win32_SetPropW,                   win32_window Window, wchr *String, win32_handle Data) \
    IMPORT(User32,   b08,                         SetWindowPlacement,         Win32_SetWindowPlacement,         win32_window Window, win32_window_placement *WindowPlacement) \
    IMPORT(User32,   b08,                         SetWindowPos,               Win32_SetWindowPos,               win32_window Window, win32_window WindowInsertAfter, s32 X, s32 Y, s32 CX, s32 CY, u32 Flags) \
    IMPORT(User32,   b08,                         ShowWindow,                 Win32_ShowWindow,                 win32_window Window, s32 CmdShow) \
    IMPORT(User32,   b08,                         SystemParametersInfoW,      Win32_SystemParametersInfoW,      u32 Action, u32 UParam, vptr VParam, u32 WinIni) \
    IMPORT(User32,   b08,                         TrackMouseEvent,            Win32_TrackMouseEvent,            win32_track_mouse_event *EventTrack) \
    IMPORT(User32,   b08,                         TranslateMessage,           Win32_TranslateMessage,           win32_message *Message) \
    IMPORT(User32,   b08,                         WaitMessage,                Win32_WaitMessage,                void) \
    IMPORT(User32,   s16,                         GetKeyState,                Win32_GetKeyState,                s32 VirtualKey) \
    IMPORT(User32,   s32,                         GetSystemMetrics,           Win32_GetSystemMetrics,           s32 Index) \
    IMPORT(User32,   s32,                         GetWindowLongW,             Win32_GetWindowLongW,             win32_window Window, s32 Index) \
    IMPORT(User32,   s32,                         ReleaseDC,                  Win32_ReleaseDC,                  win32_window Window, win32_device_context DeviceContext) \
    IMPORT(User32,   s32,                         ShowCursor,                 Win32_ShowCursor,                 b08 Show) \
    IMPORT(User32,   s32,                         ToUnicode,                  Win32_ToUnicode,                  u32 VirtualKey, u32 Scancode, u08 *KeyState, wchr *Buffer, s32 BufferSize, u32 Flags) \
    IMPORT(User32,   s32,                         MapWindowPoints,            Win32_MapWindowPoints,            win32_window From, win32_window To, win32_point *Points, u32 PointsCount) \
    IMPORT(User32,   s64,                         DefWindowProcA,             Win32_DefWindowProcA,             win32_window Window, u32 Message, u64 WParam, s64 LParam) \
    IMPORT(User32,   s64,                         DefWindowProcW,             Win32_DefWindowProcW,             win32_window Window, u32 Message, u64 WParam, s64 LParam) \
    IMPORT(User32,   s64,                         DispatchMessageA,           Win32_DispatchMessageA,           win32_message *Message) \
    IMPORT(User32,   s64,                         DispatchMessageW,           Win32_DispatchMessageW,           win32_message *Message) \
    IMPORT(User32,   s64,                         GetMessageTime,             Win32_GetMessageTime,             void) \
    IMPORT(User32,   s64,                         SendMessageW,               Win32_SendMessageW,               win32_window Window, u32 Message, u64 WParam, s64 LParam) \
    IMPORT(User32,   u16,                         RegisterClassA,             Win32_RegisterClassA,             win32_window_class_a *WindowClass) \
    IMPORT(User32,   u16,                         RegisterClassW,             Win32_RegisterClassW,             win32_window_class_w *WindowClass) \
    IMPORT(User32,   u32,                         GetRawInputData,            Win32_GetRawInputData,            win32_raw_input_handle RawInput, u32 Command, vptr Data, u32 *Size, u32 SizeHeader) \
    IMPORT(User32,   u32,                         GetSysColor,                Win32_GetSysColor,                s32 Index) \
    IMPORT(User32,   u32,                         GetWindowThreadProcessId,   Win32_GetWindowThreadProcessId,   win32_window Window, u32 *ProcessId) \
    IMPORT(User32,   u32,                         MapVirtualKeyW,             Win32_MapVirtualKeyW,             u32 Code, u32 MapType) \
    IMPORT(User32,   win32_brush,                 GetSysColorBrush,           Win32_GetSysColorBrush,           s32 Index) \
    IMPORT(User32,   win32_cursor,                LoadCursorA,                Win32_LoadCursorA,                win32_instance Instance, chr *CursorName) \
    IMPORT(User32,   win32_cursor,                LoadCursorW,                Win32_LoadCursorW,                win32_instance Instance, wchr *CursorName) \
    IMPORT(User32,   win32_cursor,                SetCursor,                  Win32_SetCursor,                  win32_cursor Cursor) \
    IMPORT(User32,   win32_device_context,        GetDC,                      Win32_GetDC,                      win32_window Window) \
    IMPORT(User32,   win32_handle,                GetPropA,                   Win32_GetPropA,                   win32_window Window, chr *String) \
    IMPORT(User32,   win32_handle,                GetPropW,                   Win32_GetPropW,                   win32_window Window, wchr *String) \
    IMPORT(User32,   win32_menu,                  GetMenu,                    Win32_GetMenu,                    win32_window Window) \
    IMPORT(User32,   win32_monitor,               MonitorFromPoint,           Win32_MonitorFromPoint,           win32_point Point, u32 Flags) \
    IMPORT(User32,   win32_window,                CreateWindowExA,            Win32_CreateWindowExA,            u32 ExtendedStyle, chr *ClassName, chr *WindowName, u32 Style, s32 X, s32 Y, s32 Width, s32 Height, win32_window Parent, win32_menu Menu, win32_instance Instance, vptr Param) \
    IMPORT(User32,   win32_window,                CreateWindowExW,            Win32_CreateWindowExW,            u32 ExtendedStyle, wchr *ClassName, wchr *WindowName, u32 Style, s32 X, s32 Y, s32 Width, s32 Height, win32_window Parent, win32_menu Menu, win32_instance Instance, vptr Param) \
    IMPORT(User32,   win32_window,                GetActiveWindow,            Win32_GetActiveWindow,            void) \
    IMPORT(User32,   win32_window,                GetCapture,                 Win32_GetCapture,                 void) \
    IMPORT(User32,   win32_window,                GetDesktopWindow,           Win32_GetDesktopWindow,           void) \
    IMPORT(User32,   win32_window,                GetForegroundWindow,        Win32_GetForegroundWindow,        void) \
    IMPORT(User32,   win32_window,                GetParent,                  Win32_GetParent,                  win32_window Window) \
    IMPORT(User32,   win32_window,                GetShellWindow,             Win32_GetShellWindow,             void) \
    IMPORT(User32,   win32_window,                SetCapture,                 Win32_SetCapture,                 win32_window Window) \
    IMPORT(User32,   win32_window,                SetFocus,                   Win32_SetFocus,                   win32_window Window) \

#define WGL__PROCS \
    PROC(,b08,                         wglChoosePixelFormatARB,    WGL_ChoosePixelFormatARB,    win32_device_context DeviceContext, s32 *AttribListI, r32 *AttribListF, u32 MaxFormats, s32 *Formats, u32 *NumFormats) \
    PROC(,b08,                         wglSwapIntervalEXT,         WGL_SwapIntervalEXT,         s32 Num) \
    PROC(,win32_opengl_render_context, wglCreateContextAttribsARB, WGL_CreateContextAttribsARB, win32_device_context DeviceContext, win32_opengl_render_context ShareContext, s32 *AttribList) \

#define PLATFORM__LOADER__PROCS \
    WIN32__PROCS \
    WGL__PROCS \
