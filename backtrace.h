#pragma once

// stuff for back tracing and getting a call stack (ideally)

#include <exception>
#include <vector>

#include "baseHeaders.h"
//#include <windows.h>

#pragma warning(disable : 4091)
#include <dbghelp.h>
#pragma comment(lib, "DbgHelp.lib")
#pragma warning(default : 4091)

#include "Log.h"

// after we convert an seh exception to a c++ exception
class sehException : std::exception
{
public:
    explicit sehException (unsigned int n, EXCEPTION_POINTERS *e)
        : errCode (n), exceptPtrs (e)
    {
    }

    unsigned int        errCode;
    EXCEPTION_POINTERS *exceptPtrs;
};

//#define _TRYCATCH

#ifdef _MSC_VER

LONG WINAPI UnhandledSehExceptionHandler (EXCEPTION_POINTERS *e);

#endif

// stuff for try and catch
// ugly but could improve speed later on
#ifdef _TRYCATCH

// defined in the cpp
void sehTranslator (unsigned int code, EXCEPTION_POINTERS *e);

void terminateHandler ();

void unexpectedHandler ();

#define _TRY try
#undef _CATCH // undefine this _CATCH
#define _CATCH catch (...)
#define _CATCH_SEH catch (sehException e)
#define _CATCH_SEH_REPORT_ERROR(Class, funcName)                                                                                                 \
    _CATCH_SEH                                                                                                                                   \
    {                                                                                                                                            \
        Log::Error ("An SEH exception\n(name: %s | id=0x%X)\nwas raised from %s::%s", "UNKNOWN", e.errCode, typeid (*Class).name (), #funcName); \
    }
#define _INSTALL_SEH_TRANSLATOR() _set_se_translator (&sehTranslator) // this must be called within a try statement

#else
// dont enable these (maybe compiling on clang???)

#include "../SDK/backtrace_off.h"

#endif

// begin runtime one-time asserts

#define __RASSERT(x, msg)                                                                                          \
    static bool oneTime = false;                                                                                   \
    if (!x && oneTime == false) {                                                                                  \
        oneTime = true;                                                                                            \
        Log::Error ("line: %d function: %s file: %s \n error: %s", __LINE__, __FUNCSIG__, __FILE__, " ## msg ## ") \
    }

#define RASSERT(x, msg)    \
    do {                   \
        __RASSERT (x, msg) \
    } while (0)

#define RASSERTA(x, msg, action) \
    do {                         \
        __RASSERT (x, msg)       \
        else                     \
        {                        \
            action               \
        }                        \
    } while (0)