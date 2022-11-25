#pragma once

namespace zeno::dx11
{

struct dxgi_t
{
    IDXGIFactory7* factory;
    IDXGIDevice4*  device;
    IDXGIAdapter4* graphics_card;
    IDXGIOutput6*  monitor;
};

class ZENO_API renderer_t
{
public:
    renderer_t(HWND window_handle);

    void update();

private:
    ///  @brief search for all dxgi adapters (graphics cards) in the system
    ///  @param out_adapters reference to a vector to hold found adapters
    void                    get_all_available_adapters(std::vector<IDXGIAdapter4*>& out_adapters);

    ///  @brief create direct3d and dxgi device and its context
    ///  @return true on success, false on error
    bool                    create_device();

    const D3D_FEATURE_LEVEL m_feature_level;

    dxgi_t                  m_dxgi;

    ID3D11Device5*          m_device;

    IDXGISwapChain4*        m_swapchain;

    ID3D11DeviceContext4*   m_device_context;

    HRESULT                 m_result;
};
}  //  namespace zeno::dx11