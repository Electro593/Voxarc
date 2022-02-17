/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(DEFAULT)
    #define DEFAULT(...)
#endif
#if !defined(EXTERN)
    #define EXTERN DEFAULT
#endif
#if !defined(INTERN)
    #define INTERN DEFAULT
#endif
#if !defined(EXPORT)
    #define EXPORT DEFAULT
#endif
#if !defined(IMPORT)
    #define IMPORT DEFAULT
#endif

X

#undef IMPORT
#undef EXPORT
#undef INTERN
#undef EXTERN
#undef DEFAULT
#undef X