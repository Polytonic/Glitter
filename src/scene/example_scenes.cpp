#include "scene/example_scenes.hpp"

#include <memory>
#include <utility>

#include "boids/simulation.hpp"
#include "texture/box_textures.hpp"
#include "shapes/elementary_models.hpp"
#include "realtime/fps_counter.hpp"
#include "shapes/interpolation.hpp"
#include "shapes/iterable_mesh.hpp"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"
#include "learnopengl/shader_m.h"
#include "shapes/mesh_iterator.hpp"
#include "shapes/mutation_generator.hpp"
#include "realtime/point_shadows_dynamic_renderer.hpp"
#include "realtime/multi_light_renderer.hpp"
#include "realtime/rt_renderer.hpp"
#include "shapes/onion.hpp"
#include "texture/texture_gen.hpp"

std::unique_ptr<RtRenderer> CurrentScene(
    bool windowed_mode, std::default_random_engine* random_gen) {
  std::unique_ptr<MultiLightRenderer> renderer(
      new MultiLightRenderer(windowed_mode));
  FpsCounter* fps = new FpsCounter;
  renderer->AddEventHandler(fps);
  renderer->Init("HelixGarlicNanoScene");
  {
    Texture texture = GetTestBoxTexture(random_gen);
    std::unique_ptr<IterableMesh> it_mesh(new IterableRectPlane(20.0f, 20.0f));
    BasicMeshIterator mesh_iterator(4, 4);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices smooth_mesh_vert = mesh_iterator.GetMesh();
    Mesh smooth_mesh(smooth_mesh_vert.vertices, smooth_mesh_vert.indices,
                     {texture});
    std::unique_ptr<Model> smooth_generated_model(new Model({smooth_mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -3.0f, 0));
    renderer->AddModel(std::move(smooth_generated_model), model_mat);
  }
  {
    Texture texture = GetWhiteTexture();
    Material::Options mat_opts;
    mat_opts.transparency = 0.9;
    mat_opts.index = 2.0;
    Material material(std::move(texture), mat_opts);
    std::unique_ptr<IterableMesh> it_mesh(new IterableSphere(0.5f));
    BasicMeshIterator mesh_iterator(20, 20);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, material);
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(1.5, -2.5, 0.0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Texture texture = TextureFromFile(
        FileSystem::getPath("resources/textures/awesomeface.png"),
        "texture_diffuse");
    std::unique_ptr<Model> model = BuildBoxModel(texture);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(-1.5, -2.0, 1.0));
    model_mat = glm::scale(model_mat, glm::vec3(0.5));
    renderer->AddModel(std::move(model), model_mat);
  }
  {
    Texture texture = GetWhiteTexture();
    // Texture texture = GetTestBoxTexture(random_gen);
    Material::Options mat_opts;
    mat_opts.transparency = 0.9;
    mat_opts.index = 2.0;
    Material material(std::move(texture), mat_opts);
    MeshVertices mesh_vert = GetGarlic(/*outer_radius=*/0.4,
                                       /*inner_radius=*/0.3,
                                       /*cloves=*/5,
                                       /*clove_res=*/15,
                                       /*height_res=*/40);
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, material);
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(-1.5, -2.0, -3.0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
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
    // BasicMeshIterator mesh_iterator(20, 250);
    CalcNormalsMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::rotate(model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  glm::mat4 second_helix_model_mat = glm::mat4(1.0f);
  second_helix_model_mat =
      glm::translate(second_helix_model_mat, glm::vec3(3.0f, -1.4f, -3.0));
  {
    Texture texture = GetWhiteTexture();
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    renderer->AddModel(std::move(generated_model), second_helix_model_mat);
  }
  {
    Texture texture = GetTestBoxTexture(random_gen);
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    // BasicMeshIterator mesh_iterator(20, 250);
    CalcNormalsMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat =
        glm::rotate(second_helix_model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Light l;
    l.Position = glm::vec3(-2, -1, -1);
    l.Color = glm::vec3(1);
    renderer->AddLight(l);
  }
  {
    Light l;
    l.Position = glm::vec3(-1, -1, 1);
    l.Color = glm::vec3(1);
    renderer->AddLight(l);
  }
  renderer->set_directional_light_pos(glm::vec3(-6.0f, 10.0f, -6.0f));
  renderer->set_directional_light_color(glm::vec3(0.5f, 0.5f, 0.5f));
  return renderer;
}

std::unique_ptr<RtRenderer> HelixGarlicNanoScene(
    bool windowed_mode, std::default_random_engine* random_gen) {
  std::unique_ptr<MultiLightRenderer> renderer(
      new MultiLightRenderer(windowed_mode));
  FpsCounter* fps = new FpsCounter;
  renderer->AddEventHandler(fps);
  renderer->Init("HelixGarlicNanoScene");
  {
    // Nanosuit
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
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -3.0f, 0.0f));
    model_mat = glm::scale(model_mat, glm::vec3(0.13f));
    model_mat =
        glm::rotate(model_mat, (float)(M_PI / -2.0), glm::vec3(0, 1.0f, 0));
    renderer->AddModel(file_path, model_mat);
  }
  {
    // Floor
    TexCanvas canvas = GetColorCanvas(RgbPix({0, 0, 255}), 1000, 1000);
    ApplyGrid(&canvas, 7, 7, 10, RgbPix({255, 0, 0}));
    std::unique_ptr<IterableMesh> it_mesh(new IterableRectPlane(20.0f, 20.0f));
    BasicMeshIterator mesh_iterator(4, 4);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    Material::Options mat_opts;
    // mat_opts.reflectivity = 0.5;
    Material material(std::move(canvas.ToTexture()), mat_opts);
    MeshVertices smooth_mesh_vert = mesh_iterator.GetMesh();
    Mesh smooth_mesh(smooth_mesh_vert.vertices, smooth_mesh_vert.indices,
                     material);
    std::unique_ptr<Model> smooth_generated_model(new Model({smooth_mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -3.0f, 0));
    renderer->AddModel(std::move(smooth_generated_model), model_mat);
  }
  {
    // Lower back sphere
    Texture texture = GetColorTexture(RgbPix({0, 0, 0}), 1, 1);
    Material::Options mat_opts;
    mat_opts.reflectivity = 0.8;
    Material material(std::move(texture), mat_opts);
    std::unique_ptr<IterableMesh> it_mesh(new IterableSphere(0.5f));
    BasicMeshIterator mesh_iterator(100, 100);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, material);
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(1.5, -2.5, 0.0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    // Lower forward sphere
    Texture texture = GetColorTexture(RgbPix({0, 0, 0}), 1, 1);
    Material::Options mat_opts;
    mat_opts.reflectivity = 0.8;
    Material material(std::move(texture), mat_opts);
    std::unique_ptr<IterableMesh> it_mesh(new IterableSphere(0.5f));
    BasicMeshIterator mesh_iterator(100, 100);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, material);
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(1.5, -2.5, 1.5));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    // Upper sphere
    Texture texture = GetColorTexture(RgbPix({0, 0, 0}), 1, 1);
    Material::Options mat_opts;
    mat_opts.reflectivity = 0.2;
    Material material(std::move(texture), mat_opts);
    std::unique_ptr<IterableMesh> it_mesh(new IterableSphere(0.5f));
    BasicMeshIterator mesh_iterator(100, 100);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, material);
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(2.5, -1.0, 2.0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    // Face box
    Texture texture = TextureFromFile(
        FileSystem::getPath("resources/textures/awesomeface.png"),
        "texture_diffuse");
    std::unique_ptr<Model> model = BuildBoxModel(texture);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(-1.5, -2.0, 1.0));
    model_mat = glm::scale(model_mat, glm::vec3(0.5));
    renderer->AddModel(std::move(model), model_mat);
  }
  {
    // Garlic
    Texture texture = GetWhiteTexture();
    // Texture texture = GetTestBoxTexture(random_gen);
    Material::Options mat_opts;
    mat_opts.transparency = 0.9;
    mat_opts.index = 2.0;
    Material material(std::move(texture), mat_opts);
    MeshVertices mesh_vert = GetGarlic(/*outer_radius=*/0.4,
                                       /*inner_radius=*/0.3,
                                       /*cloves=*/5,
                                       /*clove_res=*/50,
                                       /*height_res=*/100);
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, material);
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(-1.5, -2.0, -3.0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
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
    // BasicMeshIterator mesh_iterator(20, 250);
    CalcNormalsMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::rotate(model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  glm::mat4 second_helix_model_mat = glm::mat4(1.0f);
  second_helix_model_mat =
      glm::translate(second_helix_model_mat, glm::vec3(3.0f, -1.4f, -3.0));
  {
    Texture texture = GetWhiteTexture();
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    renderer->AddModel(std::move(generated_model), second_helix_model_mat);
  }
  {
    Texture texture = GetTestBoxTexture(random_gen);
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    // BasicMeshIterator mesh_iterator(20, 250);
    CalcNormalsMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat =
        glm::rotate(second_helix_model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Light l;
    l.Position = glm::vec3(-2, -1, -1);
    l.Color = glm::vec3(1);
    renderer->AddLight(l);
  }
  {
    Light l;
    l.Position = glm::vec3(3, -2, 1);
    l.Color = glm::vec3(1);
    renderer->AddLight(l);
  }
  renderer->set_directional_light_pos(glm::vec3(-6.0f, 10.0f, -6.0f));
  renderer->set_directional_light_color(glm::vec3(0.5f, 0.5f, 0.5f));
  return renderer;
}

std::unique_ptr<RtRenderer> GetBoidsScene(
    bool windowed_mode, std::default_random_engine* random_gen) {
  std::unique_ptr<RtRenderer> renderer(
      new PointShadowsDynamicRenderer(windowed_mode));
  renderer->Init("In-Progress Scene");
  std::unique_ptr<BoidsSimulation> simulation(
      new BoidsSimulation(*random_gen, 100));
  renderer->AddEventHandler(simulation.get());
  renderer->AddDynamicModel(std::move(simulation));
  FpsCounter* fps = new FpsCounter;
  renderer->AddEventHandler(fps);
  return renderer;
}

std::unique_ptr<RtRenderer> GetBasicFractalNoiseDemo(
    bool windowed_mode, std::default_random_engine* random_gen) {
  std::unique_ptr<RtRenderer> renderer(
      new PointShadowsDynamicRenderer(windowed_mode));
  renderer->Init("Fractal Noise Demo");
  {
    Texture texture = GetWhiteTexture();
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(0.5f, 4.0f, 0.1f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 250);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    mesh_vert = Polygonate(mesh_vert);
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
    mesh_vert = Polygonate(mesh_vert);
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::rotate(model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Texture texture = GetTestBoxTexture(random_gen);
    std::unique_ptr<IterableMesh> it_mesh(new IterableRectPlane(4.0f, 4.0f));
    std::shared_ptr<MutationGenerator> mut =
        std::make_shared<FractalNoiseGenerator>(random_gen, 7, -0.5f, 0.7f);
    MutationMeshIterator mesh_iterator(17, 17, mut);
    mesh_iterator.SetIterableMesh(std::move(it_mesh));
    MeshVertices smooth_mesh_vert = mesh_iterator.GetMesh();
    Mesh smooth_mesh(smooth_mesh_vert.vertices, smooth_mesh_vert.indices,
                     {texture});
    std::unique_ptr<Model> smooth_generated_model(new Model({smooth_mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(-2.0f, -3.0f, 0));
    renderer->AddModel(std::move(smooth_generated_model), model_mat);

    MeshVertices poly_mesh_vert = Polygonate(smooth_mesh_vert);
    Mesh poly_mesh(poly_mesh_vert.vertices, poly_mesh_vert.indices, {texture});
    std::unique_ptr<Model> poly_generated_model(new Model({poly_mesh}));
    model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(2.0f, -3.0f, 0));
    model_mat = glm::scale(model_mat, glm::vec3(-1.0f, 1.0f, 1.0f));
    renderer->AddModel(std::move(poly_generated_model), model_mat);
  }
  return renderer;
}

std::unique_ptr<RtRenderer> GetSponzaDemo(
    bool windowed_mode, std::default_random_engine* random_gen) {
  std::unique_ptr<RtRenderer> renderer(
      new PointShadowsDynamicRenderer(windowed_mode));
  renderer->Init("Sponza Demo");
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
