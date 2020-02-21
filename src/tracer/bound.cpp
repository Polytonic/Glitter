#include "bound.hpp"

#include <map>

std::optional<ShadeablePoint> BoundShape::Intersect(const Ray& ray) {
  std::map<double, Intersectable*> ordered_children;
  for (Intersectable* child : children_) {
    std::optional<DVec3> inter = child->EarliestIntersect(ray);
    if (inter.has_value()) {
      ordered_children[glm::distance2(*inter, ray.origin)] = child;
    }
  }
  double closest = 1e10;
  std::optional<ShadeablePoint> closest_inter;
  for (const auto& child : ordered_children) {
    if (closest_inter.has_value() && child.first > closest) {
      return closest_inter;
    }
    std::optional<ShadeablePoint> inter = child.second->Intersect(ray);
    if (inter.has_value()) {
      double curr = glm::distance2(inter->point, ray.origin);
      if (curr < closest) {
        closest = curr;
        closest_inter = inter;
      }
    }
  }
  return closest_inter;
}

void AddChildren(BoundShape* shape,
                 const std::vector<Intersectable*> children) {
  for (Intersectable* child : children) {
    shape->AddChild(child);
  }
}

BoundBox::BoundBox() : box_(DVec3(0)) {}

BoundBox::BoundBox(DVec3 bot, DVec3 top) : box_(bot, top) {}

void BoundBox::AddChild(Intersectable* child) {
  box_.Update(child->GetAaBox());
  BoundShape::AddChild(child);
}

std::optional<DVec3> BoundBox::EarliestIntersect(const Ray& ray) {
  return box_.EarliestIntersect(ray);
}

AaBox BoundBox::GetAaBox() { return box_.GetAaBox(); }

double BoundBox::SurfaceArea() const { return box_.SurfaceArea(); }
