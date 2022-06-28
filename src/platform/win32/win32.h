/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INVALID_HANDLE_VALUE ((vptr)(s64)-1)

#define BRUSH_BLACK 4
#define CW_USEDEFAULT ((s32)0x80000000)
#define IDC_ARROW       ((c08*)((u64)((u16)(32512))))
#define IDI_APPLICATION ((c08*)((u64)((u16)(32512))))

#define READ_CONTROL  0x00020000L
#define SYNCHRONIZE   0x00100000L
#define GENERIC_WRITE 0x40000000L
#define GENERIC_READ  0x80000000L
#define FILE_SHARE_READ 0x00000001
#define FILE_ATTRIBUTE_NORMAL 0x80

#define ERROR_SUCCESS          0L
#define ERROR_FILE_NOT_FOUND   2L
#define ERROR_PATH_NOT_FOUND   3L
#define ERROR_ALREADY_EXISTS 183L

#define CREATE_NEW    1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3

#define HID_USAGE_PAGE_GENERIC 0x01

#define HID_USAGE_GENERIC_MOUSE 0x02

#define RIDEV_REMOVE   0x00000001
#define RIDEV_NOLEGACY 0x00000030

#define RID_INPUT 0x10000003

#define MOUSE_MOVE_RELATIVE 0x00
#define MOUSE_MOVE_ABSOLUTE 0x01

#define RI_MOUSE_LEFT_BUTTON_DOWN   0x0001
#define RI_MOUSE_LEFT_BUTTON_UP     0x0002
#define RI_MOUSE_RIGHT_BUTTON_DOWN  0x0004
#define RI_MOUSE_RIGHT_BUTTON_UP    0x0008
#define RI_MOUSE_MIDDLE_BUTTON_DOWN 0x0010
#define RI_MOUSE_MIDDLE_BUTTON_UP   0x0020
#define RI_MOUSE_BUTTON_4_DOWN      0x0040
#define RI_MOUSE_BUTTON_4_UP        0x0080
#define RI_MOUSE_BUTTON_5_DOWN      0x0100
#define RI_MOUSE_BUTTON_5_UP        0x0200

#define HTCLIENT 1

#define MA_ACTIVATE         1
#define MA_ACTIVATEANDEAT   2
#define MA_NOACTIVATE       3
#define MA_NOACTIVATEANDEAT 4





typedef u16 win32_atom;
typedef vptr win32_handle;
typedef win32_handle win32_brush;
typedef win32_handle win32_cursor;
typedef win32_handle win32_device_context;
typedef win32_handle win32_gdi_object;
typedef win32_handle win32_icon;
typedef win32_handle win32_instance;
typedef win32_handle win32_menu;
typedef win32_handle win32_opengl_render_context;
typedef win32_handle win32_raw_input_handle;
typedef win32_handle win32_window;
typedef win32_instance win32_module;





typedef u32 (API_ENTRY *func_Win32_ThreadCallback)(vptr Parameter);
typedef s64 (API_ENTRY *func_Win32_WindowCallback)(win32_window Window, u32 Message, s64 WParam, s64 LParam);





#define PAGE_READWRITE 0x04

#define MEM_COMMIT  0x1000
#define MEM_RESERVE 0x2000
#define MEM_RELEASE 0x8000

#define WS_OVERLAPPED  0x00000000
#define WS_MAXIMIZEBOX 0x00010000
#define WS_MINIMIZEBOX 0x00020000
#define WS_THICKFRAME  0x00040000
#define WS_SYSMENU     0x00080000
#define WS_CAPTION     0x00C00000
#define WS_VISIBLE     0x10000000

#define WM_DESTROY        0x0002
#define WM_SIZE           0x0005
#define WM_SETFOCUS       0x0007
#define WM_KILLFOCUS      0x0008
#define WM_CLOSE          0x0010
#define WM_QUIT           0x0012
#define WM_MOUSEACTIVATE  0x0021
#define WM_INPUT          0x00FF
#define WM_KEYDOWN        0x0100
#define WM_KEYUP          0x0101
#define WM_SYSKEYDOWN     0x0104
#define WM_SYSKEYUP       0x0105
#define WM_MOUSEMOVE      0x0200
#define WM_LBUTTONDOWN    0x0201
#define WM_LBUTTONUP      0x0202
#define WM_SIZING         0x0214
#define WM_CAPTURECHANGED 0x0215

