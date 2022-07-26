#pragma once

#if defined(ZENO_WINDOWS)
#    if defined(ZENO_LIB)
#        define ZENO_API __declspec(dllexport)
#    else
#        define ZENO_API __declspec(dllimport)
#    endif
#endif