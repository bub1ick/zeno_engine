#pragma once

#include "../window.hpp"

namespace zeno::plat::win32
{
class ZENO_API win32_window_t final : public window_t
{
public:
    win32_window_t(const char* title, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y, HINSTANCE instance);

    HWND get_handle() const noexcept { return m_handle; };

private:
    HINSTANCE      m_instance;

    WNDCLASSEXA    m_class;

    ATOM           m_class_atom;

    HWND           m_handle;

    static LRESULT m_window_procedure(HWND window, uint32_t message_id, WPARAM w_param, LPARAM l_param);
};

}  //  namespace zeno::plat::win32