#define PM_REMOVE 0x1

#define PFD_TYPE_RGBA      0x00
#define PFD_MAIN_PLANE     0x00
#define PFD_DOUBLE_BUFFER  0x01
#define PFD_DRAW_TO_WINDOW 0x04
#define PFD_SUPPORT_OPENGL 0x20

#define WGL_DRAW_TO_WINDOW_ARB    0x2001
#define WGL_ACCELERATION_ARB      0x2003
#define WGL_SUPPORT_OPENGL_ARB    0x2010
#define WGL_DOUBLE_BUFFER_ARB     0x2011
#define WGL_PIXEL_TYPE_ARB        0x2013
#define WGL_COLOR_BITS_ARB        0x2014
#define WGL_DEPTH_BITS_ARB        0x2022
#define WGL_STENCIL_BITS_ARB      0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB         0x202B
#define WGL_SAMPLE_BUFFERS_ARB    0x2041
#define WGL_SAMPLES_ARB           0x2042

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB       0x0001
#define WGL_CONTEXT_DEBUG_BIT_ARB              0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB          0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB          0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB            0x2093
#define WGL_CONTEXT_FLAGS_ARB                  0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB           0x9126





typedef enum win32_exception_disposition {
    Win32Exception_ContinueExecution,
    Win32Exception_ContinueSearch,
    Win32Exception_NestedException,
    Win32Exception_CollidedUnwind,
} win32_exception_disposition;





typedef struct win32_rect {
    s32 Left;
    s32 Top;
    s32 Right;
    s32 Bottom;
} win32_rect;

typedef struct win32_unicode_string {
    u16 Length;
    u16 MaximumLength;
    u08 _Padding0_[4];
    c16 *Buffer;
} win32_unicode_string;

typedef struct win32_list_entry {
    struct win32_list_entry *Next;
    struct win32_list_entry *Prev;
} win32_list_entry;

typedef union win32_large_integer {
    u32 LowPart;
    s32 HighPart;
    struct {
        u32 LowPart;
        s32 HighPart;
    } U;
    s64 QuadPart;
} win32_large_integer;

typedef struct win32_file_time {
    u32 LowDateTime;
    u32 HighDateTime;
} win32_file_time;

typedef struct win32_system_time {
    u16 Year;
    u16 Month;
    u16 DayOfWeek;
    u16 Day;
    u16 Hour;
    u16 Minute;
    u16 Second;
    u16 Milliseconds;
} win32_system_time;

typedef struct win32_window_class_a {
    u32 Style;
    func_Win32_WindowCallback Callback;
    s32 WindowClassExtraBytes;
    s32 WindowInstanceExtraBytes;
    win32_instance Instance;
    win32_icon Icon;
    win32_cursor Cursor;
    win32_brush Background;
    c08 *MenuName;
    c08 *ClassName;
} win32_window_class_a;

typedef struct win32_overlapped {
    u64 Internal;
    u64 InternalHigh;
    union {
        struct {
            u32 Offset;
            u32 OffsetHigh;
        };
        vptr Pointer;
    };
    win32_handle Event;
} win32_overlapped;

typedef struct win32_security_attributes {
    u32 Length;
    vptr SecurityDescriptor;
    b08 InheritHandle;
} win32_security_attributes;

