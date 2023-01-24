#pragma once

namespace zeno::gfx
{

class dxgi_component_t
{
public:
    dxgi_component_t();
    dxgi_component_t(const dxgi_component_t& that);
    dxgi_component_t(dxgi_component_t&& that);
    dxgi_component_t& operator=(const dxgi_component_t& that);
    dxgi_component_t& operator=(dxgi_component_t&& that);
    ~dxgi_component_t();

    void             initialize_device(ID3D11Device5* const in_d3d11_device);

    void             create_swapchain(ID3D11Device5* in_device, bool in_windowed, const sys::window_t& in_window);

    IDXGISwapChain4* get_swapchain() const noexcept { return m_swapchain; };

    IDXGIAdapter4*   get_graphics_card() const noexcept { return m_graphics_card; };

private:
    ///  @brief search for all dxgi adapters (graphics cards) in the system
    ///  @param out_adapters reference to a vector to hold found adapters
    void                         m_get_all_available_adapters();

    ///  @brief this function gets all the display modes available to user
    ///  @return the mode with the best resolution and refresh rate
    DXGI_MODE_DESC1              m_get_best_display_mode();

    HRESULT                      m_result;

    IDXGIFactory7*               m_factory;

    IDXGIDevice4*                m_device;

    std::vector<IDXGIAdapter4*>  m_adapters;

    IDXGIAdapter4*               m_graphics_card;

    IDXGIOutput6*                m_monitor;

    //  DXGI_MODE_DESC1*            m_display_modes;
    std::vector<DXGI_MODE_DESC1> m_display_modes;

    IDXGISwapChain4*             m_swapchain;
};

}  //  namespace zeno::gfx
