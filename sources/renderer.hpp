#pragma once

#include "dx11.hpp"

namespace zeno::gfx
{
class ZENO_API renderer_t
{
public:
    renderer_t(const sys::window_t& in_window);

    void update(const sys::window_t& in_window);

    void set_mesh(const mesh_t* in_mesh) { m_dx11.set_mesh(in_mesh); };

private:
    ///  @brief handles DirectX11
    dx11_t  m_dx11;
    ///  @brief holds the results of direct3d and dxgi functions
    HRESULT m_result;

    double  m_get_delta_time(std::chrono::time_point<std::chrono::high_resolution_clock> in_start);
};
}  //  namespace zeno::gfx