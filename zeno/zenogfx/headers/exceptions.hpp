#pragma once

namespace zeno::gfx
{
class dx_exception_t
{
public:
    enum category_e
    {
        unspecified,
        d3d11,
        dxgi
    };

    dx_exception_t(std::string_view in_error_string, HRESULT in_result, category_e exception_category);
    ~dx_exception_t() = default;
    void        operator=(const dx_exception_t& in_that);
    std::string get_error_message() const;

private:
    void category_format(category_e exception_category);

    std::string m_error_message;
    HRESULT     m_result;
};

}  //  namespace zeno::gfx