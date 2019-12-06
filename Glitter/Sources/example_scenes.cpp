#include "example_scenes.hpp"

#include <memory>
#include <utility>

#include "boids/simulation.hpp"
#include "box_textures.hpp"
#include "elementary_models.hpp"
#include "fps_counter.hpp"
#include "interpolation.hpp"
#include "iterable_mesh.hpp"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"
#include "learnopengl/shader_m.h"
#include "mesh_iterator.hpp"
#include "mutation_generator.hpp"
#include "point_shadows_dynamic_renderer.hpp"
#include "point_shadows_rt_renderer.hpp"
#include "rt_renderer.hpp"
#include "simple_rt_renderer.hpp"
#include "texture_gen.hpp"

std::unique_ptr<RtRenderer> InProgressScene(
    std::default_random_engine* random_gen) {
  std::unique_ptr<RtRenderer> renderer(new PointShadowsDynamicRenderer());
  renderer->OpenWindow("In-Progress Scene");
  std::unique_ptr<BoidsSimulation> simulation(
      new BoidsSimulation(*random_gen, 100));
  renderer->AddEventHandler(simulation.get());
  renderer->AddDynamicModel(std::move(simulation));
  FpsCounter* fps = new FpsCounter;
  renderer->AddEventHandler(fps);
  return renderer;
}

std::unique_ptr<RtRenderer> GetBasicFractalNoiseDemo(
    std::default_random_engine* random_gen) {
  std::unique_ptr<RtRenderer> renderer(new PointShadowsRtRenderer());
  renderer->OpenWindow("Fractal Noise Demo");
  {
    Texture texture = GetWhiteTexture();
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Texture texture = GetTestBoxTexture(random_gen);
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::rotate(model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Texture texture = GetTestBoxTexture(random_gen);
    std::unique_ptr<IterableMesh> it_mesh(new IterableRectPlane(2.0f, 2.0f));
    std::shared_ptr<MutationGenerator> mut =
        std::make_shared<FractalNoiseGenerator>(random_gen, 7, -0.5f, 0.7f);
    MutationMeshIterator mesh_iterator(129, 129, mut);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0, -3.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  return renderer;
}

std::unique_ptr<RtRenderer> GetSponzaDemo(
    std::default_random_engine* random_gen) {
  std::unique_ptr<RtRenderer> renderer(new PointShadowsRtRenderer());
  renderer->OpenWindow("Sponza Demo");
  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "nanosuit";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir)
        .append(model_dir)
        .append("/")
        .append(object_file)
        .append(file_type);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.5f, 0.0f));
    model_mat = glm::scale(
        model_mat, glm::vec3(0.1f, 0.1f, 0.1f));  // scale nanosuit model
    renderer->AddModel(file_path, model_mat);
  }
  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "sponza";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir)
        .append(model_dir)
        .append("/")
        .append(object_file)
        .append(file_type);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.5f, 0.0f));
    model_mat = glm::scale(
        model_mat, glm::vec3(0.005f, 0.005f, 0.005f));  // scale sponza model
    renderer->AddModel(file_path, model_mat);
  }
  return renderer;
}
