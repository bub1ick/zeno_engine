///  This file is intended to be used for engine-defined macros.
///  It should be always included in all header files of engine modules.

#pragma once

//  linkage
#if defined(ZENO_WINDOWS)
#    if defined(ZENO_LIB)
#        define ZENO_API __declspec(dllexport)
#    else
#        define ZENO_API __declspec(dllimport)
#    endif
#else
#    define ZENO_API
#endif

//  various helper macros that ease development for windows
#if defined(ZENO_WINDOWS)
#    if defined(interface)
#        undef interface
#    endif
//  a useful macro that allows for easy release of com objects with checking and nullifying
//  typical usage: destructors
#    define COM_RELEASE(interface)    \
        {                             \
            if (interface)            \
            {                         \
                interface->Release(); \
                interface = nullptr;  \
            }                         \
        }

//  a useful macro that handles reference counting when copying a com interface pointer
#    define COM_COPY(to, from) \
        {                      \
            from->AddRef();    \
            to = from;         \
        }

#    define COM_MOVE(to, from) \
        {                      \
            COM_RELEASE(to);   \
            to   = from;       \
            from = nullptr;    \
        }

#    define interface __STRUCT__
#endif

#if !defined(ZENO_LIB)
#    include "zeno_user_defines.hpp"
#endif