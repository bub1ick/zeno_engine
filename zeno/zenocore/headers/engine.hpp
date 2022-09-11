#pragma once

namespace zeno::core
{

class ZENO_API engine_t
{
public:
    engine_t(
        const char* in_window_name, int32_t in_pos_x = 0, int32_t in_pos_y = 0, int32_t in_size_x = 0,
        int32_t in_size_y = 0
    );

    void run();

private:
    std::string   m_command_line = "";

    bool          m_running = true;

    std::string   load_shaders(const char* shader_path) const;
};
}  //  namespace zeno::core