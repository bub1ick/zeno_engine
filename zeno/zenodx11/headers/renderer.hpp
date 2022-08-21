#pragma once

namespace zeno::dx11
{

struct ZENO_API render_data_t
{
};

class ZENO_API renderer_t
{
public:
    renderer_t(plat::window_t* in_window_ptr);

private:
    HWND                    m_window_handle;

    ID3D11Device5*          m_d3d11_device;

    ID3D11DeviceContext4*   m_d3d11_device_context;

    const D3D_FEATURE_LEVEL m_feature_levels [2] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};

    
};


}  //  namespace zeno::dx11