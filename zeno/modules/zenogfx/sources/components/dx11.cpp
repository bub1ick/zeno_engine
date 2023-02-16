#include <zeno.hpp>

namespace zeno::gfx
{

dx11_t::dx11_t(const sys::window_t& in_window)
    : m_feature_level {D3D_FEATURE_LEVEL_11_1}
{
    m_dxgi = new dxgi_t();

    if (!m_create_device())
        throw dx_exception_t("Couldn't create Direct3D Device!", m_result, dx_exception_t::e_d3d11);

    m_dxgi->initialize_device(m_device);
    m_dxgi->create_swapchain(m_device, true, in_window);

    if (!m_create_target_view())
        throw dx_exception_t("Couldn't create Direct3D Target View!", m_result, dx_exception_t::e_d3d11);

    ID3DBlob* vs_blob {};  //  holds compiled vertex shader
    ID3DBlob* ps_blob {};  //  holds compiled pixel shader

    if (!m_compile_shaders(vs_blob, ps_blob))
        throw dx_exception_t("Couldn't compile Direct3D Shaders!", m_result, dx_exception_t::e_d3d11);

    if (!m_setup_input_layout(vs_blob))
        throw dx_exception_t("Couldn't Setup Direct3D Input Layout!", m_result, dx_exception_t::e_d3d11);

    m_setup_camera(in_window);
}

dx11_t::dx11_t(const dx11_t& that)
    : m_feature_level {D3D_FEATURE_LEVEL_11_1}
{
    *this = that;
}

dx11_t::dx11_t(dx11_t&& that)
    : m_feature_level {D3D_FEATURE_LEVEL_11_1}
{
    *this = std::move(that);
}

dx11_t& dx11_t::operator=(const dx11_t& that)
{
    if (this == &that)
        return *this;

    m_result = that.m_result;
    m_dxgi   = new dxgi_t(*that.m_dxgi);

    COM_COPY(m_device, that.m_device);
    COM_COPY(m_device_context, that.m_device_context);
    COM_COPY(m_render_target_view, that.m_render_target_view);
    COM_COPY(m_vs, that.m_vs);
    COM_COPY(m_ps, that.m_ps);
    COM_COPY(m_vertex_input_layout, that.m_vertex_input_layout);
    COM_COPY(m_current_vertex_buffer, that.m_current_vertex_buffer);
    COM_COPY(m_current_constant_buffer, that.m_current_constant_buffer);

    m_vertices      = that.m_vertices;
    m_vertex_stride = that.m_vertex_stride;
    m_vertex_offset = that.m_vertex_offset;
    m_vertex_count  = that.m_vertex_count;

    m_world_matrix      = that.m_world_matrix;
    m_view_matrix       = that.m_view_matrix;
    m_projection_matrix = that.m_projection_matrix;
    m_matrix_buffer     = that.m_matrix_buffer;

    return *this;
}

dx11_t& dx11_t::operator=(dx11_t&& that)
{
    if (this == &that)
        return *this;

    m_result      = that.m_result;
    that.m_result = 0;

    if (m_dxgi != nullptr)
    {
        delete m_dxgi;
        m_dxgi      = that.m_dxgi;
        that.m_dxgi = nullptr;
    }

    COM_MOVE(m_device, that.m_device);
    COM_MOVE(m_device_context, that.m_device_context);
    COM_MOVE(m_render_target_view, that.m_render_target_view);
    COM_MOVE(m_vs, that.m_vs);
    COM_MOVE(m_ps, that.m_ps);
    COM_MOVE(m_vertex_input_layout, that.m_vertex_input_layout);
    COM_MOVE(m_current_vertex_buffer, that.m_current_vertex_buffer);
    COM_MOVE(m_current_constant_buffer, that.m_current_constant_buffer);

    m_vertices = that.m_vertices;
    that.m_vertices.clear();

    m_vertex_stride = that.m_vertex_stride;
    m_vertex_offset = that.m_vertex_offset;
    m_vertex_count  = that.m_vertex_count;

    that.m_vertex_stride = 0;
    that.m_vertex_offset = 0;
    that.m_vertex_count  = 0;

    m_world_matrix      = that.m_world_matrix;
    m_view_matrix       = that.m_view_matrix;
    m_projection_matrix = that.m_projection_matrix;
    m_matrix_buffer     = that.m_matrix_buffer;

    return *this;
}

dx11_t::~dx11_t()
{
    COM_RELEASE(m_device);
    COM_RELEASE(m_device_context);
    COM_RELEASE(m_render_target_view);
    COM_RELEASE(m_vs);
    COM_RELEASE(m_ps);
    COM_RELEASE(m_vertex_input_layout);
    COM_RELEASE(m_current_vertex_buffer);
    COM_RELEASE(m_current_index_buffer);
    COM_RELEASE(m_current_constant_buffer);

    delete m_dxgi;
    delete m_current_mesh;
}

void dx11_t::update(const sys::window_t& in_window, const float delta_time_in_seconds)
{
    m_update_rotation(delta_time_in_seconds);

    m_device_context->VSSetShader(m_vs, nullptr, 0);

    m_device_context->VSSetConstantBuffers(0, 1, &m_current_constant_buffer);

    sys::window_t::dimentions_t window_size = in_window.get_dimentions();
    D3D11_VIEWPORT              viewport {};                     //  stores rendering viewport
    viewport.Width    = static_cast<float>(window_size.width);   //  window width
    viewport.Height   = static_cast<float>(window_size.height);  //  window height
    viewport.MaxDepth = 1.f;                                     //  maximum window depth
    m_device_context->RSSetViewports(1, &viewport);

    m_device_context->PSSetShader(m_ps, nullptr, 0);

    //  clear it with a color declared earlier
    m_device_context->ClearRenderTargetView(m_render_target_view, DirectX::Colors::MidnightBlue);

    //  set it as a render target
    ID3D11RenderTargetView* old_target_view {};
    m_result = m_render_target_view->QueryInterface<ID3D11RenderTargetView>(&old_target_view);

    m_device_context->OMSetRenderTargets(1, &old_target_view, nullptr);

    COM_RELEASE(old_target_view);

    //  draw indexed vertices
    m_device_context->DrawIndexed(36, 0, 0);

    m_dxgi->get_swapchain()->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

void dx11_t::set_mesh(const mesh_t* in_mesh)
{
    m_current_mesh = const_cast<mesh_t*>(in_mesh);

    m_setup_vertex_buffer();
    m_setup_index_buffer();
    m_setup_constant_buffer();
}

bool dx11_t::m_create_device()
{
    m_result = D3D11CreateDevice(
        m_dxgi->get_graphics_card(),
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

bool dx11_t::m_create_target_view()
{
    ID3D11Texture2D1* frame_buffer {};
    //  take out the current frame from the swapchain
    m_result = m_dxgi->get_swapchain()->GetBuffer(0, IID_ID3D11Texture2D1, reinterpret_cast<void**>(&frame_buffer));
    if (FAILED(m_result))
        return false;

    m_result = m_device->CreateRenderTargetView1(frame_buffer, nullptr, &m_render_target_view);
    if (FAILED(m_result))
        return false;
    return true;
}

bool dx11_t::m_compile_shaders(ID3DBlob*& out_vs_blob, ID3DBlob*& out_ps_blob)
{
    ID3DBlob* error_blob = nullptr;

    //  compile vertex shader
    m_result = D3DCompileFromFile(
        L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &out_vs_blob, &error_blob
    );
    if (FAILED(m_result))
    {
        if (error_blob)
            OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
        COM_RELEASE(error_blob);
        COM_RELEASE(out_vs_blob);
        COM_RELEASE(out_ps_blob);
        return false;
    }

    //  compile pixel shader
    m_result = D3DCompileFromFile(
        L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &out_ps_blob, &error_blob
    );
    if (FAILED(m_result))
    {
        if (error_blob)
            OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
        COM_RELEASE(error_blob);
        COM_RELEASE(out_vs_blob);
        COM_RELEASE(out_ps_blob);
        return false;
    }

    COM_RELEASE(error_blob);

    m_result = m_device->CreateVertexShader(out_vs_blob->GetBufferPointer(), out_vs_blob->GetBufferSize(), nullptr, &m_vs);
    if (FAILED(m_result))
        return false;

    m_result = m_device->CreatePixelShader(out_ps_blob->GetBufferPointer(), out_ps_blob->GetBufferSize(), nullptr, &m_ps);
    if (FAILED(m_result))
        return false;


    return true;
}

bool dx11_t::m_setup_input_layout(ID3DBlob*& in_vs_blob)
{
    //  the data that can be passed to vertex shader
    D3D11_INPUT_ELEMENT_DESC layout [] = {
        {"POSITION", 0,   DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {  "NORMAL", 0,   DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {   "COLOR", 0, DXGI_FORMAT_R16G16B16A16_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };


    //  create an input layout and store it in m_vertex_input_layout
    m_result = m_device->CreateInputLayout(layout, ARRAYSIZE(layout), in_vs_blob->GetBufferPointer(), in_vs_blob->GetBufferSize(), &m_vertex_input_layout);
    if (FAILED(m_result))
        return false;

    //  set the input layout we just created
    m_device_context->IASetInputLayout(m_vertex_input_layout);

    //  set rendering topology
    //  topology types:  https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology
    m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool dx11_t::m_setup_vertex_buffer()
{
    m_vertices.clear();
    //  store cube vertices
    //  m_vertices = {
    //     {  {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, //  0th vertex
    //     {   {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, //  1st vertex
    //     {  {1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, //  2nd vertex
    //     { {-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, //  3rd vertex
    //     { {-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, //  4th vertex
    //     {  {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, //  5th vertex
    //     { {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //  6th vertex
    //     {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, //  7th vertex
    //  };

    for (int index = 0; index < m_current_mesh->vertex_positions.size(); index++)
    {
        m_vertices.push_back({m_current_mesh->vertex_positions [index], m_current_mesh->vertex_normals [index], m_current_mesh->vertex_colors [index]});
    }

    m_vertex_stride = sizeof(simple_vertex_t);  //  how big each complex piece of data is
    m_vertex_offset = 0;                        //  offset where the array starts
    m_vertex_count  = m_vertices.size();        //  how big the array is

    //  creating a vertex buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.ByteWidth = m_vertex_stride * m_vertices.size();
    buff_desc.Usage     = D3D11_USAGE_DEFAULT;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subres_data {};
    subres_data.pSysMem = m_vertices.data();

    m_result = m_device->CreateBuffer(&buff_desc, &subres_data, &m_current_vertex_buffer);

    if (FAILED(m_result))
        return false;

    m_device_context->IASetVertexBuffers(0, 1, &m_current_vertex_buffer, &m_vertex_stride, &m_vertex_offset);

    return true;
}

bool dx11_t::m_setup_index_buffer()
{
    //  std::vector<uint16_t> indices = {
    //      //  clang-format off
    //      3, 1, 0,  //  vertices
    //      2, 1, 3,  //  corresponding indices

    //  0, 5, 4,  //  vertices
    //  1, 5, 0,  //  corresponding indices

    //  3, 4, 7,  //  vertices
    //  0, 4, 3,  //  corresponding indices

    //  1, 6, 5,  //  vertices
    //  2, 6, 1,  //  corresponding indices

    //  2, 7, 6,  //  vertices
    //  3, 7, 2,  //  corresponding indices

    //  6, 4, 5,  //  vertices
    //  7, 4, 6,  //  corresponding indices
    //  //  clang-format on
    //};

    std::vector<uint16_t> indices;

    for (const auto& index : m_current_mesh->vertex_indices)
        indices.push_back(index);

    //  initialize an index buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.Usage          = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth      = indices.size() * sizeof(uint16_t);
    buff_desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subres_data {};
    subres_data.pSysMem = indices.data();

    m_result = m_device->CreateBuffer(&buff_desc, &subres_data, &m_current_index_buffer);
    if (FAILED(m_result))
        return false;

    m_device_context->IASetIndexBuffer(m_current_index_buffer, DXGI_FORMAT_R16_UINT, 0);

    return true;
}

bool dx11_t::m_setup_constant_buffer()
{
    //  initialize a matrix buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.Usage          = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth      = sizeof(matrix_buffer_t);
    buff_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    buff_desc.CPUAccessFlags = 0;

    m_result = m_device->CreateBuffer(&buff_desc, nullptr, &m_current_constant_buffer);

    return SUCCEEDED(m_result);
}

void dx11_t::m_setup_camera(const sys::window_t& in_window)
{
    //  initialize world matrix
    m_world_matrix = DirectX::XMMatrixIdentity();

    //  initialize view matrix
    DirectX::XMVECTOR cam_position  = DirectX::XMVectorSet(0.f, 2.f, 5.f, 0.f);  //  camera position in a right-handed coordinate position
    DirectX::XMVECTOR cam_direction = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);  //  a point in space the camera is looking at
    DirectX::XMVECTOR cam_up_vector = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);  //  vector pointing up from camera pos
    m_view_matrix                   = DirectX::XMMatrixLookAtRH(cam_position, cam_direction, cam_up_vector);  //  set the camera

    sys::window_t::dimentions_t window_size  = in_window.get_dimentions();
    float                       aspect_ratio = static_cast<float>(window_size.width) / static_cast<float>(window_size.height);
    //  initialize projection matrix
    m_projection_matrix = DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV2, aspect_ratio, 0.01f, 100.f);
}

void dx11_t::m_update_rotation(const float delta_time_in_seconds)
{
    m_world_matrix = DirectX::XMMatrixRotationY(delta_time_in_seconds);

    m_matrix_buffer.world_matrix      = DirectX::XMMatrixTranspose(m_world_matrix);
    m_matrix_buffer.view_matrix       = DirectX::XMMatrixTranspose(m_view_matrix);
    m_matrix_buffer.projection_matrix = DirectX::XMMatrixTranspose(m_projection_matrix);

    m_device_context->UpdateSubresource(m_current_constant_buffer, 0, nullptr, &m_matrix_buffer, 0, 0);
}
}  //  namespace zeno::gfx