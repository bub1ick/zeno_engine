#pragma once

//  standard lib
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

//  windows lib
#if defined(_WIN32) || defined(_WIN64)
//  Switches to turn off some unnecessary APIs
#    define NOCOMM
#    define NOGDI
#    define NOMINMAX
#    define NOKANJI
#    define NOMCX

//  HACK: for some reason we have to define this to use IID_* macros, otherwise they're undefined and do not compile.
#    define INITGUID

#    include <windows.h>
#    include <d3d11_4.h>
#    include <dxgi1_6.h>
#endif

//  engine libs
#include "zenocore/zenocore.hpp"
