/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _DEBUG
external void _API_ENTRY
GL_DebugCallback(u32 Source,
                 u32 Type,
                 u32 ID,
                 u32 Severity,
                 s32 Length,
                 chr *Message,
                 vptr UserParam)
{
    UNUSED(Length, UserParam);
    
    str TempStr;
    
    str SourceStr   = Str_Create(0, "\nSource:   ", 0);
    str TypeStr     = Str_Create(0, "\nType:     ", 0);
    str IDStr       = Str_Create(0, "\nError:    ", 0);
    str SeverityStr = Str_Create(0, "\nSeverity: ", 0);
    str MessageStr  = Str_Create(0, "\nMessage:  ", 0);
    
    switch(Source)
    {
        case GL_DEBUG_SOURCE_API:
        {
            Str_CatC(&SourceStr, "API");
        } break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        {
            Str_CatC(&SourceStr, "Window");
        } break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        {
            Str_CatC(&SourceStr, "Shader");
        } break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
        {
            Str_CatC(&SourceStr, "Third-Party");
        } break;
        case GL_DEBUG_SOURCE_APPLICATION:
        {
            Str_CatC(&SourceStr, "Application");
        } break;
        case GL_DEBUG_SOURCE_OTHER:
        {
            Str_CatC(&SourceStr, "Other");
        } break;
        default:
        {
            Str_CatC(&SourceStr, "Unknown ()");
            Str_Ins(&SourceStr, U32_To_Str(&TempStr, Source), 9);
            Str_Free(TempStr);
        }
    }
    
    switch(Type)
    {
        case GL_DEBUG_TYPE_ERROR:
        {
            Str_CatC(&TypeStr, "Error");
        } break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        {
            Str_CatC(&TypeStr, "Deprecated");
        } break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        {
            Str_CatC(&TypeStr, "Undefined");
        } break;
        case GL_DEBUG_TYPE_PORTABILITY:
        {
            Str_CatC(&TypeStr, "Portability");
        } break;
        case GL_DEBUG_TYPE_PERFORMANCE:
        {
            Str_CatC(&TypeStr, "Performance");
        } break;
        case GL_DEBUG_TYPE_MARKER:
        {
            Str_CatC(&TypeStr, "Marker");
        } break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
        {
            Str_CatC(&TypeStr, "Group Push");
        } break;
        case GL_DEBUG_TYPE_POP_GROUP:
        {
            Str_CatC(&TypeStr, "Group Pop");
        } break;
        case GL_DEBUG_TYPE_OTHER:
        {
            Str_CatC(&TypeStr, "Other");
        } break;
        default:
        {
            Str_CatC(&TypeStr, "Unknown ()");
            Str_Ins(&TypeStr, U32_To_Str(&TempStr, Type), 9);
            Str_Free(TempStr);
        }
    }
    
    switch(ID)
    {
        case GL_INVALID_ENUM:
        {
            Str_CatC(&IDStr, "Invalid Enum");
        } break;
        case GL_INVALID_VALUE:
        {
            Str_CatC(&IDStr, "Invalid Value");
        } break;
        case GL_INVALID_OPERATION:
        {
            Str_CatC(&IDStr, "Invalid Operation");
        } break;
        case GL_STACK_OVERFLOW:
        {
            Str_CatC(&IDStr, "Stack Overflow");
        } break;
        case GL_STACK_UNDERFLOW:
        {
            Str_CatC(&IDStr, "Stack Underflow");
        } break;
        case GL_OUT_OF_MEMORY:
        {
            Str_CatC(&IDStr, "Out of Memory");
        } break;
        default:
        {
            Str_CatC(&IDStr, "Unknown ()");
            Str_Ins(&IDStr, U32_To_Str(&TempStr, ID), 9);
            Str_Free(TempStr);
        }
    }
    
    switch(Severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        {
            Str_CatC(&SeverityStr, "-------- OpenGL Message - Error (High) -------------------------");
        } break;
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            Str_CatC(&SeverityStr, "-------- OpenGL Message - Error (Medium) -----------------------");
        } break;
        case GL_DEBUG_SEVERITY_LOW:
        {
            Str_CatC(&SeverityStr, "-------- OpenGL Message - Error (Low) --------------------------");
        } break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            Str_CatC(&SeverityStr, "-------- OpenGL Message - Notification -------------------------");
        } break;
        default:
        {
            Str_CatC(&SeverityStr, "-------- OpenGL Message - Unknown Severity () --------");
            
            str NumBuf = U32_To_Str(NULL, Severity);
            u32 NumLen = Str_Len(NumBuf);
            Str_Ins(&SeverityStr, NumBuf, 44);
            Str_Free(NumBuf);
            
            str HyphenStr = Str_Create(NULL, NULL, 10 - NumLen); // 10 - NumLen = Number of hyphens to add
            Mem_Set(HyphenStr, '-', Str_Len(HyphenStr));
            Str_Cat(&SeverityStr, HyphenStr);
            Str_Free(HyphenStr);
        }
    }
    
    Str_CatC(&MessageStr, Message);
    
    Platform_Print_DEBUG(SeverityStr);
    Platform_Print_DEBUG(SourceStr);
    Platform_Print_DEBUG(TypeStr);
    Platform_Print_DEBUG(IDStr);
    Platform_Print_DEBUG(MessageStr);
    Platform_Print_DEBUG("\n----------------------------------------------------------------\n\n");
    
    Str_Free(SeverityStr);
    Str_Free(SourceStr);
    Str_Free(TypeStr);
    Str_Free(IDStr);
    Str_Free(MessageStr);
}
#endif