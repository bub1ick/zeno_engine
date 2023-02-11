#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <zeno.hpp>

namespace zeno
{
gltf_loader_t::gltf_loader_t()
    : m_gltf_parser(),
      m_gltf_model()
{
}

mesh_t* gltf_loader_t::load(std::string in_name)
{
    bool    could_load = m_gltf_parser.LoadASCIIFromFile(&m_gltf_model, nullptr, nullptr, in_name);

    mesh_t* loaded_mesh = new mesh_t;

    try
    {
        for (const auto& mesh : m_gltf_model.meshes)
        {
            for (const auto& primitive : mesh.primitives)
            {
                load_vertex_attribute(primitive, "POSITION", loaded_mesh);
                load_vertex_attribute(primitive, "NORMAL", loaded_mesh);
                load_vertex_indices(primitive, loaded_mesh);
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}

void gltf_loader_t::load_vertex_attribute(const tinygltf::Primitive& primitive, std::string_view in_attribute, mesh_t* out_mesh)
{
    const auto&                    accessor      = m_gltf_model.accessors.at(primitive.attributes.at(in_attribute.data()));
    const auto&                    buffer_view   = m_gltf_model.bufferViews.at(accessor.bufferView);
    const auto&                    buffer        = m_gltf_model.buffers.at(buffer_view.buffer);
    const size_t                   buffer_offset = accessor.byteOffset + buffer_view.byteOffset;

    const float*                   vertex_attribute_data = reinterpret_cast<const float*>(&buffer.data.at(buffer_offset));

    std::vector<DirectX::XMFLOAT3> vertex_atributes;

    for (int index = 0; index < accessor.count; index++)
    {
        float x = vertex_attribute_data [index * 3];
        float y = vertex_attribute_data [index * 3 + 1];
        float z = vertex_attribute_data [index * 3 + 2];

        vertex_atributes.push_back({x, y, z});
    }


    if (in_attribute == "POSITION")
        out_mesh->vertex_positions = std::move(vertex_atributes);
    else if (in_attribute == "NORMAL")
        out_mesh->vertex_normals = std::move(vertex_atributes);
}

void gltf_loader_t::load_vertex_indices(const tinygltf::Primitive& primitive, mesh_t* out_mesh)
{
    const auto&     accessor      = m_gltf_model.accessors.at(primitive.indices);
    const auto&     buffer_view   = m_gltf_model.bufferViews.at(accessor.bufferView);
    const auto&     buffer        = m_gltf_model.buffers.at(buffer_view.buffer);
    const size_t    buffer_offset = accessor.byteOffset + buffer_view.byteOffset;

    const uint16_t* vertex_indices_data = reinterpret_cast<const uint16_t*>(&buffer.data.at(buffer_offset));

    for (int index = 0; index < accessor.count; index++)
        out_mesh->vertex_indices.push_back(vertex_indices_data [index]);
}
};  //  namespace zeno