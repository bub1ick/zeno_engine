#pragma once

namespace zeno
{
namespace plat
{
    window_t* create_platform_window(
        const char* in_window_title, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y
    );

    void platform_loop(const std::function<void()>& in_engine_loop);
}
}  //  namespace zeno
