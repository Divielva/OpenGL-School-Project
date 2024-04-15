#include "Heightmap.h"
#include "stb_image.h"

void Heightmap::generate_terrain(const char* heightmap_path, const float& height_scale, const float& texture_scale)
{
    int width, height, channels;
    unsigned char* data = stbi_load(heightmap_path, &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load heightmap: " << heightmap_path << std::endl;
        return;
    }

    set_position(glm::vec3(-width / 2 * texture_scale, 0, -height / 2 * texture_scale));

    vertices.clear();
    indices.clear();

    for (int z = 0; z < height; z++)
    {
        for (int x = 0; x < width; x++)
        {
            float height = (data[(z * width + x) * channels] / 255.0f - 0.5f) * height_scale;
            vertices.push_back(Vertex{ glm::vec3(x * texture_scale, height, z * texture_scale), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(x / 8.f, z / 8.f)});
        }
    }

    for (int z = 0; z < height - 1; z++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            indices.push_back(z * width + x);
            indices.push_back((z + 1) * width + x);
            indices.push_back(z * width + x + 1);
            indices.push_back(z * width + x + 1);
            indices.push_back((z + 1) * width + x);
            indices.push_back((z + 1) * width + x + 1);
        }
    }

    stbi_image_free(data);
    collider.parent = this;
    collider.should_overlap = false;
}