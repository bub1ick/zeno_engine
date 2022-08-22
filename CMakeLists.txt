cmake_minimum_required(VERSION 3.22)

## Project declaration
project(
    zeno_engine_cmake
    VERSION 0.1.0
    DESCRIPTION "\"zeno_engine\" is a collection of tools and libraries to create interactive expreriences."
    LANGUAGES CXX
)


## Language settings:
# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Enable some of the standard C++ features, such as literals for logical operators.
if(MSVC)
    add_compile_options(/permissive-)
    add_compile_options(/utf-8)
endif()


## Build settings:
# By default, use dynamic libs for modules, unless it's not a module
set(BUILD_SHARED_LIBS ON)

# We can exclude zeno sample from the build.
option(ZENO_DO_BUILD_SAMPLE "Whether or not build the sample project provided." OFF)

# SDL paths
set(ZENO_SDL_INCLUDE_PATH ${CMAKE_HOME_DIRECTORY} CACHE PATH "Path to SDL include directory")
set(ZENO_SDL_INCLUDE_CONFIG_PATH ${CMAKE_HOME_DIRECTORY} CACHE PATH "Path to SDL include-config directory")
set(ZENO_SDL_LIB_PATH ${CMAKE_HOME_DIRECTORY} CACHE PATH "Path to SDL library directory")

# glm path
set(ZENO_GLM_INCLUDE_PATH ${CMAKE_HOME_DIRECTORY} CACHE PATH "Path to GLM include directory")

if(${WIN32})
    add_compile_definitions("ZENO_WINDOWS")
elseif(${UNIX} AND NOT ${APPLE})
    add_compile_definitions("ZENO_LINUX")
elseif(${APPLE})
    message(FATAL_ERROR "Currently support only for Windows and Linux is planned.")
endif()


link_directories(AFTER ${ZENO_SDL_LIB_PATH})

include_directories(AFTER ${ZENO_SDL_INCLUDE_PATH} ${ZENO_GLM_INCLUDE_PATH} ${ZENO_SDL_INCLUDE_CONFIG_PATH})

## Add directories:

# Build script for the zeno engine.
add_subdirectory(zeno)

# At last build the sample to test the engine.
if(${ZENO_DO_BUILD_SAMPLE})
    add_subdirectory(sample)
    add_dependencies(sample zenocore)
endif()