typedef struct win32_pixel_format_descriptor {
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

typedef struct win32_message {
    win32_window Window;
    u32 Message;
    u64 WParam;
    s64 LParam;
    u32 Time;
    v2s32 Point;
    u32 Private;
} win32_message;

typedef struct win32_exception_record {
    u32 ExceptionCode;
    u32 ExceptionFlags;
    struct win32_exception_record *ExceptionRecord;
    vptr ExceptionAddress;
    u32 NumberParameters;
    u64 ExceptionInformation[15];
} win32_exception_record;

typedef struct win32_context {
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
    union {
        struct win32_xsave_format {
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
            struct win32_m128a {
                u64 Low;
                s64 High;
            } FloatRegisters[8];
            struct win32_m128a XmmRegisters[16];
            u08 Reserved4[96];
        } FltSave;
        struct {
            struct win32_m128a Header[2];
            struct win32_m128a Legacy[8];
            struct win32_m128a Xmm0;
            struct win32_m128a Xmm1;
            struct win32_m128a Xmm2;
            struct win32_m128a Xmm3;
            struct win32_m128a Xmm4;
            struct win32_m128a Xmm5;
            struct win32_m128a Xmm6;
            struct win32_m128a Xmm7;
            struct win32_m128a Xmm8;
            struct win32_m128a Xmm9;
            struct win32_m128a Xmm10;
            struct win32_m128a Xmm11;
            struct win32_m128a Xmm12;
            struct win32_m128a Xmm13;
            struct win32_m128a Xmm14;
            struct win32_m128a Xmm15;
        };
    };
    struct win32_m128a VectorRegister[26];
    u64 VectorControl;
    u64 DebugControl;
    u64 LastBranchToRip;
    u64 LastBranchFromRip;
    u64 LastExceptionToRip;
    u64 LastExceptionFromRip;
} win32_context;

typedef struct win32_ldr_data_table_entry {
    win32_list_entry LoadOrderLinks;
    win32_list_entry MemoryOrderLinks;
    win32_list_entry InitializationOrderLinks;
    vptr DllBase;
    vptr EntryPoint;
    u32 SizeOfImage;
    u08 _Padding0_[4];
    win32_unicode_string FullDllName;
    win32_unicode_string BaseDllName;
    union {
        u08 FlagGroup[4];
        u32 Flags;
        struct {
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
    struct win32_ldr_ddag_node {
        win32_list_entry Modules;
        struct win32_ldr_service_tag_record {
            struct win32_ldr_service_tag_record *Next;
            u32 ServiceTag;
            u08 _Padding0_[4];
        } *ServiceTagList;
        u32 LoadCount;
        u32 LoadWhileUnloadingCount;
        u32 LowestLink;
        u08 _Padding0_[4];
        struct win32_ldrp_cslist {
            struct win32_single_list_entry {
                struct win32_single_list_entry *Next;
            } *Tail;
        } Dependencies;
        struct win32_ldrp_cslist IncomingDependencies;
        enum win32_ldr_ddag_state {
            Win32Ldr_ModulesMerged                  = -5,
            Win32Ldr_ModulesInitError               = -4,
            Win32Ldr_ModulesSnapError               = -3,
            Win32Ldr_ModulesUnloaded                = -2,
            Win32Ldr_ModulesUnloading               = -1,
            Win32Ldr_ModulesPlaceHolder             = 0,
            Win32Ldr_ModulesMapping                 = 1,
            Win32Ldr_ModulesMapped                  = 2,
            Win32Ldr_ModulesWaitingForDependencies  = 3,
            Win32Ldr_ModulesSnapping                = 4,
            Win32Ldr_ModulesSnapped                 = 5,
            Win32Ldr_ModulesCondensed               = 6,
            Win32Ldr_ModulesReadyToInit             = 7,
            Win32Ldr_ModulesInitializing            = 8,
            Win32Ldr_ModulesReadyToRun              = 9,
        } State;
        u08 _Padding1_[4];
        struct win32_single_list_entry CondenseLink;
        u32 PreorderNumber;
        u08 _Padding2_[4];
    } *DdagNode;
    win32_list_entry NodeModuleLink;
    struct win32_ldrp_load_context *LoadContext;
    vptr ParentDllBase;
    vptr SwitchBackContext;
    struct win32_rtl_balanced_node {
        union {
            struct win32_rtl_balanced_node *Children[2];
            struct {
                struct win32_rtl_balanced_node *Left;
                struct win32_rtl_balanced_node *Right;
            };
        };
        union {
            struct {
                u08 Red     : 1;
                u08 Balance : 2;
            };
            u64 ParentValue;
        };
    } BaseAddressIndexNode;
    struct win32_rtl_balanced_node MappingInfoIndexNode;
    u64 OriginalBase;
    u08 _Padding2_[4];
    win32_large_integer LoadTime;
    u32 BaseNameHashValue;
    enum win32_ldr_dll_load_reason {
        Win32LoadReason_Unknown                    = -1,
        Win32LoadReason_StaticDependency           =  0,
        Win32LoadReason_StaticForwarderDependency  =  1,
        Win32LoadReason_DynamicForwarderDependency =  2,
        Win32LoadReason_DelayloadDependency        =  3,
        Win32LoadReason_DynamicLoad                =  4,
        Win32LoadReason_AsImageLoad                =  5,
        Win32LoadReason_AsDataLoad                 =  6,
        Win32LoadReason_EnclavePrimary             =  7,
        Win32LoadReason_EnclaveDependency          =  8,
    } LoadReason;
    u32 ImplicitPathOptions;
    u32 ReferenceCount;
    u32 DependentLoadFlags;
    u08 SigningLevel;
    u08 _Padding3_[3];
} win32_ldr_data_table_entry;

typedef struct win32_teb {
    struct win32_nt_tib {
        struct win32_exception_registration_record {
            struct win32_exception_registration_record *Next;
            win32_exception_disposition (API_ENTRY *Handler)(win32_exception_record *ExceptionRecord, vptr EstablisherFrame, win32_context *ContextRecord, vptr DispatcherContext);
        } *ExceptionList;
        vptr StackBase;
        vptr StackLimit;
        vptr SubSystemTib;
        union {
            vptr FiberData;
            u32 Version;
        };
        vptr ArbitraryUserPointer;
        struct win32_nt_tib *Self;
    } NtTib;
    vptr EnvironmentPointer;
    struct win32_client_id {
        win32_handle UniqueProcess;
        win32_handle UniqueThread;
    } ClientId;
    vptr ActiveRpcHandle;
    vptr ThreadLocalStoragePointer;
    struct win32_peb {
        u08 InheritedAddressSpace;
        u08 ReadImageFileExecOptions;
        u08 BeingDebugged;
        union {
            u08 BitField;
            struct {
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
        struct win32_peb_ldr_data {
            u32 Length;
            u08 Initialized;
            u08 _Padding0_[3];
            vptr SsHandle;
            win32_list_entry LoadOrderList;
            win32_list_entry MemoryOrderList;
            win32_list_entry InitializationOrderList;
            vptr EntryInProgress;
            u08 ShutdownInProgress;
            u08 _Padding1_[7];
            vptr ShutdownThreadId;
        } *Ldr;
        struct win32_rtl_user_process_parameters {
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
            struct win32_curdir {
                win32_unicode_string DosPath;
                vptr Handle;
            } CurrentDirectory;
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
            struct win32_rtl_drive_letter_curdir {
                u16 Flags;
                u16 Length;
                u32 TimeStamp;
                struct win32_string {
                    u16 Length;
                    u16 MaximumLength;
                    u08 _Padding0_[4];
                    c08 *Buffer;
                } DosPath;
            } CurrentDirectores[32];
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
        } *ProcessParameters;
        vptr SubSystemData;
        vptr ProcessHeap;
        struct win32_rtl_critical_section {
            struct win32_rtl_critical_section_debug {
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
            } *DebugInfo;
            s32 LockCount;
            s32 RecursionCount;
            vptr OwningThread;
            vptr LockSemaphore;
            u64 SpinCount;
        } *FastPebLock;
        union win32_slist_header {
            u64 Alignment;
            u64 Region;
            struct {
                u64 Depth     : 16;
                u64 Sequence  : 48;
                u64 Reserved  :  4;
                u64 NextEntry : 60;
            } HeaderX64;
        } *AtlThunkSListPtr;
        vptr IFEOKey;
        union {
            u32 CrossProcessFlags;
            struct {
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
        union {
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
        struct win32_rtl_critical_section *LoaderLock;
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
        void (API_ENTRY *PostProcessInitRoutine)(void);
        vptr TlsExpansionBitmap;
        u32 TlsExpansionBitmapBits[32];
        u32 SessionId;
        u08 _Padding6_[4];
        union win32_ularge_integer {
            u32 LowPart;
            u32 HighPart;
            struct {
                u32 LowPart;
                u32 HighPart;
            } U;
            u64 QuadPart;
        } AppCompatFlags;
        union win32_ularge_integer AppCompatFlagsUser;
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
        union {
            u32 TracingFlags;
            struct {
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
        c08 PlaceholderCompatibilityMode;
        c08 PlaceholderCompatibilityModeReserved[7];
        struct win32_leap_second_data {
            u08 Enabled;
            u08 _Padding0_[3];
            u32 Count;
            win32_large_integer Data[1];
        } *LeapSecondData;
        union {
            u32 LeapSecondFlags;
            struct {
                u32 SixtySecondEnabled :  1;
                u32 Reserved           : 31;
            };
        };
        u32 NtGlobalFlag2;
    } *PEB;
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
    c08 PlaceholderCompatibilityMode;
    u08 PlaceholderHydrationAlwaysExplicit;
    c08 PlaceholderReserved[10];
    u32 ProxiedProcessId;
    struct win32_activation_context_stack {
        struct win32_rtl_activation_context_stack_frame {
            struct win32_rtl_activation_context_stack_frame *Previous;
            struct win32_activation_context *ActivationContext;
            u32 Flags;
            u08 _Padding0_[4];
        } *ActiveFrame;
        win32_list_entry FrameListCache;
        u32 Flags;
        u32 NextCookieSequenceNumber;
        u32 StackId;
        u08 _Padding0_[4];
    } ActivationStack;
    u08 WorkingOnBehalfOfTicket[8];
    s32 ExceptionCode;
    u08 _Padding1_[4];
    struct win32_activation_context_stack *ActivationContextStackPointer;
    u64 InstrumentationCallbackSp;
    u64 InstrumentationCallbackPreviousPc;
    u64 InstrumentationCallbackPreviousSp;
    u32 TxFsContext;
    u08 InstrumentationCallbackDisabled;
    u08 UnalignedLoadStoreExceptions;
    u08 _Padding2_[2];
    struct win32_gdi_teb_batch {
        struct {
            u32 Offset              : 31;
            u32 HasRenderingCommand :  1;
        };
        u64 HDC;
        u32 Buffer[310];
    } GdiTebBatch;
    struct win32_client_id RealClientId;
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
    c16 StaticUnicodeBuffer[261];
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
    struct win32_guid {
        u32 Data1;
        u16 Data2;
        u16 Data3;
        u08 Data4[8];
    } ActivityId;
    vptr SubProcessTag;
    vptr PerflibData;
    vptr EtwTraceData;
    vptr WinSockData;
    u32 GdiBatchCount;
    union {
        struct win32_processor_number {
            u16 Group;
            u08 Number;
            u08 Reserved;
        } CurrentIdealProcessor;
        u32 IdealProcessorValue;
        struct {
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
    struct win32_teb_active_frame {
        u32 Flags;
        u08 _Padding0_[4];
        struct win32_teb_active_frame *Previous;
        struct win32_teb_active_frame_context {
            u32 Flags;
            u08 _Padding0_[4];
            c08 *FrameName;
        } *Context;
    } *ActiveFrame;
    vptr FlsData;
    vptr PreferredLanguages;
    vptr UserPrefLanguages;
    u32 MuiImpersonation;
    union {
        u16 CrossTebFlags;
        u16 SpareCrossTebBits;
    };
    union {
        u16 SameTebFlags;
        struct {
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
    struct win32_guid EffectiveContainerId;
} win32_teb;

typedef struct win32_image_dos_header {
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

typedef struct win32_image_nt_headers
{
    u32 Signature;
    struct win32_image_file_header {
        u16 Machine;
        u16 NumberOfSections;
        u32 TimeDateStamp;
        u32 PointerToSymbolTable;
        u32 NumberOfSymbols;
        u16 SizeOfOptionalHeader;
        u16 Characteristics;
    } FileHeader;
    struct win32_image_optional_header {
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
        struct win32_image_data_directory {
            u32 VirtualAddress;
            u32 Size;
        } ExportTable;
        struct win32_image_data_directory ImportTable;
        struct win32_image_data_directory ResourceTable;
        struct win32_image_data_directory ExceptionTable;
        struct win32_image_data_directory CertificateTable;
        struct win32_image_data_directory BaseRelocationTable;
        struct win32_image_data_directory Debug;
        struct win32_image_data_directory Architecture;
        struct win32_image_data_directory GlobalPtr;
        struct win32_image_data_directory TLSTable;
        struct win32_image_data_directory LoadConfigTable;
        struct win32_image_data_directory BoundImport;
        struct win32_image_data_directory IAT;
        struct win32_image_data_directory DelayImportDescriptor;
        struct win32_image_data_directory CLRRuntimeHeader;
        struct win32_image_data_directory ReservedTable;
    } OptionalHeader;
} win32_image_nt_headers;

typedef struct win32_image_export_directory {
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

typedef struct win32_raw_input_device {
    u16 UsagePage;
    u16 Usage;
    u32 Flags;
    win32_window Target;
} win32_raw_input_device;

typedef struct win32_raw_input_header {
    u32 Type;
    u32 Size;
    win32_handle Device;
    u64 WParam;
} win32_raw_input_header;

typedef struct win32_raw_mouse {
    u16 Flags;
    union {
        u32 Buttons;
        struct {
            u16 ButtonFlags;
            u16 ButtonData;
        };
    };
    u32 RawButtons;
    s32 LastX;
    s32 LastY;
    u32 ExtraInformation;
} win32_raw_mouse;

typedef struct win32_raw_keyboard {
    u16 MakeCode;
    u16 Flags;
    u16 Reserved;
    u16 VKey;
    u32 Message;
    u32 ExtraInformation;
} win32_raw_keyboard;

typedef struct win32_raw_hid {
    u32 Size;
    u32 Count;
    u08 RawData[1];
} win32_raw_hid;

typedef struct win32_raw_input {
    win32_raw_input_header Header;
    union {
        win32_raw_mouse Mouse;
        win32_raw_keyboard Keyboard;
        win32_raw_hid HID;
    };
} win32_raw_input;





#define WIN32_FUNCS \
    IMPORT(Gdi32,    s32,                  ChoosePixelFormat,         win32_device_context DeviceContext, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    IMPORT(User32,   b32,                  ClientToScreen,            win32_window Window, v2s32 *Point) \
    IMPORT(User32,   b32,                  ClipCursor,                win32_rect *ClipRect) \
    IMPORT(Kernel32, b08,                  CloseHandle,               win32_handle Object) \
    IMPORT(Kernel32, s32,                  CompareFileTime,           win32_file_time *FileTime1, win32_file_time *FileTime2) \
    IMPORT(Kernel32, b32,                  CopyFileA,                 c08 *ExistingFileName, c08 *NewFileName, b32 FailIfExists) \
    IMPORT(Kernel32, win32_handle,         CreateFileA,               c08 *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, win32_handle TemplateFile) \
    IMPORT(User32,   win32_window,         CreateWindowExA,           u32 StyleEx, c08 *ClassName, c08 *WindowName, u32 Style, s32 x, s32 y, s32 Width, s32 Height, win32_window ParentWindow, win32_menu Menu, win32_instance Instance, vptr Param) \
    IMPORT(User32,   s32,                  DefWindowProcA,            win32_window Window, u32 Message, s64 WParam, s64 LParam) \
    IMPORT(Gdi32,    s32,                  DescribePixelFormat,       win32_device_context DeviceContext, s32 PixelFormat, u32 BytesCount, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    IMPORT(User32,   b08,                  DestroyWindow,             win32_window Window) \
    IMPORT(User32,   s64,                  DispatchMessageA,          win32_message *Message) \
    IMPORT(Kernel32, void,                 ExitProcess,               u32 ExitCode) \
    IMPORT(Kernel32, b32,                  FreeLibrary,               win32_module Library) \
    IMPORT(User32,   b32,                  GetClientRect,             win32_window Window, win32_rect *RectOut) \
    IMPORT(User32,   b32,                  GetCursorPos,              v2s32 *Point) \
    IMPORT(User32,   win32_device_context, GetDC,                     win32_window Window) \
    IMPORT(Kernel32, b08,                  GetFileSizeEx,             win32_handle File, win32_large_integer *FileSize) \
    IMPORT(Kernel32, u32,                  GetLastError,              void) \
    IMPORT(Kernel32, b32,                  FileTimeToSystemTime,      win32_file_time *FileTime, win32_system_time *SystemTime) \
    IMPORT(Kernel32, b32,                  GetFileTime,               win32_handle File, win32_file_time *CreationTime, win32_file_time *LastAccessTime, win32_file_time *LastWriteTime) \
    IMPORT(User32,   b32,                  GetMessageA,               win32_message *Msg, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax) \
    IMPORT(Kernel32, win32_module,         GetModuleHandleA,          c08 *ModuleName) \
    IMPORT(Kernel32, fptr,                 GetProcAddress,            win32_module Module, c08 *Name) \
    IMPORT(User32,   vptr,                 GetPropA,                  win32_window Window, c08 *Name) \
    IMPORT(User32,   u32,                  GetRawInputData,           win32_raw_input_handle RawInputHandle, u32 Command, vptr Data, u32 *DataSize, u32 HeaderSize) \
    IMPORT(Gdi32,    win32_gdi_object,     GetStockObject,            s32 I) \
    IMPORT(User32,   win32_cursor,         LoadCursorA,               win32_instance Instance, c08 *CursorName) \
    IMPORT(User32,   win32_icon,           LoadIconA,                 win32_instance Instance, c08 *IconName) \
    IMPORT(Kernel32, win32_module,         LoadLibraryA,              c08 *Name) \
    IMPORT(Kernel32, void,                 OutputDebugStringA,        c08 *String) \
    IMPORT(User32,   b32,                  PeekMessageA,              win32_message *Message, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    IMPORT(Kernel32, b32,                  QueryPerformanceCounter,   s64 *Time) \
    IMPORT(Kernel32, b32,                  QueryPerformanceFrequency, s64 *Frequency) \
    IMPORT(Kernel32, b08,                  ReadFile,                  win32_handle File, vptr Buffer, u32 NumberOfBytesToRead, u32 *NumberOfBytesRead, win32_overlapped *Overlapped) \
    IMPORT(User32,   win32_atom,           RegisterClassA,            win32_window_class_a *WindowClass) \
    IMPORT(User32,   b32,                  RegisterRawInputDevices,   win32_raw_input_device *Devices, u32 DeviceCount, u32 Size) \
    IMPORT(User32,   s32,                  ReleaseDC,                 win32_window Window, win32_device_context DeviceContext) \
    IMPORT(User32,   b32,                  ScreenToClient,            win32_window Window, v2s32 *Point) \
    IMPORT(User32,   s64,                  SendMessageA,              win32_window Window, u32 Message, s64 WParam, s64 LParam) \
    IMPORT(User32,   b32,                  SetCursor,                 win32_cursor Cursor) \
    IMPORT(User32,   b32,                  SetCursorPos,              s32 X, s32 Y) \
    IMPORT(Gdi32,    b08,                  SetPixelFormat,            win32_device_context DeviceContext, s32 Format, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    IMPORT(User32,   b32,                  SetPropA,                  win32_window Window, c08 *Name, vptr Data) \
    IMPORT(Gdi32,    b08,                  SwapBuffers,               win32_device_context DeviceContext) \
    IMPORT(User32,   b32,                  TranslateMessage,          win32_message *Message) \
    IMPORT(Kernel32, vptr,                 VirtualAlloc,              vptr Address, u64 Size, u32 AllocationType, u32 Protect) \
    IMPORT(Kernel32, b08,                  VirtualFree,               vptr Address, u64 Size, u32 FreeType) \
    IMPORT(User32,   b08,                  WaitMessage,               void) \
    IMPORT(Kernel32, b08,                  WriteFile,                 win32_handle File, vptr Buffer, u32 NumberOfBytesToWrite, u32 *NumberOfBytesWritten, win32_overlapped *Overlapped) \

#define WGL_FUNCS_TYPE_1 \
    IMPORT(win32_opengl_render_context, CreateContext,           win32_device_context DeviceContext) \
    IMPORT(b08,                         DeleteContext,           win32_opengl_render_context RenderContext) \
    IMPORT(fptr,                        GetProcAddress,          c08 *Process) \
    IMPORT(b08,                         MakeCurrent,             win32_device_context DeviceContext, win32_opengl_render_context RenderContext) \

#define WGL_FUNCS_TYPE_2 \
    IMPORT(b08,                         ChoosePixelFormatARB,    win32_device_context DeviceContext, s32 *AttribListI, r32 *AttribListF, u32 MaxFormats, s32 *Formats, u32 *NumFormats) \
    IMPORT(win32_opengl_render_context, CreateContextAttribsARB, win32_device_context DeviceContext, win32_opengl_render_context ShareContext, s32 *AttribList) \
    IMPORT(b08,                         SwapIntervalEXT,         s32 Num) \

#define IMPORT(Module, ReturnType, Name, ...) \
    typedef ReturnType func_Win32_##Name(__VA_ARGS__); \
    global func_Win32_##Name *Win32_##Name;
#define X WIN32_FUNCS
#include <x.h>

#define IMPORT(ReturnType, Name, ...) \
    typedef ReturnType func_WGL_##Name(__VA_ARGS__); \
    global func_WGL_##Name *WGL_##Name;
#define X WGL_FUNCS_TYPE_1 WGL_FUNCS_TYPE_2
#include <x.h>