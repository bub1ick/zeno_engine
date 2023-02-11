#pragma once

namespace zeno
{
struct mesh_t
{
    std::vector<DirectX::XMFLOAT3> vertex_positions;
    std::vector<DirectX::XMFLOAT3> vertex_normals;
    std::vector<DirectX::XMFLOAT4> vertex_colors;
    std::vector<int32_t>           vertex_indices;
};

class gltf_loader_t
{
public:
    gltf_loader_t();

    mesh_t* load(std::string in_name);

private:
    void               load_vertex_attribute(const tinygltf::Primitive& primitive, std::string_view in_attribute, mesh_t* out_mesh);
    void               load_vertex_indices(const tinygltf::Primitive& primitive, mesh_t* out_mesh);

    tinygltf::TinyGLTF m_gltf_parser;

    tinygltf::Model    m_gltf_model;
};
}  //  namespace zeno