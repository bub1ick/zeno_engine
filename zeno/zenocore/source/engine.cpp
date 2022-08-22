#include <zeno.hpp>

namespace zeno::core
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER);
}

void engine_t::run()
{
}

}  //  namespace zeno::core
