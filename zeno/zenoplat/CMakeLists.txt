add_library(zenoplat STATIC)

target_include_directories(
    zenoplat
    PRIVATE
    "../"
)

target_sources(
    zenoplat
    PUBLIC
    "source/platform/windows.cpp"
)