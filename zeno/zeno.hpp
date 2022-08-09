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

//  DirectX headers
#    include <d3d11_4.h>
#    include <d2d1_3.h>
#    include <d2d1_3helper.h>
#    include <dxgi1_6.h>
#endif

#include "zeno_defines.hpp"

//  engine libs
#include "zenodx11/zenodx11.hpp"

#include "zenocore/zenocore.hpp"
