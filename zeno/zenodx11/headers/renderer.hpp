#pragma once

#include "renderer/window.hpp"

namespace zeno
{
class ZENO_API renderer_t
{
public:
    renderer_t(
        const char* window_name, HINSTANCE instance = nullptr, int32_t pos_x = CW_USEDEFAULT, int32_t pos_y = CW_USEDEFAULT,
        int32_t size_x = CW_USEDEFAULT, int32_t size_y = CW_USEDEFAULT, int32_t show_command = SW_SHOWDEFAULT
    );

    renderer_t(const renderer_t& other);

    renderer_t& operator=(const renderer_t& other);

    ~renderer_t();

    void      initialize();

    window_t* get_window() const noexcept { return _window; };

private:
    window_t* _window;
};
}  //  namespace zeno
