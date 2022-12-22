#include <zeno.hpp>
#include <assert.h>

namespace zeno::gfx
{

renderer_t::renderer_t(const sys::window_t& in_window)

try : m_feature_level{D3D_FEATURE_LEVEL_11_1}, m_window(in_window), m_dxgi_module()
{
    assert(m_create_device());

    m_dxgi_module.create_swapchain(m_device, true, m_window.get_handle());

    assert(m_create_target_view());

    ID3DBlob* vs_blob = nullptr;  //  holds compiled vertex shader
    ID3DBlob* ps_blob = nullptr;  //  holds compiled pixel shader
    assert(m_compile_shaders(vs_blob, ps_blob));

    m_result = m_device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &m_vs);
    assert(SUCCEEDED(m_result));
    m_result = m_device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &m_ps);
    assert(SUCCEEDED(m_result));

    //  the data that can be passed to vertex shader
    D3D11_INPUT_ELEMENT_DESC layout [] = {
        {"POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    m_result = m_device->CreateInputLayout(layout, ARRAYSIZE(layout), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &m_vertex_input_layout);
    assert(SUCCEEDED(m_result));

    m_device_context->IASetInputLayout(m_vertex_input_layout);

    assert(m_setup_vertex_buffer());
    assert(m_setup_index_buffer());

    m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    assert(m_setup_constant_buffer());

    //  initialize world matrix
    m_world_matrix = DirectX::XMMatrixIdentity();

    //  initialize view matrix
    DirectX::XMVECTOR cam_position  = DirectX::XMVectorSet(0.f, -5.f, 0.f, 0.f);
    DirectX::XMVECTOR cam_direction = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);
    DirectX::XMVECTOR cam_up        = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);  //  ?
    m_view_matrix                   = DirectX::XMMatrixLookAtRH(cam_position, cam_direction, cam_up);

    int  width {};
    int  height {};
    RECT rect {};
    if (GetWindowRect(m_window.get_handle(), &rect))
    {
        width  = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    //  initialize projection matrix
    m_projection_matrix = DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV2, static_cast<float>(width / height), 0.01f, 100.f);
}

catch (const dxgi_exception_t& dxgi_ex)
{
    std::cerr << dxgi_ex.get_error_message() << std::endl;
}

renderer_t::~renderer_t()
{
    m_device->Release();
    m_device_context->Release();
}

