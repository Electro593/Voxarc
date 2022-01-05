/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef X_MACRO
#   ifndef IPROC
#       define IPROC(...)
#   endif
#   ifndef EPROC
#       define EPROC(...)
#   endif
#   ifndef IFUNC
#       define IFUNC(...)
#   endif
#   ifndef EFUNC
#       define EFUNC(...)
#   endif
    
    X_MACRO
    
#   undef IPROC
#   undef EPROC
#   undef IFUNC
#   undef EFUNC
#   undef X_MACRO
#endif