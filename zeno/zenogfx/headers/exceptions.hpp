#pragma once

namespace zeno::gfx
{
class dxgi_exception_t
{
public:
    dxgi_exception_t(std::string_view in_error_string, HRESULT in_result);
    ~dxgi_exception_t();
    void             operator=(const dxgi_exception_t& in_that);
    std::string_view get_error_message() const;

protected:
    std::string m_error_message;
    HRESULT     m_result;
};

}  //  namespace zeno::gfx