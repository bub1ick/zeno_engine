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

gfx::mesh_t* gltf_loader_t::load(std::string in_name)
{
    bool         could_load = m_gltf_parser.LoadASCIIFromFile(&m_gltf_model, nullptr, nullptr, in_name);

    gfx::mesh_t* loaded_mesh = new gfx::mesh_t;

    try
    {
        for (const auto& mesh : m_gltf_model.meshes)
        {
            for (const auto& primitive : mesh.primitives)
            {
                load_vertex_attribute(primitive, "POSITION", loaded_mesh);
                load_vertex_attribute(primitive, "NORMAL", loaded_mesh);
                load_vertex_attribute(primitive, "COLOR_0", loaded_mesh);
                load_vertex_indices(primitive, loaded_mesh);
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
    return loaded_mesh;
}

void gltf_loader_t::load_vertex_attribute(const tinygltf::Primitive& primitive, std::string_view in_attribute, gfx::mesh_t* out_mesh)
{
    const auto&  accessor      = m_gltf_model.accessors.at(primitive.attributes.at(in_attribute.data()));
    const auto&  buffer_view   = m_gltf_model.bufferViews.at(accessor.bufferView);
    const auto&  buffer        = m_gltf_model.buffers.at(buffer_view.buffer);
    const size_t buffer_offset = accessor.byteOffset + buffer_view.byteOffset;

    //  fill in the color (special)
    if (in_attribute == "COLOR_0")
    {
        if (accessor.type == TINYGLTF_TYPE_VEC4)
        {
            const uint16_t*                vertex_attribute_data = reinterpret_cast<const uint16_t*>(&buffer.data.at(buffer_offset));
            std::vector<DirectX::XMFLOAT4> vertex_attributes_vec4;
            int8_t                         vecsize_jump = 4;

            for (int index = 0; index < accessor.count; index++)
            {
                uint16_t r_norm = vertex_attribute_data [index * vecsize_jump];
                uint16_t g_norm = vertex_attribute_data [index * vecsize_jump + 1];
                uint16_t b_norm = vertex_attribute_data [index * vecsize_jump + 2];
                uint16_t a_norm = vertex_attribute_data [index * vecsize_jump + 3];

                uint16_t max_value = std::numeric_limits<uint16_t>::max();

                float    r_float = static_cast<float>(r_norm) / static_cast<float>(max_value);
                float    g_float = static_cast<float>(g_norm) / static_cast<float>(max_value);
                float    b_float = static_cast<float>(b_norm) / static_cast<float>(max_value);
                float    a_float = static_cast<float>(a_norm) / static_cast<float>(max_value);

                vertex_attributes_vec4.push_back({r_float, g_float, b_float, a_float});
            }
            
            out_mesh->vertex_colors = std::move(vertex_attributes_vec4);
        }
    }
    else
    {
        const float*                   vertex_attribute_data = reinterpret_cast<const float*>(&buffer.data.at(buffer_offset));
        std::vector<DirectX::XMFLOAT3> vertex_attributes_vec3;
        int32_t                        vecsize_jump = 3;

        for (int index = 0; index < accessor.count; index++)
        {
            float x = vertex_attribute_data [index * vecsize_jump];
            float y = vertex_attribute_data [index * vecsize_jump + 1];
            float z = vertex_attribute_data [index * vecsize_jump + 2];

            vertex_attributes_vec3.push_back({x, y, z});
        }

        if (in_attribute == "POSITION")
            out_mesh->vertex_positions = std::move(vertex_attributes_vec3);
        else if (in_attribute == "NORMAL")
            out_mesh->vertex_normals = std::move(vertex_attributes_vec3);
    }
}

void gltf_loader_t::load_vertex_indices(const tinygltf::Primitive& primitive, gfx::mesh_t* out_mesh)
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