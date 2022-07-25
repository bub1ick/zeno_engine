#include <zeno.hpp>

namespace zeno
{
    application_t::application_t(const char* window_name, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y)
    {
        try
        {
        // Try getting the application instance
            _instance = GetModuleHandleA(nullptr);
            if (!_instance)
            {
                throw std::runtime_error("Could not get .exe instance handle. To avoid this issue, please use the full constructor.");
            }
        }
        catch (std::exception& exception)
        {
            std::cout << exception.what();
        }

        _window = new window_t(window_name, pos_x, pos_y, size_x, size_y, _instance, SW_SHOWDEFAULT);
    }

    application_t::application_t(const char* window_name, int32_t argument_count, char** arguments, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y)
    {
        try
        {
        // Try getting the application instance
            _instance = GetModuleHandleA(nullptr);
            if (!_instance)
            {
                throw std::runtime_error("Could not get .exe instance handle. To avoid this issue, please use the full constructor.");
            }
        }
        catch (std::exception& exception)
        {
            std::cout << exception.what();
        }

        _window = new window_t(window_name, pos_x, pos_y, size_x, size_y, _instance, SW_SHOWDEFAULT);
    }

    application_t::~application_t()
    {
        delete _window;
    }

    void application_t::run()
    {
        // HRESULT result;

        // ID3D11Device* device = nullptr;
        // ID3D11Device5* device5 = nullptr;
        // ID3D11DeviceContext* device_context = nullptr;
        // ID3D11DeviceContext4* device_context4 = nullptr;

        // D3D11CreateDevice(
        //     nullptr,
        //     D3D_DRIVER_TYPE_HARDWARE,
        //     nullptr,
        //     0,
        //     nullptr, 6,
        //     D3D11_SDK_VERSION,
        //     &device,
        //     nullptr,
        //     &device_context
        // );

        // device->QueryInterface<ID3D11Device5>(&device5);
        // device->Release();

        // device_context->QueryInterface<ID3D11DeviceContext4>(&device_context4);
        // device_context->Release();

        // DXGI_SWAP_CHAIN_DESC descriptor = {};
        // descriptor.Windowed = TRUE;
        // descriptor.BufferCount = 2;
        // descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        // descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        // descriptor.SampleDesc.Count = 1;
        // descriptor.SampleDesc.Quality = 0;
        // descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        // descriptor.OutputWindow = _window->get_window_handle();

        // IDXGIDevice4* dxgi_device = nullptr;
        // IDXGIAdapter3* dxgi_adapter = nullptr;
        // IDXGIFactory5* dxgi_factory = nullptr;

        // dxgi_device = (IDXGIDevice4*)device5;

        // result = dxgi_device->GetAdapter((IDXGIAdapter**)&dxgi_adapter);

        // if (SUCCEEDED(result))
        // {
        //     dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));
        // }

        MSG message;
        while (true)
        {
            if (GetMessageA(&message, _window->get_window_handle(), 0, 0) != 0)
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
        }
    }



}
