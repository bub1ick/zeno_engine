#include <zeno.hpp>

namespace zeno::gfx
{
dx_exception_t::dx_exception_t(std::string_view in_error_string, HRESULT in_result, category_e exception_category)
    : m_result {in_result}
{
    category_format(exception_category);
    m_error_message += in_error_string;
}

void dx_exception_t::operator=(const dx_exception_t& in_that)
{
    this->m_error_message = in_that.get_error_message();
}

std::string dx_exception_t::get_error_message() const
{
    std::stringstream error_code;
    error_code << std::hex << m_result;  //  convert result value to hex format
    return m_error_message + "\n\t-Error code:" + error_code.str();
}

void dx_exception_t::category_format(category_e exception_category)
{
    switch (exception_category)
    {
        case category_e::e_d3d11:
            m_error_message = "Direct3D exception caught:\n\t-";
            break;

        case category_e::e_dxgi:
            m_error_message = "DXGI exception caught:\n\t-";
            break;

        default:
            m_error_message = "Unspecified DirectX exception caught:\n\t-";
            break;
    }
}

}  //  namespace zeno::gfx
