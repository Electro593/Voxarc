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
typedef win32_handle win32_window;
typedef win32_instance win32_module;





typedef u32 (API_ENTRY *func_Win32_ThreadCallback)(vptr Parameter);
typedef s64 (API_ENTRY *func_Win32_WindowCallback)(win32_window Window, u32 Message, s64 WParam, s64 LParam);





typedef enum win32_page_flags {
    Win32_Page_ReadWrite = 0x04,
} win32_page_flags;

typedef enum win32_alloc_flags {
    Win32_Alloc_Commit  = 0x1000,
    Win32_Alloc_Reserve = 0x2000,
    Win32_Alloc_Release = 0x8000,
} win32_alloc_flags;

typedef enum win32_class_style_flags {
    Win32_ClassStyle_VRedraw = 0x0001,
    Win32_ClassStyle_HRedraw = 0x0002,
    Win32_ClassStyle_OwnDC   = 0x0020,
} win32_class_style_flags;

typedef enum win32_window_style_flags {
    Win32_WindowStyle_Overlapped = 0x00000000L,
    Win32_WindowStyle_SysMenu    = 0x00080000L,
    Win32_WindowStyle_Caption    = 0x00C00000L,
    Win32_WindowStyle_Visible    = 0x10000000L,
} win32_window_style_flags;

typedef enum win32_window_message {
    Win32_WindowMessage_Destroy       = 0x0002,
    Win32_WindowMessage_Close         = 0x0010,
    Win32_WindowMessage_Quit          = 0x0012,
} win32_window_message;

typedef enum win32_peek_message_flags {
    Win32_PeekMessage_Remove = 0x1,
} win32_peek_message_flags;

typedef enum win32_exception_disposition {
    Win32Exception_ContinueExecution,
    Win32Exception_ContinueSearch,
    Win32Exception_NestedException,
    Win32Exception_CollidedUnwind,
} win32_exception_disposition;

typedef enum win32_pixel_format_descriptor_flags {
    Win32_PixelFormatDescriptor_TypeRGBA      = 0x00,
    Win32_PixelFormatDescriptor_MainPlane     = 0x00,
    Win32_PixelFormatDescriptor_DoubleBuffer  = 0x01,
    Win32_PixelFormatDescriptor_DrawToWindow  = 0x04,
    Win32_PixelFormatDescriptor_SupportOpenGL = 0x20,
} win32_pixel_format_descriptor_flags;

typedef enum wgl_attributes {
    WGL_ARB_DrawToWindow     = 0x2001,
    WGL_ARB_Acceleration     = 0x2003,
    WGL_ARB_SupportOpenGL    = 0x2010,
    WGL_ARB_DoubleBuffer     = 0x2011,
    WGL_ARB_PixelType        = 0x2013,
    WGL_ARB_ColorBits        = 0x2014,
    WGL_ARB_DepthBits        = 0x2022,
    WGL_ARB_StencilBits      = 0x2023,
    WGL_ARB_FullAcceleration = 0x2027,
    WGL_ARB_TypeRGBA         = 0x202B,
    WGL_ARB_SampleBuffers    = 0x2041,
    WGL_ARB_Samples          = 0x2042,
    
    WGL_ARB_Context_MajorVersion = 0x2091,
    WGL_ARB_Context_MinorVersion = 0x2092,
    WGL_ARB_Context_LayerPlane   = 0x2093,
    WGL_ARB_Context_Flags        = 0x2094,
    WGL_ARB_Context_ProfileMask  = 0x9126,
    
    WGL_ARB_Context_CoreProfile = 0x00000001,
    
    WGL_ARB_Context_Debug             = 0x00000001,
    WGL_ARB_Context_ForwardCompatible = 0x00000002,
} wgl_attributes;





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

typedef struct win32_point {
    s32 X;
    s32 Y;
} win32_point;

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
    win32_point Point;
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





