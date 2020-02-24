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
    if (child->GetAaBox().Contains(ray.origin)) {
      ordered_children.insert({0.0, child.get()});
      continue;
    }
    std::optional<DVec3> inter = child->EarliestIntersect(ray);
    if (inter.has_value()) {
      ordered_children.insert({glm::distance(*inter, ray.origin), child.get()});
    }
  }
  double closest = 1e100;
  std::optional<ShadeablePoint> closest_inter;
  for (const auto& child : ordered_children) {
    if (closest_inter.has_value() && child.first - 1e-5 > closest) {
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

void BoundShape::RecursiveAssertSanity() const {
  AaBox my_box = GetAaBox();
  for (const auto& child : inter_children_) {
    if (!my_box.Contains(*child)) {
      std::cerr << "RecursiveAssertSanity failed on inter_children_"
                << std::endl;
      exit(-1);
    }
  }
  for (const auto& child : bound_children_) {
    if (!my_box.Contains(*child)) {
      std::cerr << "RecursiveAssertSanity failed on bound_children_"
                << std::endl;
      exit(-1);
    }
  }
  for (const auto& child : bound_children_) {
    child->RecursiveAssertSanity();
  }
}

void AddChildren(BoundShape* shape,
                 const std::vector<Intersectable*> children) {
  for (Intersectable* child : children) {
    shape->AddChild(child);
  }
}

BoundBox::BoundBox() {}

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

AaBox BoundBox::GetAaBox() const { return box_; }

DVec3 BoundBox::EstimateCenter() const { return box_.EstimateCenter(); }

double BoundBox::SurfaceArea() const { return box_.SurfaceArea(); }
