#pragma once

namespace zeno
{
class ZENO_API engine_t
{
public:

    engine_t(
        const char* window_name, int32_t pos_x = CW_USEDEFAULT, int32_t pos_y = CW_USEDEFAULT,
        int32_t size_x = CW_USEDEFAULT, int32_t size_y = CW_USEDEFAULT
    );

    ~engine_t();

    void run();

private:
    renderer_t* _renderer;

    std::string _command_line;
};
}  //  namespace zeno