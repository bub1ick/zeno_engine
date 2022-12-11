#include <zeno.hpp>

namespace zeno::dx11
{
dxgi_exception_t::dxgi_exception_t(std::string_view in_error_string, HRESULT in_result)
    : m_error_message(in_error_string),
      m_result {in_result}
{
}

dxgi_exception_t::~dxgi_exception_t()
{
    //  nothing to deinitialize
}

dxgi_exception_t& dxgi_exception_t::operator=(const dxgi_exception_t& in_that)
{
    this->m_error_message = in_that.get_error_message();
    return *this;
}

std::string_view dxgi_exception_t::get_error_message() const
{
    std::stringstream error_code;
    error_code << std::hex << m_result;  //  convert result value to hex format
    return m_error_message + "\t" + error_code.str();
}

}  //  namespace zeno::dx11
