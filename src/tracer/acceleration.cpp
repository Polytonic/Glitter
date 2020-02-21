#include "acceleration.hpp"

BoundPtr ConstructBounds(std::vector<InterPtr>* inters) {
  BoundPtr outer_bound(new BoundBox());
  for (const InterPtr& inter : *inters) {
    outer_bound->AddChild(inter.get());
  }
  return outer_bound;
}
