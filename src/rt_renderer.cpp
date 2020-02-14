#include "rt_renderer.hpp"

#include "learnopengl/filesystem.h"

void RtRenderer::AddModel(const std::string& file_path,
                          glm::mat4 model_matrix) {
  static_models_.push_back(
      std::unique_ptr<Model>(new Model(FileSystem::getPath(file_path))));
  static_model_matrices_.push_back(model_matrix);
}

void RtRenderer::AddModel(std::unique_ptr<Renderable> model,
                          glm::mat4 model_matrix) {
  static_models_.push_back(std::move(model));
  static_model_matrices_.push_back(model_matrix);
}

void RtRenderer::AddDynamicModel(std::unique_ptr<DynamicRenderable> model) {
  dynamic_models_.push_back(std::move(model));
}
