/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <shared.h>

#include <util/module.h>
#include <game/module.h>
#include <platform/module.h>

#if 0 && _PLATFORM_WINDOWS
#   include <d3d/d3d.c>
#else
#   include <platform/opengl/opengl.c>
#endif

#if _LOADER_WIN32
#   include <platform/win32/loader.c>
#endif