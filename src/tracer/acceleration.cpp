#include "tracer/acceleration.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

namespace {

BoundPtr BoundInters(std::vector<Intersectable*> inters) {
  BoundPtr outer_bound(new BoundBox());
  for (Intersectable* inter : inters) {
    outer_bound->AddChild(inter);
  }
  return outer_bound;
}

AaBox BoxInters(std::vector<Intersectable*> inters) {
  AaBox box;
  for (Intersectable* inter : inters) {
    box.Update(inter->GetAaBox());
  }
  return box;
}

int TopDownTriplePickSplitIndex(const std::vector<Intersectable*>& inters,
                                int iteration) {
  return iteration % 3;
}

double PickTripleSplitPoint(const std::vector<Intersectable*>& inters,
                            AaBox box, int split_index) {
  double mean = (box.top()[split_index] + box.bot()[split_index]) / 2.0;
  return mean;
}

std::vector<std::vector<Intersectable*>> TripleSplit(
    const std::vector<Intersectable*>& inters, int split_index) {
  AaBox box = BoxInters(inters);
  double split_point = PickTripleSplitPoint(inters, box, split_index);
  std::vector<std::vector<Intersectable*>> vecs = {
      std::vector<Intersectable*>(), std::vector<Intersectable*>(),
      std::vector<Intersectable*>()};
  DVec3 bot = box.bot();
  DVec3 top = box.top();
  DVec3 mid_bot = box.bot();
  mid_bot[split_index] = split_point;
  DVec3 mid_top = box.top();
  mid_top[split_index] = split_point;
  AaBox less_box(bot, mid_top);
  AaBox more_box(mid_bot, mid_top);
  for (Intersectable* inter : inters) {
    if (less_box.Contains(*inter)) {
      vecs[0].push_back(inter);
    } else if (more_box.Contains(*inter)) {
      vecs[1].push_back(inter);
    } else {
      vecs[2].push_back(inter);
    }
  }
  return vecs;
}

BoundPtr TopDownTripleHelper(const BoundTopDownTripleOptions& opts,
                             const std::vector<Intersectable*>& inters,
                             int iteration) {
  if (inters.size() < opts.target_per_box) {
    // std::cerr << "Reached size " << inters.size() << std::endl;
    return BoundInters(inters);
  }
  if (iteration > opts.max_iterations) {
    // std::cerr << "Too many iterations with size " << inters.size() <<
    // std::endl;
    return BoundInters(inters);
  }
  int split_index = TopDownTriplePickSplitIndex(inters, iteration);
  std::vector<std::vector<Intersectable*>> vecs =
      TripleSplit(inters, split_index);
  BoundPtr wrapper(new BoundBox());
  for (const std::vector<Intersectable*>& vec : vecs) {
    if (!vec.empty()) {
      wrapper->AddChild(TopDownTripleHelper(opts, vec, iteration + 1));
    }
  }
  if (wrapper->size() == 1 && wrapper->bound_children().size() == 1) {
    return std::move(wrapper->bound_children()[0]);
  }
  return wrapper;
}

}  // namespace

BoundPtr ConstructBoundsNoAcceleration(const std::vector<InterPtr>* inters) {
  BoundPtr outer_bound(new BoundBox());
  for (const InterPtr& inter : *inters) {
    outer_bound->AddChild(inter.get());
  }
  return outer_bound;
}

BoundPtr ConstructBoundsTopDownTriple(const BoundTopDownTripleOptions& opts,
                                      const std::vector<InterPtr>* inters) {
  std::vector<Intersectable*> ptrs;
  for (const InterPtr& inter : *inters) {
    ptrs.push_back(inter.get());
  }
  return TopDownTripleHelper(opts, ptrs, 0);
}
