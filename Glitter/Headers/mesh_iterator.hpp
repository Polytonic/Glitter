#ifndef MESH_ITERATOR_HPP
#define MESH_ITERATOR_HPP

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "iterable_mesh.hpp"
#include "learnopengl/mesh.h"
#include "mutation_generator.hpp"

struct MeshVertices {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

class MeshIterator {
 public:
  void SetIterableMesh(std::unique_ptr<IterableMesh> iterable_model) {
    iterable_model_ = std::move(iterable_model);
    is_closed_ = iterable_model_->IsClosed();
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

class MutationMeshIterator : public MeshIterator {
 public:
  MutationMeshIterator(unsigned int u_texels, unsigned int v_texels,
                       std::shared_ptr<MutationGenerator> generator,
                       double epsilon = 1e-10);

  MeshVertices GetMesh() override;

 private:
  DVec3 GetMeshPos(double u, double v);
  DVec3 GetMeshNorm(double u, double v);

  unsigned int u_texels_;
  unsigned int v_texels_;
  std::shared_ptr<MutationGenerator> generator_;
  double epsilon_;
};

using VBoundsFn = std::function<std::pair<double, double>(double u)>;

// Note: cannot be used for closed meshes.
class BoundedMeshIterator : public MeshIterator {
public:
  BoundedMeshIterator(unsigned int u_texels, unsigned int v_texels,
		      double u_min, double u_max,
		      VBoundsFn bounds_fn, bool reverse_normals = false);

  MeshVertices GetMesh() override;

 private:
  unsigned int u_texels_;
  unsigned int v_texels_;
  double u_min_;
  double u_max_;
  VBoundsFn bounds_fn_;
  bool reverse_normals_;
};

#endif
