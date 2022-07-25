#pragma once

// standard lib
#include <cstdint>
#include <cstring>
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>

// windows lib
#if defined(_WIN32) || defined(_WIN64)
#   include <windows.h>
#   include <d3d11_4.h>
#endif

// engine libs
#include "zenocore/zenocore.hpp"

