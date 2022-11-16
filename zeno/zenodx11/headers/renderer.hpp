#pragma once

namespace zeno::dx11
{

struct dxgi_t
{
    IDXGIFactory7* factory;
    IDXGIDevice4*  device;
    IDXGIAdapter4* video_card;
    IDXGIOutput6*  monitor;
};

class ZENO_API renderer_t
{
public:
    renderer_t();

    void update();

private:

    /// @brief search for all dxgi adapters (graphics cards) in the system
    /// @param out_adapters reference to a vector to hold found adapters
    void get_all_available_adapters(std::vector<IDXGIAdapter4*>& out_adapters);

    const D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL_11_1;

    dxgi_t             m_dxgi;

    HRESULT m_result;
};
}  //  namespace zeno::dx11