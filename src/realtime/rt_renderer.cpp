#include "realtime/rt_renderer.hpp"

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

void RtRenderer::GetTris(std::vector<InterPtr>* tris) {
  for (int i = 0; i < static_models_.size(); i++) {
    static_models_[i]->GetTris(static_model_matrices_[i], tris);
  }
  for (const std::unique_ptr<DynamicRenderable>& model : dynamic_models_) {
    model->GetTris(glm::mat4(1.0f), tris);
  }
}
