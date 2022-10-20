#include <zeno.hpp>

namespace zeno
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
    : m_window(in_window_name, in_pos_x, in_pos_y, in_size_x, in_size_y)
{
}

int32_t engine_t::run()
{
    auto engine_loop = [this]() -> bool
    {
        // handle all the engine systems

        m_renderer;
        return m_game_is_done;
    };

    m_window.loop(engine_loop);

    return 0;
}

std::string engine_t::load_shaders(const char* shader_path) const
{
    std::ifstream shader_file(shader_path, std::ios::in);
    std::string   shader_string;

    if (shader_file.is_open())
    {
        std::string shader_line = "";
        while (not shader_file.eof())
        {
            std::getline(shader_file, shader_line);

            shader_string.append(shader_line + "\n");
        }
    }

    return shader_string;
}

}  //  namespace zeno::core