#define WIN32_FUNCS \
    FUNC(Gdi32,    s32,                  ChoosePixelFormat,   win32_device_context DeviceContext, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    FUNC(Kernel32, b08,                  CloseHandle,         win32_handle Object) \
    FUNC(Kernel32, win32_handle,         CreateFileA,         c08 *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, win32_handle TemplateFile) \
    FUNC(Kernel32, win32_handle,         CreateThread,        win32_security_attributes *ThreadAttributes, u64 StackSize, func_Win32_ThreadCallback *StartAddress, vptr Parameter, u32 CreationFlags, u32 *ThreadId) \
    FUNC(User32,   win32_window,         CreateWindowExA,     u32 StyleEx, c08 *ClassName, c08 *WindowName, u32 Style, s32 x, s32 y, s32 Width, s32 Height, win32_window ParentWindow, win32_menu Menu, win32_instance Instance, vptr Param) \
    FUNC(User32,   s32,                  DefWindowProcA,      win32_window Window, u32 Message, s64 WParam, s64 LParam) \
    FUNC(Gdi32,    s32,                  DescribePixelFormat, win32_device_context DeviceContext, s32 PixelFormat, u32 BytesCount, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    FUNC(User32,   b08,                  DestroyWindow,       win32_window Window) \
    FUNC(User32,   s64,                  DispatchMessageA,    win32_message *Message) \
    FUNC(Kernel32, void,                 ExitProcess,         u32 ExitCode) \
    FUNC(User32,   win32_device_context, GetDC,               win32_window Window) \
    FUNC(Kernel32, b08,                  GetFileSizeEx,       win32_handle File, win32_large_integer *FileSize) \
    FUNC(Kernel32, u32,                  GetLastError,        void) \
    FUNC(User32,   b32,                  GetMessageA,         win32_message *Msg, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax) \
    FUNC(Kernel32, win32_module,         GetModuleHandleA,    c08 *ModuleName) \
    FUNC(Kernel32, fptr,                 GetProcAddress,      win32_module Module, c08 *Name) \
    FUNC(Gdi32,    win32_gdi_object,     GetStockObject,      s32 I) \
    FUNC(User32,   win32_cursor,         LoadCursorA,         win32_instance Instance, c08 *CursorName) \
    FUNC(User32,   win32_icon,           LoadIconA,           win32_instance Instance, c08 *IconName) \
    FUNC(Kernel32, win32_module,         LoadLibraryA,        c08 *Name) \
    FUNC(Kernel32, void,                 OutputDebugStringA,  c08 *String) \
    FUNC(User32,   b32,                  PeekMessageA,        win32_message *Message, win32_window Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    FUNC(User32,   b32,                  PostThreadMessageA,  u32 Thread, u32 Message, s64 WParam, s64 LParam) \
    FUNC(Kernel32, b08,                  ReadFile,            win32_handle File, vptr Buffer, u32 NumberOfBytesToRead, u32 *NumberOfBytesRead, win32_overlapped *Overlapped) \
    FUNC(User32,   win32_atom,           RegisterClassA,      win32_window_class_a *WindowClass) \
    FUNC(User32,   s32,                  ReleaseDC,           win32_window Window, win32_device_context DeviceContext) \
    FUNC(User32,   s64,                  SendMessageA,        win32_window Window, u32 Message, s64 WParam, s64 LParam) \
    FUNC(Gdi32,    b08,                  SetPixelFormat,      win32_device_context DeviceContext, s32 Format, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    FUNC(Gdi32,    b08,                  SwapBuffers,         win32_device_context DeviceContext) \
    FUNC(User32,   b32,                  TranslateMessage,    win32_message *Message) \
    FUNC(Kernel32, vptr,                 VirtualAlloc,        vptr Address, u64 Size, u32 AllocationType, u32 Protect) \
    FUNC(Kernel32, b08,                  VirtualFree,         vptr Address, u64 Size, u32 FreeType) \
    FUNC(User32,   b08,                  WaitMessage,         void) \
    FUNC(Kernel32, b08,                  WriteFile,           win32_handle File, vptr Buffer, u32 NumberOfBytesToWrite, u32 *NumberOfBytesWritten, win32_overlapped *Overlapped) \

#define WGL_FUNCS \
    FUNC_TYPE2(b08,                         ChoosePixelFormatARB,    win32_device_context DeviceContext, s32 *AttribListI, r32 *AttribListF, u32 MaxFormats, s32 *Formats, u32 *NumFormats) \
    FUNC_TYPE1(win32_opengl_render_context, CreateContext,           win32_device_context DeviceContext) \
    FUNC_TYPE2(win32_opengl_render_context, CreateContextAttribsARB, win32_device_context DeviceContext, win32_opengl_render_context ShareContext, s32 *AttribList) \
    FUNC_TYPE1(b08,                         DeleteContext,           win32_opengl_render_context RenderContext) \
    FUNC_TYPE1(fptr,                        GetProcAddress,          c08 *Process) \
    FUNC_TYPE1(b08,                         MakeCurrent,             win32_device_context DeviceContext, win32_opengl_render_context RenderContext) \
    FUNC_TYPE2(b08,                         SwapIntervalEXT,         s32 Num) \

#define FUNC(Module, ReturnType, Name, ...) \
    typedef ReturnType func_Win32_##Name(__VA_ARGS__); \
    global func_Win32_##Name *Win32_##Name;
WIN32_FUNCS
#undef FUNC

#define FUNC_TYPE1(ReturnType, Name, ...) \
    typedef ReturnType func_WGL_##Name(__VA_ARGS__); \
    global func_WGL_##Name *WGL_##Name;
#define FUNC_TYPE2 FUNC_TYPE1
WGL_FUNCS
#undef FUNC_TYPE1
#undef FUNC_TYPE2