void renderer_t::update()
{
    float           background_color [4] = {0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f};

    static float    delta_time   = 0.f;
    static uint64_t time_start   = 0;
    uint64_t        time_current = GetTickCount64();

    if (time_start == 0)
        time_start = time_current;
    delta_time = (time_current - time_start) / 1000.f;

    m_world_matrix = DirectX::XMMatrixRotationY(delta_time);

    ID3D11RenderTargetView* old_target_view;
    m_result = m_render_target_view->QueryInterface<ID3D11RenderTargetView>(&old_target_view);
    assert(SUCCEEDED(m_result));
    m_device_context->OMSetRenderTargets(1, &old_target_view, nullptr);

    m_device_context->ClearRenderTargetView(m_render_target_view, background_color);

    matrix_buffer_t matrix_buffer;
    matrix_buffer.world_matrix      = DirectX::XMMatrixTranspose(m_world_matrix);
    matrix_buffer.view_matrix       = DirectX::XMMatrixTranspose(m_view_matrix);
    matrix_buffer.projection_matrix = DirectX::XMMatrixTranspose(m_projection_matrix);

    m_device_context->UpdateSubresource(m_current_constant_buffer, 0, nullptr, &matrix_buffer, 0, 0);

    m_device_context->VSSetShader(m_vs, nullptr, 0);
    m_device_context->VSSetConstantBuffers(0, 1, &m_current_constant_buffer);
    m_device_context->PSSetShader(m_ps, nullptr, 0);

    D3D11_RECT window_rectangle;
    GetClientRect(m_window.get_handle(), &window_rectangle);

    D3D11_VIEWPORT viewport {};
    viewport.Width    = static_cast<float>(window_rectangle.right - window_rectangle.left);
    viewport.Height   = static_cast<float>(window_rectangle.bottom - window_rectangle.top);
    viewport.MaxDepth = 1.f;
    m_device_context->RSSetViewports(1, &viewport);

    m_device_context->DrawIndexed(36, 0, 0);

    m_dxgi_module.get_swapchain()->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

bool renderer_t::m_create_device()
{
    m_result = D3D11CreateDevice(
        m_dxgi_module.get_graphics_card(),
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

    if (FAILED(m_result))
    {
        std::cerr << "Failed to create Direct3D device!\t" << std::hex << m_result << std::endl;
        return false;
    }

    try
    {
        m_dxgi_module.initialize_device(m_device);
    }
    catch (const dxgi_exception_t& dxgi_ex)
    {
        std::cerr << dxgi_ex.get_error_message() << std::endl;
        return false;
    }

    m_result = m_device->QueryInterface<ID3D11Debug>(&m_debug_device);
    assert(SUCCEEDED(m_result));

    m_result = m_debug_device->QueryInterface<ID3D11InfoQueue>(&m_info_queue);
    assert(SUCCEEDED(m_result));


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
    m_result = m_device->CreateRenderTargetView1(frame_buffer, nullptr, &m_render_target_view);
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't create Render Target view!" << std::endl;
        return false;
    }
    return true;
}

bool renderer_t::m_compile_shaders(ID3DBlob*& out_vs_blob, ID3DBlob*& out_ps_blob)
{
    ID3DBlob* error_blob = nullptr;

    //  compile vertex shader
    m_result = D3DCompileFromFile(
        L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &out_vs_blob, &error_blob
    );
    if (FAILED(m_result))
    {
        if (error_blob)
        {
            OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }
        if (out_vs_blob)
            out_vs_blob->Release();

        out_vs_blob = nullptr;
        out_ps_blob = nullptr;
        return false;
    }

    //  compile pixel shader
    m_result = D3DCompileFromFile(
        L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &out_ps_blob, &error_blob
    );
    if (FAILED(m_result))
    {
        if (error_blob)
        {
            OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }
        if (out_ps_blob)
            out_ps_blob->Release();
        out_vs_blob = nullptr;
        out_ps_blob = nullptr;
        return false;
    }

    return true;
}

bool renderer_t::m_setup_vertex_buffer()
{
    simple_vertex_t vertices [] = {
        { {-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, //  0th vertex
        {  {1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, //  1st vertex
        {   {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, //  2nd vertex
        {  {-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, //  3rd vertex
        {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, //  4th vertex
        { {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, //  5th vertex
        {  {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //  6th vertex
        { {-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, //  7th vertex
    };

    m_vertex_stride = sizeof(simple_vertex_t);  //  how big each complex piece of data is
    m_vertex_offset = 0;                        //  offset where the array starts
    m_vertex_count  = 8;                        //  how big the array is

    //  creating a vertex buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.ByteWidth = sizeof(vertices);
    buff_desc.Usage     = D3D11_USAGE_DEFAULT;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subres_data {};
    subres_data.pSysMem = vertices;

    m_result = m_device->CreateBuffer(&buff_desc, &subres_data, &m_current_vertex_buffer);

    if (FAILED(m_result))
        return false;

    m_device_context->IASetVertexBuffers(0, 1, &m_current_vertex_buffer, &m_vertex_stride, &m_vertex_offset);

    return true;
}

bool renderer_t::m_setup_index_buffer()
{
    uint16_t indices [] {
        //  clang-format off
        3, 1, 0,  //  vertices
        2, 1, 3,  //  corresponding indices

        0, 5, 4,  //  vertices
        1, 5, 0,  //  corresponding indices

        3, 4, 7,  //  vertices
        0, 4, 3,  //  corresponding indices

        1, 6, 5,  //  vertices
        2, 6, 1,  //  corresponding indices

        2, 7, 6,  //  vertices
        3, 7, 2,  //  corresponding indices

        6, 4, 5,  //  vertices
        7, 4, 6,  //  corresponding indices
        //  clang-format on
    };

    //  initialize an index buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.Usage          = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth      = sizeof(indices);
    buff_desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subres_data {};
    subres_data.pSysMem = indices;

    m_result = m_device->CreateBuffer(&buff_desc, &subres_data, &m_current_index_buffer);
    if (FAILED(m_result))
        return false;

    m_device_context->IASetIndexBuffer(m_current_index_buffer, DXGI_FORMAT_R16_UINT, 0);

    return true;
}

bool renderer_t::m_setup_constant_buffer()
{
    //  initialize a matrix buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.Usage          = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth      = sizeof(matrix_buffer_t);
    buff_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    buff_desc.CPUAccessFlags = 0;

    m_result = m_device->CreateBuffer(&buff_desc, nullptr, &m_current_constant_buffer);

    return FAILED(m_result) ? false : true;
}
}  //  namespace zeno::gfx