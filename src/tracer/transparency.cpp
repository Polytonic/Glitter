#include "tracer/transparency.hpp"

#include <algorithm>
#include <optional>

void InsideModelStack::Push(const ShadeablePoint& point) {
  const Model* model = point.shape->GetParentModel();
  auto iter_pair = inside_map_.insert({model, point});
  if (!iter_pair.second) {
    iter_pair.first->second = point;
    RemoveFromVector(model);
  }
  inside_stack_.push_back({model, point});
}

bool InsideModelStack::Contains(const Model* model) {
  return inside_map_.find(model) != inside_map_.end();
}

std::optional<ShadeablePoint> InsideModelStack::Pop(const Model* model) {
  auto iter = inside_map_.find(model);
  if (iter != inside_map_.end()) {
    RemoveFromVector(model);
    inside_map_.erase(model);
    return iter->second;
  } else {
    return std::nullopt;
  }
}

std::optional<ShadeablePoint> InsideModelStack::Get(const Model* model) {
  auto iter = inside_map_.find(model);
  if (iter != inside_map_.end()) {
    return iter->second;
  } else {
    return std::nullopt;
  }
}

Material* InsideModelStack::CurrentMaterial() {
  if (inside_stack_.empty()) {
    return &air_;
  } else {
    return inside_stack_.back().second.shape->material();
  }
}

void InsideModelStack::RemoveFromVector(const Model* model) {
  inside_stack_.erase(
      std::remove_if(inside_stack_.begin(), inside_stack_.end(),
                     [model](const std::pair<const Model*, ShadeablePoint>& m) {
                       return m.first == model;
                     }),
      inside_stack_.end());
}

Material InsideModelStack::air_ = Material(Texture(), Material::Options({
                                                          .transparency = 1.0,
                                                          .index = 1.0003,
                                                          .reflectivity = 0.0,
                                                      }));