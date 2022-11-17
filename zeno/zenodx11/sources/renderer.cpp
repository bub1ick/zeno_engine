#include <zeno.hpp>

namespace zeno::dx11
{
renderer_t::renderer_t()
{
    //  create DXGI factory to handle DXGI
    m_result = CreateDXGIFactory2(0, IID_IDXGIFactory7, reinterpret_cast<void**>(&m_dxgi.factory));

    if (FAILED(m_result))  //  check for failure
        std::cerr << "Failed on CreateDXGIFactory2!\t" << std::hex << m_result << std::endl;

    std::vector<IDXGIAdapter4*> adapters;  //  vector holding all found adapters

    get_all_available_adapters(adapters);  //  the first adapter is the the most powerful one

    m_dxgi.video_card = adapters [0];

    const D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_1;

    m_result = D3D11CreateDevice(
        m_dxgi.video_card,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        0,
        &feature_level,
        1,
        D3D11_SDK_VERSION,
        reinterpret_cast<ID3D11Device**>(&m_device),
        nullptr,
        reinterpret_cast<ID3D11DeviceContext**>(&m_device_context)
    );

    if (FAILED(m_result))
        std::cerr << "Failed to create Direct3D device!\t" << std::hex << m_result << std::endl;

    m_result = m_device->QueryInterface<IDXGIDevice4>(&m_dxgi.device);

    if (FAILED(m_result))
        std::cerr << "Failed to obtain DXGI device!\t" << std::hex << m_result << std::endl;

    DXGI_SWAP_CHAIN_DESC1 swapchain_descriptor {};
    swapchain_descriptor.Width       = 0;
    swapchain_descriptor.Height      = 0;
    swapchain_descriptor.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_descriptor.Stereo      = false;
    swapchain_descriptor.SampleDesc  = {1, 0};
    swapchain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_SHARED;
    swapchain_descriptor.BufferCount = 2;
    swapchain_descriptor.Scaling     = DXGI_SCALING_ASPECT_RATIO_STRETCH;
    swapchain_descriptor.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapchain_descriptor.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapchain_descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchain_fullscreen_descriptor {};

    // m_dxgi.factory->CreateSwapChainForHwnd(
    //     m_device,
    //     nullptr,  //  FIXME

    // )
}

void renderer_t::get_all_available_adapters(std::vector<IDXGIAdapter4*>& out_adapters)
{
    IDXGIAdapter4* adapter;  //  used to temporary store found adapter

    //  getting all the adapters
    uint8_t        index = 0;   //  adapter index
    bool           could_find;  //  to check wether there are adapters left

    for (;;)
    {
        could_find =
            m_dxgi.factory->EnumAdapterByGpuPreference(
                index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_IDXGIAdapter4, reinterpret_cast<void**>(&adapter)
            ) != DXGI_ERROR_NOT_FOUND;  //  get the adapter in the order of "highest performance first"

        if (not could_find)  //  finish if couldn't find the next adapter
            break;

        out_adapters.push_back(adapter);  //  store the found adapter

        index++;  //  next adapter
    }
}
}  //  namespace zeno::dx11