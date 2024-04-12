#pragma once

namespace zeno::gfx
{
class dx_exception_t
{
public:
    enum category_e
    {
        e_unspecified,
        e_d3d11,
        e_dxgi
    };

    dx_exception_t(std::string_view in_error_string, HRESULT in_result, category_e exception_category = e_unspecified);
    ~dx_exception_t() = default;
    void        operator=(const dx_exception_t& in_that);
    std::string get_error_message() const;

private:
    void        category_format(category_e exception_category);

    std::string m_error_message;
    HRESULT     m_result;
};

}  //  namespace zeno::gfx