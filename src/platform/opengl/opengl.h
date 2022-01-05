/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PLATFORM_OPENGL_OPENGL_H_
#define PLATFORM_OPENGL_OPENGL_H_

#define PLATFORM_OPENGL__OPENGL_DEBUG__EXPORTS \
    FUNC(void _API_ENTRY, GL_DebugCallback, u32 Source, u32 Type, u32 ID, u32 Severity, s32 Length, chr *Message, vptr UserParam) \

#define PLATFORM_OPENGL__OPENGL_DEBUG__FUNCS \

#define PLATFORM_OPENGL__OPENGL__EXPORTS \

#define PLATFORM_OPENGL__OPENGL__FUNCS \

#define PLATFORM__GL__EXPORTS \
    PLATFORM_OPENGL__OPENGL_DEBUG__EXPORTS \
    PLATFORM_OPENGL__OPENGL__EXPORTS \

#define PLATFORM__GL__FUNCS \
    PLATFORM_OPENGL__OPENGL_DEBUG__FUNCS \
    PLATFORM_OPENGL__OPENGL__FUNCS \

#endif