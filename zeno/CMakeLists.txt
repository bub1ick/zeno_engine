# The engine CMakeLists.txt file. Defines build rules for all engine modules and libs.

add_compile_definitions(ZENO_LIB)

if(${WIN32})
    add_subdirectory(zenodx11)
endif()

add_subdirectory(zenocore)