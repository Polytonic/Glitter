#include "boids/character.hpp"

#include "texture_gen.hpp"

namespace {

  std::pair<double, double> WingVFunc(double u){
    if(u < 0.25){
      return std::pair<double, double>(
	0.25 + 0.5 * u,
	3 * u
	);
    } else if(u < 0.45){
      return std::pair<double, double>(
	0.25 + 0.25 - 0.5 * u,
	1.5 - 3 * u
	);
    } else if(u < 0.55){
      return std::pair<double, double>(-1, -1);
    } else if(u < 0.75){
      return std::pair<double, double>(
	0.25 + 0.5 * (u - 0.5),
	3 * (u - 0.5)
	);
    } else {
      return std::pair<double, double>(
	0.25 + 0.25 - 0.5 * (u - 0.5),
	1.5 - 3 * (u - 0.5)
	);
    }
  }

  Mesh GetWingOuterMesh(std::default_random_engine* random_gen){
    Texture texture = GetColorTexture({66, 133, 244});
    std::unique_ptr<IterableMesh> it_mesh(
      new IterableCylinder(3.0, 0.5));
    BoundedMeshIterator mesh_iterator(100, 30, 0.05, 0.95,
				      &WingVFunc);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    glm::mat4 mesh_model_mat = glm::mat4(1.0f);
    mesh_model_mat = glm::rotate(mesh_model_mat, (float)M_PI / 2.0f,
				 glm::vec3(1.0f, 0, 0));
    return Mesh(mesh_vert.vertices, mesh_vert.indices, {texture}, mesh_model_mat);
  }

  Mesh GetWingInnerMesh(std::default_random_engine* random_gen){
    Texture texture = GetColorTexture({234, 67, 53});
    std::unique_ptr<IterableMesh> it_mesh(
      new IterableCylinder(3.0, 0.499));
    BoundedMeshIterator mesh_iterator(100, 30, 0.05, 0.95,
				      &WingVFunc, true);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    glm::mat4 mesh_model_mat = glm::mat4(1.0f);
    mesh_model_mat = glm::rotate(mesh_model_mat, (float)M_PI / 2.0f,
				 glm::vec3(1.0f, 0, 0));
    return Mesh(mesh_vert.vertices, mesh_vert.indices, {texture}, mesh_model_mat);
  }

  Mesh GetBody(std::default_random_engine* random_gen) {
    Texture texture = GetColorTexture({234, 67, 53});
    std::unique_ptr<IterableMesh> it_mesh(
      new IterableSphere(0.3));
    BasicMeshIterator mesh_iterator(50, 50);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    glm::mat4 mesh_model_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.5f));
    return Mesh(mesh_vert.vertices, mesh_vert.indices, {texture}, mesh_model_mat);
  }
  
}

std::unique_ptr<Model> GetBoidCharacter(std::default_random_engine* random_gen){
  std::unique_ptr<Model> generated_model(new Model({GetWingOuterMesh(random_gen),
						    GetWingInnerMesh(random_gen),
						    GetBody(random_gen)}));
  return generated_model;
}
