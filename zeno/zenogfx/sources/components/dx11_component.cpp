#include <zeno.hpp>

namespace zeno::gfx
{

dx11_component_t::dx11_component_t(const sys::window_t& in_window)
    : m_feature_level {D3D_FEATURE_LEVEL_11_1},
      m_dxgi()
{
    m_create_device();
    if (true)
        throw dx_exception_t("Failed to create Direct3D Device!", m_result, dx_exception_t::d3d11);
    m_dxgi.initialize_device(m_device);
    m_dxgi.create_swapchain(m_device, true, in_window);
    m_create_target_view();
}

dx11_component_t::~dx11_component_t()
{
    SAFE_COM_RELEASE(m_device);
    SAFE_COM_RELEASE(m_device_context);
    SAFE_COM_RELEASE(m_render_target_view);
    SAFE_COM_RELEASE(m_vs);
    SAFE_COM_RELEASE(m_ps);
    SAFE_COM_RELEASE(m_vertex_input_layout);
    SAFE_COM_RELEASE(m_current_vertex_buffer);
    SAFE_COM_RELEASE(m_current_index_buffer);
    SAFE_COM_RELEASE(m_current_constant_buffer);
}

bool dx11_component_t::m_create_device()
{
    m_result = D3D11CreateDevice(
        m_dxgi.get_graphics_card(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
        &m_feature_level,
        1,
        D3D11_SDK_VERSION,
        reinterpret_cast<ID3D11Device**>(&m_device),
        nullptr,
        reinterpret_cast<ID3D11DeviceContext**>(&m_device_context)
    );
    return SUCCEEDED(m_result);
}

bool dx11_component_t::m_create_target_view()
{
    //  take out the current frame from the swapchain
    ID3D11Texture2D1* frame_buffer;
    m_result = m_dxgi.get_swapchain()->GetBuffer(0, IID_ID3D11Texture2D1, reinterpret_cast<void**>(&frame_buffer));
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't get the swap chain buffer!" << std::endl;
        return false;
    }

    //  from the frame get the target view
    m_result = m_device->CreateRenderTargetView1(frame_buffer, nullptr, &m_render_target_view);
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't create Render Target view!" << std::endl;
        return false;
    }
    return true;
}
}  //  namespace zeno::gfx