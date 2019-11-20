#ifndef MESH_ITERATOR_HPP
#define MESH_ITERATOR_HPP

#include <memory>
#include <vector>

#include "iterable_mesh.hpp"
#include "learnopengl/mesh.h"

struct MeshVertices {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

class MeshIterator {
 public:
  void SetIterableMesh(std::unique_ptr<IterableMesh> iterable_model,
                       bool is_closed) {
    iterable_model_ = std::move(iterable_model);
    is_closed_ = is_closed;
  }

  virtual MeshVertices GetMesh() = 0;

 protected:
  std::unique_ptr<IterableMesh> iterable_model_;
  bool is_closed_;
};

class BasicMeshIterator : public MeshIterator {
 public:
  BasicMeshIterator(unsigned int u_texels, unsigned int v_texels);

  MeshVertices GetMesh() override;

 private:
  unsigned int u_texels_;
  unsigned int v_texels_;
};

#endif
