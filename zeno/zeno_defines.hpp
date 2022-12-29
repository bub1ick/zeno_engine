///  This file is intended to be used for engine-defined macros.
///  It should be always included in all header files of engine modules.

#pragma once

#if defined(ZENO_WINDOWS)
#    if defined(ZENO_LIB)
#        define ZENO_API __declspec(dllexport)
#    else
#        define ZENO_API __declspec(dllimport)
#    endif
#else
#    define ZENO_API
#endif

#if !defined(ZENO_LIB)
#    include "zeno_user_difines.hpp"
#endif