#include "zeno.hpp"

namespace zeno::gfx
{

dxgi_t::dxgi_t()
{
    m_result = CreateDXGIFactory2(0, IID_IDXGIFactory7, reinterpret_cast<void**>(&m_factory));
    if (FAILED(m_result))
        throw dx_exception_t("Failed on CreateDXGIFactory2!", m_result, dx_exception_t::e_dxgi);

    m_get_all_available_adapters();
    m_graphics_card = m_adapters [0];
    m_result        = m_graphics_card->EnumOutputs(0, reinterpret_cast<IDXGIOutput**>(&m_monitor));
    if (FAILED(m_result))
        throw dx_exception_t("Failed on EnumOutputs!", m_result, dx_exception_t::e_dxgi);
}

dxgi_t::dxgi_t(const dxgi_t& that)
{
    *this = that;
}

dxgi_t::dxgi_t(dxgi_t&& that)
{
    *this = std::move(that);
}

dxgi_t& dxgi_t::operator=(const dxgi_t& that)
{
    if (this == &that)
        return *this;

    COM_COPY(m_factory, that.m_factory);
    COM_COPY(m_device, that.m_device);
    COM_COPY(m_graphics_card, that.m_graphics_card);
    COM_COPY(m_monitor, that.m_monitor);
    COM_COPY(m_swapchain, that.m_swapchain);

    m_result        = that.m_result;
    m_display_modes = that.m_display_modes;

    //  erase existing adapters
    for (auto adapter : m_adapters)
        COM_RELEASE(adapter);
    m_adapters.clear();

    //  add new adapters
    for (auto adapter : that.m_adapters)
    {
        adapter->AddRef();
        m_adapters.push_back(adapter);
    }

    return *this;
}

dxgi_t& dxgi_t::operator=(dxgi_t&& that)
{
    if (this == &that)
        return *this;

    COM_MOVE(m_factory, that.m_factory);
    COM_MOVE(m_device, that.m_device);
    COM_MOVE(m_graphics_card, that.m_graphics_card);
    COM_MOVE(m_monitor, that.m_monitor);
    COM_MOVE(m_swapchain, that.m_swapchain);

    m_result      = that.m_result;
    that.m_result = 0;

    m_display_modes = that.m_display_modes;
    that.m_display_modes.clear();

    //  erase existing adapters
    for (auto adapter : m_adapters)
        COM_RELEASE(adapter);
    m_adapters.clear();

    //  add new adapters
    for (auto adapter : that.m_adapters)
    {
        m_adapters.push_back(adapter);
        adapter = nullptr;
    }

    return *this;
}

dxgi_t::~dxgi_t()
{
    COM_RELEASE(m_factory)
    COM_RELEASE(m_device)
    COM_RELEASE(m_graphics_card)
    COM_RELEASE(m_monitor)
    COM_RELEASE(m_swapchain)

    for (auto adapter : m_adapters)
        COM_RELEASE(adapter);
}

void dxgi_t::initialize_device(ID3D11Device5* const in_d3d11_device)
{
    m_result = in_d3d11_device->QueryInterface<IDXGIDevice4>(&m_device);
    if (FAILED(m_result))
        throw dx_exception_t("Failed to get DXGI device!", m_result, dx_exception_t::e_dxgi);
}

void dxgi_t::create_swapchain(ID3D11Device5* in_device, bool in_windowed, const sys::window_t& in_window)
{
    DXGI_MODE_DESC1             fullscreen_display_mode = m_get_best_display_mode();

    //  how can the swapchain be used
    DXGI_USAGE                  buffer_usage    = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    uint32_t                    swapchain_flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    sys::window_t::dimentions_t size = in_window.get_dimentions();

    //  info on the dxgi format topic:
    //      - https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/converting-data-color-space
    //      - https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-24-importance-being-linear
    DXGI_SWAP_CHAIN_DESC1       swapchain_descriptor {};
    swapchain_descriptor.Width       = in_windowed ? size.width : fullscreen_display_mode.Width;    //  set the swapchain size to the native resolution
    swapchain_descriptor.Height      = in_windowed ? size.height : fullscreen_display_mode.Height;  //  set the swapchain size to the native resolution
    swapchain_descriptor.Format      = DXGI_FORMAT_R16G16B16A16_FLOAT;  //  use linear format that is supported by flip mode swap chain
    swapchain_descriptor.Stereo      = false;                           //  true is for VR-like things (two screens rendering at the same time)
    swapchain_descriptor.SampleDesc  = {1, 0};                          //  disable MSAA
    swapchain_descriptor.BufferUsage = buffer_usage;
    swapchain_descriptor.BufferCount = 2;  //  we want the front and the back buffer (sums up to 2)
    swapchain_descriptor.Scaling     = DXGI_SCALING_NONE;
    swapchain_descriptor.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;  //  use flip model instead of bitblt model (more in MSDN)
    swapchain_descriptor.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;       //  FIXME: IDK what is this
    swapchain_descriptor.Flags       = swapchain_flags;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchain_fullscreen_descriptor {};
    swapchain_fullscreen_descriptor.RefreshRate      = fullscreen_display_mode.RefreshRate;       //  get the highest refresh rate from the monitor
    swapchain_fullscreen_descriptor.ScanlineOrdering = fullscreen_display_mode.ScanlineOrdering;  //  get scanline ordering from the monitor
    swapchain_fullscreen_descriptor.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;             //  never use scaling
    swapchain_fullscreen_descriptor.Windowed         = in_windowed;                               //  start the application in specified mode

    m_result = m_factory->CreateSwapChainForHwnd(
        in_device, in_window.get_handle(), &swapchain_descriptor, &swapchain_fullscreen_descriptor, nullptr, reinterpret_cast<IDXGISwapChain1**>(&m_swapchain)
    );
    if (FAILED(m_result))
    {
        throw dx_exception_t("Failed to create Swap Chain!", m_result, dx_exception_t::e_dxgi);
    }
}

void dxgi_t::m_get_all_available_adapters()
{
    IDXGIAdapter4* adapter;  //  used to temporary store found adapter

    for (uint8_t index = 0;; index++)
    {
        static bool could_find = false;  //  to check wether there are adapters left

        could_find = m_factory->EnumAdapterByGpuPreference(index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_IDXGIAdapter4, reinterpret_cast<void**>(&adapter))
                     != DXGI_ERROR_NOT_FOUND;  //  get the adapter in the order of "highest performance first"

        if (!could_find)  //  finish if couldn't find the next adapter
            break;

        m_adapters.push_back(adapter);  //  store the found adapter
    }
}

DXGI_MODE_DESC1 dxgi_t::m_get_best_display_mode()
{
    //  get display modes and take the best one (highest resolution and refresh rate)
    uint32_t number_of_display_modes = 0;
    m_monitor->GetDisplayModeList1(DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_ENUM_MODES_SCALING, &number_of_display_modes, nullptr);
    DXGI_MODE_DESC1* raw_display_modes = new DXGI_MODE_DESC1 [number_of_display_modes];
    m_display_modes.reserve(number_of_display_modes);
    m_monitor->GetDisplayModeList1(DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_ENUM_MODES_SCALING, &number_of_display_modes, raw_display_modes);
    for (uint32_t index = 0; index < number_of_display_modes; index++)
        m_display_modes.push_back(std::move(raw_display_modes [index]));
    return m_display_modes.at(number_of_display_modes - 1);
}

}  //  namespace zeno::gfx
