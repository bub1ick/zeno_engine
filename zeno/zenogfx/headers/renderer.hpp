#pragma once

#include "components/dx11.hpp"

namespace zeno::gfx
{
class ZENO_API renderer_t
{
public:
    renderer_t(const sys::window_t& in_window);
    ~renderer_t();

    void update(const sys::window_t& in_window);

private:
    ///  @brief handles DirectX11
    dx11_t   m_dx11;
    ///  @brief holds the results of direct3d and dxgi functions
    HRESULT  m_result;

    float    m_get_delta_time();
    uint64_t m_start_time_ms = 0;  //  time buffer (ms)
    uint64_t m_current_time_ms;    //  time since system started (ms)
};
}  //  namespace zeno::gfx