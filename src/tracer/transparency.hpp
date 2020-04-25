#ifndef TRACER_TRANSPARENCY_HPP
#define TRACER_TRANSPARENCY_HPP

#include <list>
#include <utility>
#include <map>
#include <optional>

#include "tracer/intersectable.hpp"

class Model;

class InsideModelStack {
public:
  void Push(const ShadeablePoint& point);
  bool Contains(const Model* model);
  std::optional<ShadeablePoint> Pop(const Model* model);
  std::optional<ShadeablePoint> Get(const Model* model);
  // Returns the material of the most recently pushed model
  // that has not been popped. If the stack is empty, returns
  // a pointer to the air material.
  Material* CurrentMaterial();

 private:
void RemoveFromVector(const Model* model);

  std::list<std::pair<const Model*, ShadeablePoint>> inside_stack_;
  std::map<const Model*, ShadeablePoint> inside_map_;
  static Material air_;
};

#endif