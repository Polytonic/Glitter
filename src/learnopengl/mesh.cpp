#include "learnopengl/mesh.h"

void Mesh::GetTris(glm::mat4 model_mat, std::vector<InterPtr>* tris) {
  DMat4 final_mat = model_mat * local_model_mat_;
  for (int i = 0; i < indices.size() - 2; i += 3) {
    DVertex v0(vertices[indices[i]]);
    DVertex v1(vertices[indices[i + 1]]);
    DVertex v2(vertices[indices[i + 2]]);
    v0.Apply(final_mat);
    v1.Apply(final_mat);
    v2.Apply(final_mat);
    tris->push_back(InterPtr(new InterTri(&material_, parent_, v0, v1, v2)));
  }
}
