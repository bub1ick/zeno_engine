#pragma once

namespace zeno::dx11
{

///  @brief struct holding all rarely used/not essential dxgi interfaces and data
struct dxgi_t
{
    IDXGIFactory7*   factory;
    IDXGIDevice4*    device;
    IDXGIAdapter4*   graphics_card;
    IDXGIOutput6*    monitor;
    DXGI_MODE_DESC1* display_modes;
};

class ZENO_API renderer_t
{
public:
    renderer_t(HWND in_window_handle);
    ~renderer_t();

    void update();

private:
    ///  @brief search for all dxgi adapters (graphics cards) in the system
    ///  @param out_adapters reference to a vector to hold found adapters
    void                     get_all_available_adapters(std::vector<IDXGIAdapter4*>& out_adapters);

    ///  @brief this fucntion gets all the display modes available to user
    ///  @return the mode with the best resolution and refresh rate
    DXGI_MODE_DESC1          get_best_display_mode();

    ///  @brief create direct3d and dxgi device and its context
    ///  @return true on success, false on error
    bool                     create_device();

    ///  @brief  create a dxgi swapchain to render to
    ///  @param in_fullscreen_display_mode default fullscreen resolution, refresh rate etc.
    ///  @param in_windowed whether to create a windowed swapchain or a fullscreen one
    ///  @param in_window_handle a handle to a win32 window to bind swapchain to
    ///  @return true on success, false on error
    bool                     create_swapchain(DXGI_MODE_DESC1* in_fullscreen_display_mode, bool in_windowed, HWND in_window_handle);

    ///  @brief shows which version of direct3d to request
    const D3D_FEATURE_LEVEL  m_feature_level;

    ///  @brief holds all dxgi related interfaces
    dxgi_t                   m_dxgi;

    ///  @brief the direct3d device
    ID3D11Device5*           m_device;

    ///  @brief the direct3d device context
    ID3D11DeviceContext4*    m_device_context;

    ///  @brief the swapchain we render to
    IDXGISwapChain4*         m_swapchain;

    ///  @brief the direct3d render target view
    ID3D11RenderTargetView1* m_render_target_view;

    ///  @brief holds the results of direct3d and dxgi functions
    HRESULT                  m_result;

    ///  @brief a handle to a win32 window
    HWND                     m_window_handle;

    ///  @brief describes how data is organized before being passed to vertex shader
    ID3D11InputLayout*       m_vertex_input_layout;

    ///  @brief an array of vertecies described by 3 coordinates on the screen
    ID3D11Buffer*            m_current_vertex_buffer;

    uint32_t                 m_vertex_stride;

    uint32_t                 m_vertex_offset;

    uint32_t                 m_vertex_count;

    //  vertex and pixel shaders
    ID3D11VertexShader*      m_vs = nullptr;
    ID3D11PixelShader*       m_ps = nullptr;
};
}  //  namespace zeno::dx11