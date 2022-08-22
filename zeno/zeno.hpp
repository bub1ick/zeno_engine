#pragma once

//  standard lib
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

//  Windows lib
#if defined(ZENO_WINDOWS)
//  Switches to turn off some unnecessary APIs
#    define NOCOMM
#    define NOGDI
#    define NOMINMAX
#    define NOKANJI
#    define NOMCX

//  HACK: for some reason we have to define this to use IID_* macros, otherwise they're undefined and do not compile.
#    define INITGUID

//  general Windows header
#    include <windows.h>
#endif

#include <SDL.h>

#include "zeno_defines.hpp"

//  engine libs
#include "zenocore/zenocore.hpp"
