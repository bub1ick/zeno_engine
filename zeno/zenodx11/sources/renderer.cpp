#include <zeno.hpp>

namespace zeno::dx11
{
renderer_t::renderer_t(HWND window_handle)
    : m_feature_level(D3D_FEATURE_LEVEL_11_1)
{
    //  create DXGI factory to handle DXGI
    m_result = CreateDXGIFactory2(0, IID_IDXGIFactory7, reinterpret_cast<void**>(&m_dxgi.factory));
    if (FAILED(m_result))
        std::cerr << "Failed on CreateDXGIFactory2!\t" << std::hex << m_result << std::endl;

    std::vector<IDXGIAdapter4*> adapters;  //  vector holding all found adapters
    get_all_available_adapters(adapters);  //  the first adapter is the the most powerful one
    m_dxgi.graphics_card = adapters [0];   //  the first adapter should be our graphics card, or generic windows renderer


    //  DEBUG print chosen graphics card
    {
        DXGI_ADAPTER_DESC3 adapter_descriptor {};
        m_dxgi.graphics_card->GetDesc3(&adapter_descriptor);

        std::wstring wide_description   = adapter_descriptor.Description;
        std::string  graphics_card_name = utils::utf16_to_utf8(wide_description);

        std::cout << graphics_card_name << std::endl;
    }

    //  get the monitor
    m_result = m_dxgi.graphics_card->EnumOutputs(0, reinterpret_cast<IDXGIOutput**>(&m_dxgi.monitor));

    if (create_device() == false)
    {
        //  TODO: handle error
    }

    DXGI_SWAP_CHAIN_DESC1 swapchain_descriptor {};
    swapchain_descriptor.Width       = 0;                               //  determine by the set window size
    swapchain_descriptor.Height      = 0;                               //  determine by the set window size
    swapchain_descriptor.Format      = DXGI_FORMAT_R16G16B16A16_FLOAT;  //  some random format that is supported by flip swap chain
    swapchain_descriptor.Stereo      = false;                           //  true is for VR-like things (two screens rendering at the same time)
    swapchain_descriptor.SampleDesc  = {1, 0};                          //  disable MSAA
    swapchain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;  //  how can the swapchain be used
    swapchain_descriptor.BufferCount = 2;                                                          //  we want the front and the back buffer (sums up to 2)
    swapchain_descriptor.Scaling     = DXGI_SCALING_NONE;                                          //  stretch preserving the original rendered aspect ratio
    swapchain_descriptor.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;                           //  use flip model instead of bitblt model (more in MSDN)
    swapchain_descriptor.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;                                //  FIXME IDK what is this
    swapchain_descriptor.Flags       = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    //  info on the dxgi format topic:
    //      - https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/converting-data-color-space
    //      - https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-24-importance-being-linear


    //  get display modes and take the best one (highest resolution and refresh rate)
    uint32_t number_of_display_modes = 0;
    m_dxgi.monitor->GetDisplayModeList1(DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_ENUM_MODES_SCALING, &number_of_display_modes, nullptr);
    DXGI_MODE_DESC1* display_modes = new DXGI_MODE_DESC1 [number_of_display_modes];
    m_dxgi.monitor->GetDisplayModeList1(DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_ENUM_MODES_SCALING, &number_of_display_modes, display_modes);
    DXGI_MODE_DESC1 best_display_mode = display_modes [number_of_display_modes - 1];

    //  DEBUG print display modes
    {
        DXGI_OUTPUT_DESC1 monitor_descriptor {};
        m_dxgi.monitor->GetDesc1(&monitor_descriptor);

        std::wstring utf16_monitor_name = monitor_descriptor.DeviceName;
        std::string  monitor_name       = utils::utf16_to_utf8(utf16_monitor_name);

        std::cout << monitor_name << "\n  Display Modes:" << std::endl;

        for (int32_t index = 0; index < number_of_display_modes; index++)
        {
            std::cout << "\t-   Width x Height: " << display_modes [index].Width << " x " << display_modes [index].Height << std::endl;
            std::cout << "\t-     Refresh Rate: " << display_modes [index].RefreshRate.Numerator / display_modes [index].RefreshRate.Denominator << "Hz"
                      << std::endl;
            std::cout << "\t-           Format: " << display_modes [index].Format << std::endl;
            std::cout << "\t- ScanlineOrdering: " << display_modes [index].ScanlineOrdering << std::endl;
            std::cout << "\t-          Scaling: " << display_modes [index].Scaling << std::endl;
            std::cout << "\t-        Is Stereo: " << display_modes [index].Stereo << std::endl;
            std::cout << std::endl;
        }
    }

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchain_fullscreen_descriptor {};

    swapchain_fullscreen_descriptor.RefreshRate      = best_display_mode.RefreshRate;       //  get the highest refresh rate from the monitor
    swapchain_fullscreen_descriptor.ScanlineOrdering = best_display_mode.ScanlineOrdering;  //  get scanline ordering from the monitor
    swapchain_fullscreen_descriptor.Scaling          = DXGI_MODE_SCALING_CENTERED;          //  FIXME never use scaling (for now)
    swapchain_fullscreen_descriptor.Windowed         = false;                               //  start the application in fullscreen

    //  this is how to get the refresh rate of the monitor. the modes are resolutions combined with refresh rate and etc.
    m_result = m_dxgi.factory->CreateSwapChainForHwnd(
        m_dxgi.device, window_handle, &swapchain_descriptor, &swapchain_fullscreen_descriptor, nullptr, reinterpret_cast<IDXGISwapChain1**>(&m_swapchain)
    );
    if (FAILED(m_result))
    {
        std::cout << "Couldn't create swapchain for the window:" << std::flush;
    }

    // DEBUG
    {
        m_swapchain->GetDesc1(&swapchain_descriptor);

        {
            std::cout << std::endl << std::endl;
            std::cout << swapchain_descriptor.Width << std::endl;
        }
    }

    DXGI_MODE_DESC new_resolution {};
    new_resolution.Width            = best_display_mode.Width;
    new_resolution.Height           = best_display_mode.Height;
    new_resolution.RefreshRate      = best_display_mode.RefreshRate;
    new_resolution.Format           = best_display_mode.Format;
    new_resolution.ScanlineOrdering = best_display_mode.ScanlineOrdering;
    new_resolution.Scaling          = best_display_mode.Scaling;

    m_swapchain->ResizeTarget(&new_resolution);

    delete [] display_modes;

    //  TODO: continue to make a renderer(follow some guides idk)
}

void renderer_t::get_all_available_adapters(std::vector<IDXGIAdapter4*>& out_adapters)
{
    IDXGIAdapter4* adapter;  //  used to temporary store found adapter

    //  getting all the adapters
    bool           could_find;  //  to check wether there are adapters left
    for (uint8_t index = 0;; index++)
    {
        could_find =
            m_dxgi.factory->EnumAdapterByGpuPreference(index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_IDXGIAdapter4, reinterpret_cast<void**>(&adapter))
            != DXGI_ERROR_NOT_FOUND;  //  get the adapter in the order of "highest performance first"

        if (not could_find)  //  finish if couldn't find the next adapter
            break;

        out_adapters.push_back(adapter);  //  store the found adapter
    }
}

bool renderer_t::create_device()
{
    m_result = D3D11CreateDevice(
        m_dxgi.graphics_card,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        0,
        &m_feature_level,
        1,
        D3D11_SDK_VERSION,
        reinterpret_cast<ID3D11Device**>(&m_device),
        nullptr,
        reinterpret_cast<ID3D11DeviceContext**>(&m_device_context)
    );  //  create the device and its context for our graphics card

    if (FAILED(m_result))
    {
        std::cerr << "Failed to create Direct3D device!\t" << std::hex << m_result << std::endl;
        return false;
    }

    //  after creating the direct3d device, obtain the corresponding dxgi device
    m_result = m_device->QueryInterface<IDXGIDevice4>(&m_dxgi.device);

    if (FAILED(m_result))
    {
        std::cerr << "Failed to obtain DXGI device!\t" << std::hex << m_result << std::endl;
        return false;
    }

    return true;
}
}  //  namespace zeno::dx11