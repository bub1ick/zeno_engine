# The engine CMakeLists.txt file. Defines build rules for all engine modules and libs.

if(${WIN32})
    add_subdirectory(zenodx11)
endif()

add_subdirectory(zenocore)