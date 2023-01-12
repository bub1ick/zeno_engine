#include <zeno.hpp>

namespace zeno::gfx
{

dx11_component_t::dx11_component_t(const sys::window_t& in_window)
    : m_feature_level {D3D_FEATURE_LEVEL_11_1},
      m_dxgi()
{
    if (!m_create_device())
        throw dx_exception_t("Couldn't create Direct3D Device!", m_result, dx_exception_t::e_d3d11);


    m_dxgi.initialize_device(m_device.get());
    m_dxgi.create_swapchain(m_device.get(), true, in_window);

    if (!m_create_target_view())
        throw dx_exception_t("Couldn't create Direct3D Target View!", m_result, dx_exception_t::e_d3d11);

    utils::unique_com_t<ID3DBlob> vs_blob(nullptr);  //  holds compiled vertex shader
    utils::unique_com_t<ID3DBlob> ps_blob(nullptr);  //  holds compiled pixel shader

    if (!m_compile_shaders(vs_blob, ps_blob))
        throw dx_exception_t("Couldn't compile Direct3D Shaders!", m_result, dx_exception_t::e_d3d11);

    if (!m_setup_input_layout(vs_blob))
    {
        SAFE_COM_RELEASE(vs_blob);
        SAFE_COM_RELEASE(ps_blob);
        throw dx_exception_t("Couldn't Setup Direct3D Input Layout!", m_result, dx_exception_t::e_d3d11);
    }
    SAFE_COM_RELEASE(vs_blob);
    SAFE_COM_RELEASE(ps_blob);

    m_setup_vertex_buffer();
    m_setup_index_buffer();
    m_setup_constant_buffer();
}

bool dx11_component_t::m_create_device()
{
    ID3D11Device5*        raw_device         = nullptr;
    ID3D11DeviceContext4* raw_device_context = nullptr;

    m_result = D3D11CreateDevice(
        m_dxgi.get_graphics_card(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
        &m_feature_level,
        1,
        D3D11_SDK_VERSION,
        reinterpret_cast<ID3D11Device**>(&raw_device),
        nullptr,
        reinterpret_cast<ID3D11DeviceContext**>(&raw_device_context)
    );

    m_device.reset(raw_device);
    m_device_context.reset(raw_device_context);

    return SUCCEEDED(m_result);
}

bool dx11_component_t::m_create_target_view()
{
    //  take out the current frame from the swapchain
    ID3D11Texture2D1* raw_frame_buffer = nullptr;
    m_result                           = m_dxgi.get_swapchain()->GetBuffer(0, IID_ID3D11Texture2D1, reinterpret_cast<void**>(&raw_frame_buffer));
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't get the swap chain buffer!" << std::endl;
        return false;
    }
    utils::unique_com_t<ID3D11Texture2D1> frame_buffer(raw_frame_buffer);

    //  from the frame get the target view
    ID3D11RenderTargetView1*                raw_target_view = nullptr;
    m_result                                                = m_device->CreateRenderTargetView1(frame_buffer.get(), nullptr, &raw_target_view);
    if (FAILED(m_result))
    {
        std::cerr << "Couldn't create Render Target view!" << std::endl;
        return false;
    }

    m_render_target_view.reset(raw_target_view);

    return true;
}

bool dx11_component_t::m_compile_shaders(utils::unique_com_t<ID3DBlob>& out_vs_blob, utils::unique_com_t<ID3DBlob>& out_ps_blob)
{
    {
        //  We need this as to pass a pointer to pointer as a function argument
        //  we have to have a pointer to write data to and then feed this "hacky"
        //  pointer to the unique_ptr to replace the address and destroy this
        //  "hacky" pointer along the way.
        //  That sentece is as simple as using a smart pointer.
        ID3DBlob* raw_vs_blob = nullptr;
        ID3DBlob* raw_ps_blob = nullptr;
        ID3DBlob* error_blob  = nullptr;

        //  compile vertex shader
        m_result = D3DCompileFromFile(
            L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &raw_vs_blob, &error_blob
        );
        if (FAILED(m_result))
        {
            if (error_blob)
                OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
            utils::com_deleter_t<ID3DBlob>(error_blob);
            return false;
        }

        //  compile pixel shader
        m_result = D3DCompileFromFile(
            L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &raw_ps_blob, &error_blob
        );
        if (FAILED(m_result))
        {
            if (error_blob)
                OutputDebugStringA(reinterpret_cast<char*>(error_blob->GetBufferPointer()));
            utils::com_deleter_t<ID3DBlob>(error_blob);
            return false;
        }

        out_vs_blob.reset(raw_vs_blob);
        out_ps_blob.reset(raw_ps_blob);
    }

    {
        ID3D11VertexShader* raw_vs = nullptr;
        ID3D11PixelShader*  raw_ps = nullptr;

        m_result = m_device->CreateVertexShader(out_vs_blob->GetBufferPointer(), out_vs_blob->GetBufferSize(), nullptr, &raw_vs);
        if (FAILED(m_result))
            return false;

        m_result = m_device->CreatePixelShader(out_ps_blob->GetBufferPointer(), out_ps_blob->GetBufferSize(), nullptr, &raw_ps);
        if (FAILED(m_result))
            return false;

        m_vs.reset(raw_vs);
        m_ps.reset(raw_ps);
    }

    return true;
}

bool dx11_component_t::m_setup_input_layout(utils::unique_com_t<ID3DBlob>& in_vs_blob)
{
    //  the data that can be passed to vertex shader
    D3D11_INPUT_ELEMENT_DESC layout [] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {  "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };


    //  create an input layout and store it in m_vertex_input_layout
    ID3D11InputLayout* raw_input_layout = nullptr;
    m_result = m_device->CreateInputLayout(layout, ARRAYSIZE(layout), in_vs_blob->GetBufferPointer(), in_vs_blob->GetBufferSize(), &raw_input_layout);
    if (FAILED(m_result))
        return false;
    m_vertex_input_layout.reset(raw_input_layout);

    //  set the input layout we just created
    m_device_context->IASetInputLayout(m_vertex_input_layout.get());

    //  set rendering topology
    //  topology types:  https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology
    m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool dx11_component_t::m_setup_vertex_buffer()
{
    //  store cube vertices
    m_vertices = {
        {  {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, //  0th vertex
        {   {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, //  1st vertex
        {  {1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, //  2nd vertex
        { {-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, //  3rd vertex
        { {-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, //  4th vertex
        {  {1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, //  5th vertex
        { {1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, //  6th vertex
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, //  7th vertex
    };

    m_vertex_stride = sizeof(simple_vertex_t);  //  how big each complex piece of data is
    m_vertex_offset = 0;                        //  offset where the array starts
    m_vertex_count  = m_vertices.size();        //  how big the array is

    //  creating a vertex buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.ByteWidth = sizeof(simple_vertex_t) * m_vertices.size();
    buff_desc.Usage     = D3D11_USAGE_DEFAULT;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subres_data {};
    subres_data.pSysMem = m_vertices.data();

    ID3D11Buffer* raw_buffer = nullptr;
    m_result                 = m_device->CreateBuffer(&buff_desc, &subres_data, &raw_buffer);
    m_current_vertex_buffer.reset(raw_buffer);

    if (FAILED(m_result))
        return false;

    m_device_context->IASetVertexBuffers(0, 1, &raw_buffer, &m_vertex_stride, &m_vertex_offset);

    return true;
}

bool dx11_component_t::m_setup_index_buffer()
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

    ID3D11Buffer* raw_buffer = nullptr;
    m_result                 = m_device->CreateBuffer(&buff_desc, &subres_data, &raw_buffer);
    if (FAILED(m_result))
        return false;
    m_current_index_buffer.reset(raw_buffer);

    m_device_context->IASetIndexBuffer(m_current_index_buffer.get(), DXGI_FORMAT_R16_UINT, 0);

    return true;
}

bool dx11_component_t::m_setup_constant_buffer()
{
    //  initialize a matrix buffer
    D3D11_BUFFER_DESC buff_desc {};
    buff_desc.Usage          = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth      = sizeof(matrix_buffer_t);
    buff_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    buff_desc.CPUAccessFlags = 0;

    ID3D11Buffer* raw_buffer = nullptr;
    m_result                 = m_device->CreateBuffer(&buff_desc, nullptr, &raw_buffer);
    m_current_constant_buffer.reset(raw_buffer);

    return FAILED(m_result) ? false : true;
}
}  //  namespace zeno::gfx