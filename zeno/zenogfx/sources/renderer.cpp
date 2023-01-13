#include <zeno.hpp>
#include <assert.h>

namespace zeno::gfx
{

renderer_t::renderer_t(const sys::window_t& in_window)

try : m_window(in_window), m_dx11(in_window)
{
}

catch (const dx_exception_t& dx_ex)
{
    std::cerr << dx_ex.get_error_message() << std::endl;
}

renderer_t::~renderer_t() { }

void renderer_t::update()
{
    m_dx11.update(m_window, m_get_delta_time());
}

float renderer_t::m_get_delta_time()
{
    m_current_time_ms = GetTickCount64();

    if (m_start_time_ms == 0)
        m_start_time_ms = m_current_time_ms;  //  save tick count into the buffer

    return (m_current_time_ms - m_start_time_ms) / 1000.f;  //  return in seconds
}
}  //  namespace zeno::gfx