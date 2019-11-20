#include "elementary_models.hpp"

#include <vector>

#include "glitter.hpp"

namespace {

std::vector<unsigned int> GetCubeFaceIndices() { return {0, 3, 2, 3, 0, 1}; }

std::vector<Vertex> GetCubeVertices() {
  std::vector<Vertex> verts;
  // back face
  verts.push_back({{-1.0f, -1.0f, -1.0f},
                   {0.0f, 0.0f, -1.0f},
                   {1.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-left
  verts.push_back({{1.0f, -1.0f, -1.0f},
                   {0.0f, 0.0f, -1.0f},
                   {0.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-right
  verts.push_back({{-1.0f, 1.0f, -1.0f},
                   {0.0f, 0.0f, -1.0f},
                   {1.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-left
  verts.push_back({{1.0f, 1.0f, -1.0f},
                   {0.0f, 0.0f, -1.0f},
                   {0.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-right

  // front face
  verts.push_back({{-1.0f, -1.0f, 1.0f},
                   {0.0f, 0.0f, 1.0f},
                   {0.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-left
  verts.push_back({{1.0f, -1.0f, 1.0f},
                   {0.0f, 0.0f, 1.0f},
                   {1.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-right
  verts.push_back({{-1.0f, 1.0f, 1.0f},
                   {0.0f, 0.0f, 1.0f},
                   {0.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-left
  verts.push_back({{1.0f, 1.0f, 1.0f},
                   {0.0f, 0.0f, 1.0f},
                   {1.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-right

  // left face
  verts.push_back({{-1.0f, -1.0f, -1.0f},
                   {-1.0f, 0.0f, 0.0f},
                   {0.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-left
  verts.push_back({{-1.0f, -1.0f, 1.0f},
                   {-1.0f, 0.0f, 0.0f},
                   {1.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-right
  verts.push_back({{-1.0f, 1.0f, -1.0f},
                   {-1.0f, 0.0f, 0.0f},
                   {0.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-left
  verts.push_back({{-1.0f, 1.0f, 1.0f},
                   {-1.0f, 0.0f, 0.0f},
                   {1.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-right

  // right face
  verts.push_back({{1.0f, -1.0f, 1.0f},
                   {1.0f, 0.0f, 0.0f},
                   {0.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-left
  verts.push_back({{1.0f, -1.0f, -1.0f},
                   {1.0f, 0.0f, 0.0f},
                   {1.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-right
  verts.push_back({{1.0f, 1.0f, 1.0f},
                   {1.0f, 0.0f, 0.0f},
                   {0.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-left
  verts.push_back({{1.0f, 1.0f, -1.0f},
                   {1.0f, 0.0f, 0.0f},
                   {1.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-right

  // bottom face
  verts.push_back({{-1.0f, -1.0f, 1.0f},
                   {0.0f, -1.0f, 0.0f},
                   {0.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-right
  verts.push_back({{1.0f, -1.0f, 1.0f},
                   {0.0f, -1.0f, 0.0f},
                   {1.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-left
  verts.push_back({{-1.0f, -1.0f, -1.0f},
                   {0.0f, -1.0f, 0.0f},
                   {0.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-right
  verts.push_back({{1.0f, -1.0f, -1.0f},
                   {0.0f, -1.0f, 0.0f},
                   {1.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-left

  // top face
  verts.push_back({{-1.0f, 1.0f, 1.0f},
                   {0.0f, 1.0f, 0.0f},
                   {0.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-left
  verts.push_back({{1.0f, 1.0f, 1.0f},
                   {0.0f, 1.0f, 0.0f},
                   {1.0f, 1.0f},
                   glm::vec3(),
                   glm::vec3()});  // bottom-right
  verts.push_back({{-1.0f, 1.0f, -1.0f},
                   {0.0f, 1.0f, 0.0f},
                   {0.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-left
  verts.push_back({{1.0f, 1.0f, -1.0f},
                   {0.0f, 1.0f, 0.0f},
                   {1.0f, 0.0f},
                   glm::vec3(),
                   glm::vec3()});  // top-right
  return verts;
}

}  // namespace

std::unique_ptr<Model> BuildBoxModel(Texture texture) {
  std::vector<Vertex> cube_verts = GetCubeVertices();
  std::vector<Mesh> meshes;
  for (int i = 0; i < 24; i += 4) {
    std::vector<Vertex> vertices = {
        cube_verts[i + 0],
        cube_verts[i + 1],
        cube_verts[i + 2],
        cube_verts[i + 3],
    };
    Mesh face_mesh(vertices, GetCubeFaceIndices(), {texture});
    meshes.push_back(face_mesh);
  }
  return std::unique_ptr<Model>(new Model(std::move(meshes)));
}
