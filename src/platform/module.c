/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// #ifndef INCLUDE_HEADER
#   include <shared.h>
// #endif

#include <util/module.h>
#include <game/module.h>
#include <platform/module.h>

#include <platform/opengl/opengl.c>

#ifdef _WIN32
// #   include <platform/win32/win32.c>
#   include <platform/win32/loader.c>
#endif