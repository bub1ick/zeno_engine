#include <zeno.hpp>
#include <assert.h>

namespace zeno::gfx
{

renderer_t::renderer_t(const sys::window_t& in_window)

try : m_dx11(in_window)
{
}

catch (const dx_exception_t& dx_ex)
{
    std::cerr << dx_ex.get_error_message() << std::endl;
}

renderer_t::~renderer_t() { }

void renderer_t::update(const sys::window_t& in_window)
{
    using namespace std::chrono;
    static time_point<high_resolution_clock> start_rendering = high_resolution_clock::now();

    m_dx11.update(in_window, m_get_delta_time(start_rendering));
}

double renderer_t::m_get_delta_time(std::chrono::time_point<std::chrono::high_resolution_clock> in_start)
{
    using namespace std::chrono;

    time_point<high_resolution_clock> now                   = high_resolution_clock::now();
    duration<double>                  delta_time_in_seconds = duration_cast<duration<double>>(now - in_start);

    return delta_time_in_seconds.count();
}
}  //  namespace zeno::gfx