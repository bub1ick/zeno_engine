#include <zeno.hpp>

namespace zeno::core
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    m_window_ptr = plat::create_platform_window(in_window_name, in_pos_x, in_pos_y, in_size_x, in_size_y);

    m_renderer_ptr = new dx11::renderer_t(m_window_ptr);
}

void engine_t::run()
{
    plat::platform_loop(
        [this]
        {

        }
    );
}

}  //  namespace zeno::core
