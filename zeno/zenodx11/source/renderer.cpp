#include <zeno.hpp>

namespace zeno::dx11
{
renderer_t::renderer_t(plat::window_t* in_window_ptr)
{
    try
    {
        const auto win32window_ptr = static_cast<plat::win32::win32_window_t*>(in_window_ptr);
        m_window_handle            = win32window_ptr->get_handle();

        HRESULT                 result = S_OK;

        ID3D11Device*           device          = nullptr;
        ID3D11Device5*          device5         = nullptr;
        ID3D11DeviceContext*    device_context  = nullptr;
        ID3D11DeviceContext4*   device_context4 = nullptr;

        const D3D_FEATURE_LEVEL feature_levels [2] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};

        result = D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels, 2, D3D11_SDK_VERSION, &device, nullptr,
            &device_context
        );
        if (FAILED(result))
            throw std::runtime_error("Couldn't Create D3D11Device and D3D11DeviceContext.");

        result = device->QueryInterface<ID3D11Device5>(&device5);
        if (FAILED(result))
            throw std::runtime_error("Couldn't query ID3D11Device5 from ID3D11Device.");

        device->Release();

        result = device_context->QueryInterface<ID3D11DeviceContext4>(&device_context4);
        if (FAILED(result))
            throw std::runtime_error("Couldn't query ID3D11DeviceContext4 from ID3D11DeviceContext.");

        device_context->Release();

        DXGI_SWAP_CHAIN_DESC1 descriptor = {};

        descriptor.Width       = 0;
        descriptor.Height      = 0;
        descriptor.Format      = DXGI_FORMAT_B8G8R8A8_UNORM;
        descriptor.Stereo      = false;
        descriptor.SampleDesc  = {1, 0};
        descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        descriptor.BufferCount = 2;
        descriptor.Scaling     = DXGI_SCALING_STRETCH;
        descriptor.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        descriptor.AlphaMode   = DXGI_ALPHA_MODE_IGNORE;
        descriptor.Flags       = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        IDXGIDevice4*    dxgi_device4    = nullptr;
        IDXGIAdapter*    dxgi_adapter    = nullptr;
        IDXGIAdapter4*   dxgi_adapter4   = nullptr;
        IDXGIFactory7*   dxgi_factory7   = nullptr;
        IDXGISwapChain1* dxgi_swapchain1 = nullptr;
        IDXGISwapChain4* dxgi_swapchain4 = nullptr;

        result = device5->QueryInterface<IDXGIDevice4>(&dxgi_device4);
        if (FAILED(result))
            throw std::runtime_error("Couldn't query IDXGIDevice4 from ID3D11Device5.");

        result = dxgi_device4->GetAdapter(&dxgi_adapter);
        if (FAILED(result))
            throw std::runtime_error("Couldn't get IDXGIAdapter from IDXGIDevice4.");

        result = dxgi_adapter->QueryInterface<IDXGIAdapter4>(&dxgi_adapter4);
        if (FAILED(result))
            throw std::runtime_error("Couldn't query IDXGIAdapter3 from IDXGIAdapter.");

        dxgi_adapter->Release();

        result = dxgi_adapter4->GetParent(IID_IDXGIFactory7, reinterpret_cast<void**>(&dxgi_factory7));
        if (FAILED(result))
            throw std::runtime_error("Couldn't get DXGIFactory parent of DXGI Adapter.");

        result = dxgi_factory7->CreateSwapChainForHwnd(
            device5, m_window_handle, &descriptor, nullptr, nullptr, &dxgi_swapchain1
        );
        if (FAILED(result))
            throw std::runtime_error("Couldn't create Swap Chain.");

        result = dxgi_swapchain1->QueryInterface<IDXGISwapChain4>(&dxgi_swapchain4);
        if (FAILED(result))
            throw std::runtime_error("Couldn't query IDXGISwapChain4 from IDXGISwapChain1");

        dxgi_swapchain1->Release();
    }
    catch (std::exception& exception)
    {
        MessageBoxA(m_window_handle, exception.what(), "DirectX Fatal Error", MB_ICONERROR);
    }
}
}  //  namespace zeno::dx11