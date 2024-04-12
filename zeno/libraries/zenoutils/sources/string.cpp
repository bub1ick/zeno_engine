#include <zeno.hpp>

namespace zeno::utils
{
std::string utf16_to_utf8(std::wstring& in_utf16_string)
{
    // we pass just the wide char data to get the size we need to store the utf-8 data
    size_t size_needed = WideCharToMultiByte(CP_UTF8, 0, in_utf16_string.data(), in_utf16_string.size(), 0, 0, 0, 0);

    std::string converted(size_needed, 0);

    WideCharToMultiByte(CP_UTF8, 0, in_utf16_string.data(), static_cast<int32_t>(in_utf16_string.size()), converted.data(), static_cast<int32_t>(size_needed), 0, 0);

    return converted;
}
}  //  namespace zeno::utils