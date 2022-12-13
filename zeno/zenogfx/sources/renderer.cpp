#include <zeno.hpp>
#include <assert.h>

namespace zeno::gfx
{

renderer_t::renderer_t(HWND in_window_handle)

try : m_feature_level{D3D_FEATURE_LEVEL_11_1}, m_window_handle{in_window_handle}, m_dxgi_module()
{
    //  create the device
    if (m_create_device() == false)
    {
        //  TODO: handle error
    }

    m_dxgi_module.create_swapchain(m_device, true, in_window_handle);


    if (m_create_target_view() == false)
    {
        //  TODO: handle error
    }


    ID3DBlob* vs_blob    = nullptr;  //  holds compiled vertex shader
    ID3DBlob* ps_blob    = nullptr;  //  holds compiled pixel shader
    ID3DBlob* error_blob = nullptr;  //  holds error if such exists

    //  compile vertex shader
    m_result = D3DCompileFromFile(
        L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vs_blob, &error_blob
    );
    if (FAILED(m_result))
    {
        if (error_blob)
        {
            OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }
        if (vs_blob)
            vs_blob->Release();
        assert(false);
    }

    //  compile pixel shader
    m_result = D3DCompileFromFile(
        L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &ps_blob, &error_blob
    );
    if (FAILED(m_result))
    {
        if (error_blob)
        {
            OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }
        if (ps_blob)
            ps_blob->Release();
        assert(false);
    }


    //  pass the vertex shader to graphics card
    m_result = m_device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &m_vs);
    assert(SUCCEEDED(m_result));

    //  pass the pixel shader to graphics card
    m_result = m_device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &m_ps);
    assert(SUCCEEDED(m_result));

    //  the data that can be passed to vertex shader
    D3D11_INPUT_ELEMENT_DESC input_descriptor [] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {   "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
  //    {"NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
  //    {"TEX", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    //  apply the input layout
    m_result = m_device->CreateInputLayout(
        input_descriptor, ARRAYSIZE(input_descriptor), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &m_vertex_input_layout
    );
    assert(SUCCEEDED(m_result));

    m_device_context->IASetInputLayout(m_vertex_input_layout);

    ///  an array defining a triangle mesh in a 2D space with 3 points:
    float vertex_data_array [][3] = {
        { 0.0f,  0.5f, 0.0f}, //  point at top
        { 0.5f, -0.5f, 0.0f}, //  point at bottom-right
        {-0.5f, -0.5f, 0.0f}, //  point at bottom-left
    };

    m_vertex_stride = 3 * sizeof(float);  //  how big each complex piece of data is
    m_vertex_offset = 0;                  //  offset where the array starts
    m_vertex_count  = 3;                  //  how big the array is

    //  creating a vertex buffer
    {
        D3D11_BUFFER_DESC vertex_buffer_descriptor = {};
        vertex_buffer_descriptor.ByteWidth         = sizeof(vertex_data_array);
        vertex_buffer_descriptor.Usage             = D3D11_USAGE_DEFAULT;
        vertex_buffer_descriptor.BindFlags         = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA sr_data = {0};
        sr_data.pSysMem                = vertex_data_array;

        HRESULT hr = m_device->CreateBuffer(&vertex_buffer_descriptor, &sr_data, &m_current_vertex_buffer);
        assert(SUCCEEDED(hr));
    }

    //  TODO: continue to make a renderer (follow some guides idk)
}

catch (const dxgi_exception_t& dxgi_ex)
{
    std::cerr << dxgi_ex.get_error_message() << std::endl;
}

renderer_t::~renderer_t()
{
    //  clean up d3d11
    m_device->Release();
    m_device_context->Release();
}

void renderer_t::update()
{
    float background_color [4] = {0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f};
    m_device_context->ClearRenderTargetView(reinterpret_cast<ID3D11RenderTargetView*>(m_render_target_view), background_color);

    RECT window_rectangle {};
    GetClientRect(m_window_handle, &window_rectangle);

    D3D11_VIEWPORT viewport {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width    = static_cast<float>(window_rectangle.right - window_rectangle.left);
    viewport.Height   = static_cast<float>(window_rectangle.bottom - window_rectangle.top);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_device_context->RSSetViewports(1, &viewport);

    m_device_context->OMSetRenderTargets(1, reinterpret_cast<ID3D11RenderTargetView**>(&m_render_target_view), nullptr);

    m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_device_context->IASetInputLayout(m_vertex_input_layout);
    m_device_context->IASetVertexBuffers(0, 1, &m_current_vertex_buffer, &m_vertex_stride, &m_vertex_offset);

    m_device_context->VSSetShader(m_vs, nullptr, 0);
    m_device_context->PSSetShader(m_ps, nullptr, 0);

    m_device_context->Draw(m_vertex_count, 0);

    m_dxgi_module.get_swapchain()->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

bool renderer_t::m_create_device()
{
    m_result = D3D11CreateDevice(
        m_dxgi_module.get_graphics_card(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        D3D11_CREATE_DEVICE_SINGLETHREADED,
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
    try
    {
        m_dxgi_module.initialize_device(m_device);
    }
    catch (const dxgi_exception_t& dxgi_ex)
    {
        std::cerr << dxgi_ex.get_error_message() << std::endl;
        return false;
    }

    return true;
}

bool renderer_t::m_create_target_view()
{
    //  take out the current frame from the swapchain
    ID3D11Texture2D1* frame_buffer;
    m_result = m_dxgi_module.get_swapchain()->GetBuffer(0, IID_ID3D11Texture2D1, reinterpret_cast<void**>(&frame_buffer));
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't get the swap chain buffer!" << std::endl;
        return false;
    }

    //  from the frame get the target view
    m_result = m_device->CreateRenderTargetView1(frame_buffer, 0, &m_render_target_view);
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't create Render Target view!" << std::endl;
        return false;
    }
    return true;
}

}  //  namespace zeno::dx11