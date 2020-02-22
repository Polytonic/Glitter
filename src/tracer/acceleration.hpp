#ifndef ACCELERATION_HPP
#define ACCELERATION_HPP

#include "tracer/bound.hpp"

struct BoundTopDownTripleOptions {
  int target_per_box = 5;
  int max_iterations = 20;
};

BoundPtr ConstructBoundsNoAcceleration(const std::vector<InterPtr>* inters);

BoundPtr ConstructBoundsTopDownTriple(const BoundTopDownTripleOptions& opts,
                                      const std::vector<InterPtr>* inters);

#endif
