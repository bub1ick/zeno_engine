#include <zeno.hpp>

namespace zeno::utils
{
std::string utf16_to_utf8(std::wstring& utf16_string)
{
    size_t      size_needed = WideCharToMultiByte(  //  get the size of the adapter name string
        CP_UTF8,
        0,
        &utf16_string [0],
        utf16_string.size(),
        0,
        0,
        0,
        0
    );

    std::string result(size_needed, 0);  //  initialize a utf-8 string for storing adapter name
    WideCharToMultiByte(                 //  translate wchar_t to char
        CP_UTF8,
        0,
        &utf16_string [0],
        (int) utf16_string.size(),
        &result [0],
        size_needed,
        0,
        0
    );

    return result;
}
}