#pragma once

#include "exceptions/exceptions.hpp"
#include "dxgi/dxgi_module.hpp"

namespace zeno::gfx
{

struct simple_vertex_t
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

struct matrix_buffer_t
{
    DirectX::XMMATRIX world_matrix;
    DirectX::XMMATRIX view_matrix;
    DirectX::XMMATRIX projection_matrix;
};

class ZENO_API renderer_t
{
public:
    renderer_t(const sys::window_t& in_window);
    ~renderer_t();

    void update();

private:
    ///  @brief create direct3d and dxgi device and its context
    ///  @return true on success, false on error
    bool                     m_create_device();

    ///  @brief is used to create a target view for device context to draw to
    ///  @return true on success, false on error
    bool                     m_create_target_view();

    ///  @brief !!!TESTING!!! temporary used to compile vertex and pixel
    ///  @param out_vs_blob vertex shader blob (binary data)
    ///  @param out_ps_blob pixel shader blob (binary data)
    ///  @return true on success, false on error
    bool                     m_compile_shaders(ID3DBlob** out_vs_blob, ID3DBlob** out_ps_blob);

    ///  @brief holds all dxgi related interfaces
    dxgi_module_t            m_dxgi_module;

    const sys::window_t&     m_window;

    ///  @brief shows which version of direct3d to request
    const D3D_FEATURE_LEVEL  m_feature_level;

    ///  @brief the direct3d device
    ID3D11Device5*           m_device;

    ///  @brief the direct3d device context
    ID3D11DeviceContext4*    m_device_context;

    ///  @brief the direct3d render target view
    ID3D11RenderTargetView1* m_render_target_view;

    ///  @brief holds the results of direct3d and dxgi functions
    HRESULT                  m_result;

    ///  @brief describes how data is organized before being passed to vertex shader
    ID3D11InputLayout*       m_vertex_input_layout;

    ///  @brief an array of vertices described by 3 coordinates on the screen
    ID3D11Buffer*            m_current_vertex_buffer;

    ///  @brief an array of indices for each vertex
    ID3D11Buffer*            m_current_index_buffer;

    ID3D11Buffer*            m_current_constant_buffer;

    uint32_t                 m_vertex_stride;

    uint32_t                 m_vertex_offset;

    uint32_t                 m_vertex_count;

    DirectX::XMMATRIX        m_world_matrix;
    DirectX::XMMATRIX        m_view_matrix;
    DirectX::XMMATRIX        m_projection_matrix;

    //  vertex and pixel shaders
    ID3D11VertexShader*      m_vs = nullptr;
    ID3D11PixelShader*       m_ps = nullptr;
};
}  //  namespace zeno::gfx