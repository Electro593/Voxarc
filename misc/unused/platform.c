/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "shared.h"

#include "platform/module.h"
#include "game/module.h"

#include "platform/opengl.c"

#ifdef _LOADER_WIN32
    #include "platform/win32/win32.h"
    
    #include "platform/win32/win32.c"
#endif