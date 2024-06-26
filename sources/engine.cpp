#include "zeno.hpp"

namespace zeno
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
    : m_window(in_window_name, in_pos_x, in_pos_y, in_size_x, in_size_y),
      m_renderer(m_window),
      m_gltf_loader()
{
    const gfx::mesh_t* mesh = m_gltf_loader.load("model.gltf");
    m_renderer.set_mesh(mesh);
}

int32_t engine_t::run()
{
    auto engine_loop = [this]() -> bool
    {
        //  TODO: handle all the engine systems

        m_renderer.update(m_window);

        return m_game_is_done;
    };

    m_window.loop(engine_loop);
    
    return 0;
}

}  //  namespace zeno
