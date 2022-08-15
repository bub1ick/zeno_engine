add_library(zenoplat STATIC)

add_compile_definitions(
    zenoplat
    PUBLIC
    ZENO_LIB
)

target_include_directories(
    zenoplat
    PRIVATE
    "../"
)

target_sources(
    zenoplat
    PUBLIC
    "source/win32/win32_window.cpp"
    "source/win32/win32_platform.cpp"
)