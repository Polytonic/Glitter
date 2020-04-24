#ifndef SHAPES_ONION_HPP
#define SHAPES_ONION_HPP

#include "shapes/iterable_mesh.hpp"

class IterableOnion : public IterableMesh {
 public:
  IterableOnion(double outer_radius,
		double inner_radius,
		int cloves);

  ComputedVertex GetVertex(double u, double v) override;

  bool IsClosed() override { return true; }

 private:
  double outer_radius_;
  double inner_radius_;
  int cloves_;
  double clove_width_;
};

MeshVertices GetGarlic(double outer_radius,
		       double inner_radius,
		       int cloves,
		       double clove_res,
		       double height_res);

#endif
