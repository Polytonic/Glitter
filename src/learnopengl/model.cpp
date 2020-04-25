#include <learnopengl/model.h>

Model::Model(std::vector<Mesh> meshes) {
  this->meshes = std::move(meshes);
  for (Mesh& mesh : this->meshes) {
    mesh.set_parent(this);
  }
}
