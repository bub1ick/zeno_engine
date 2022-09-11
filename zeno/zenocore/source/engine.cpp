#include <zeno.hpp>

namespace zeno::core
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    std::cout << "running\n";
}

void engine_t::run()
{
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
