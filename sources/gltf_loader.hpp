#pragma once

namespace zeno
{

class gltf_loader_t
{
public:
    gltf_loader_t();

    gfx::mesh_t* load(std::string in_name);

private:
    void               load_vertex_attribute(const tinygltf::Primitive& primitive, std::string_view in_attribute, gfx::mesh_t* out_mesh);
    void               load_vertex_indices(const tinygltf::Primitive& primitive, gfx::mesh_t* out_mesh);

    tinygltf::TinyGLTF m_gltf_parser;

    tinygltf::Model    m_gltf_model;
};
}  //  namespace zeno