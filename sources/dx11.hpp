#pragma once

#include "dxgi.hpp"

namespace zeno::gfx
{
struct simple_vertex_t
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT4 color;
};

struct matrix_buffer_t
{
    DirectX::XMMATRIX world_matrix;
    DirectX::XMMATRIX view_matrix;
    DirectX::XMMATRIX projection_matrix;
};

struct mesh_t
{
    std::vector<DirectX::XMFLOAT3> vertex_positions;
    std::vector<DirectX::XMFLOAT3> vertex_normals;
    std::vector<DirectX::XMFLOAT4> vertex_colors;
    std::vector<uint16_t>          vertex_indices;
};

class dx11_t
{
public:
    dx11_t(const sys::window_t& in_window);
    dx11_t(const dx11_t& that);
    dx11_t(dx11_t&& that);
    dx11_t& operator=(const dx11_t& that);
    dx11_t& operator=(dx11_t&& that);
    ~dx11_t();

    ID3D11Device5*        get_device() noexcept { return m_device; }

    ID3D11DeviceContext4* get_device_context() noexcept { return m_device_context; }

    dxgi_t*&              get_dxgi() noexcept { return m_dxgi; }

    void                  update(const sys::window_t& in_window, const float delta_time_in_seconds);

    void                  set_mesh(const mesh_t* in_mesh);

private:
    ///  @brief holds the results of direct3d functions
    HRESULT                      m_result;
    ///  @brief holds all dxgi related interfaces
    dxgi_t*                      m_dxgi;

    ///  @brief create direct3d and dxgi device and its context
    ///  @return true on success, false on error
    bool                         m_create_device();
    ///  @brief shows which version of direct3d to request
    const D3D_FEATURE_LEVEL      m_feature_level;
    ///  @brief the direct3d device
    ID3D11Device5*               m_device;
    ///  @brief the direct3d device context
    ID3D11DeviceContext4*        m_device_context;

    ///  @brief is used to create a target view for device context to draw to
    ///  @return true on success, false on error
    bool                         m_create_target_view();
    ///  @brief the direct3d render target view
    ID3D11RenderTargetView1*     m_render_target_view;

    ///  @brief !!!TESTING!!! temporary used to compile vertex and pixel
    ///  @param out_vs_blob vertex shader blob (binary data) reference
    ///  @param out_ps_blob pixel shader blob (binary data) reference
    ///  @return true on success, false on error. If there was an error, the blobs are nullptr.
    bool                         m_compile_shaders(ID3DBlob*& out_vs_blob, ID3DBlob*& out_ps_blob);
    bool                         m_setup_input_layout(ID3DBlob*& in_vs_blob);
    //  vertex and pixel shaders
    ID3D11VertexShader*          m_vs = nullptr;
    ID3D11PixelShader*           m_ps = nullptr;

    mesh_t*                      m_current_mesh;

    bool                         m_setup_vertex_buffer();
    ///  @brief describes how data is organized before being passed to vertex shader
    ID3D11InputLayout*           m_vertex_input_layout;
    ///  @brief an array of vertices described by 3 coordinates on the screen
    ID3D11Buffer*                m_current_vertex_buffer;
    std::vector<simple_vertex_t> m_vertices;
    uint32_t                     m_vertex_stride;
    uint32_t                     m_vertex_offset;
    uint32_t                     m_vertex_count;


    bool                         m_setup_index_buffer();
    ///  @brief an array of indices for each vertex
    ID3D11Buffer*                m_current_index_buffer;
    uint64_t                     m_number_of_indices;

    bool                         m_setup_constant_buffer();
    ///  @brief holds matrices for rotating the cube
    ID3D11Buffer*                m_current_constant_buffer;


    void                         m_setup_camera(const sys::window_t& in_window);
    void                         m_update_rotation(const float delta_time_in_seconds);
    DirectX::XMMATRIX            m_world_matrix;
    DirectX::XMMATRIX            m_view_matrix;
    DirectX::XMMATRIX            m_projection_matrix;
    matrix_buffer_t              m_matrix_buffer;
};
}  //  namespace zeno::gfx