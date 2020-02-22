#include "bound.hpp"

#include <iostream>
#include <set>

std::optional<ShadeablePoint> BoundShape::Intersect(const Ray& ray) {
  std::set<std::pair<double, Intersectable*> > ordered_children;
  for (Intersectable* child : inter_children_) {
    std::optional<DVec3> inter = child->EarliestIntersect(ray);
    if (inter.has_value()) {
      ordered_children.insert({glm::distance(*inter, ray.origin), child});
    }
  }
  for (BoundPtr& child : bound_children_) {
    std::optional<DVec3> inter = child->EarliestIntersect(ray);
    if (inter.has_value()) {
      ordered_children.insert({glm::distance(*inter, ray.origin), child.get()});
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
      double curr = glm::distance(inter->point, ray.origin);
      if (curr < closest) {
        closest = curr;
        closest_inter = inter;
      }
    }
  }
  return closest_inter;
}

size_t BoundShape::RecursiveShadeableSize() const {
  size_t size = 0;
  for (const Intersectable* inter : inter_children_) {
    if (inter->IsShadeable()) {
      size++;
    }
  }
  for (const BoundPtr& bound : bound_children_) {
    size += bound->RecursiveShadeableSize();
  }
  return size;
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

void BoundBox::AddChild(BoundPtr child) {
  box_.Update(child->GetAaBox());
  BoundShape::AddChild(std::move(child));
}

std::optional<DVec3> BoundBox::EarliestIntersect(const Ray& ray) {
  return box_.EarliestIntersect(ray);
}

AaBox BoundBox::GetAaBox() const { return box_.GetAaBox(); }

DVec3 BoundBox::EstimateCenter() const { return box_.EstimateCenter(); }

double BoundBox::SurfaceArea() const { return box_.SurfaceArea(); }
