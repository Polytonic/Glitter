#define _USE_MATH_DEFINES
#include <cmath>

#include "shapes/mesh_iterator.hpp"
#include "shapes/onion.hpp"

IterableOnion::IterableOnion(double outer_radius,
			     double inner_radius,
			     int cloves)
  : outer_radius_(outer_radius),
    inner_radius_(inner_radius),
    cloves_(cloves),
    clove_width_(1.0 / cloves_) {}

ComputedVertex IterableOnion::GetVertex(double u, double v) {
  ComputedVertex vert;
  double r;
  {
    double theta = v * -1 * M_PI - (M_PI / 2.0);
    // http://mathworld.wolfram.com/HeartCurve.html
    r = 2 - 2 * std::sin(theta) + std::sin(theta) *
      (std::sqrt(std::abs(std::cos(theta))) / (std::sin(theta) + 1.4));
  }
  double theta = u * 2.0 * M_PI;
  double phi = v * M_PI;
  DVec3 polar_vec;
  polar_vec.x = std::sin(theta) * std::sin(phi);
  polar_vec.y = std::cos(phi);
  polar_vec.z = std::cos(theta) * std::sin(phi);

  double clove_fraction = std::fmod(u, clove_width_) / clove_width_;
  double wall_interpol = std::sin(clove_fraction * M_PI);
  double radius = inner_radius_ +
    wall_interpol * (outer_radius_ - inner_radius_);

  vert.position = r * radius * polar_vec;
  return vert;
}

MeshVertices GetGarlic(double outer_radius,
		       double inner_radius,
		       int cloves,
		       double clove_res,
  		       double height_res){
  std::unique_ptr<IterableMesh> it_mesh(new IterableOnion(outer_radius,
							  inner_radius,
							  cloves));
  CalcNormalsMeshIterator mesh_iterator(cloves * clove_res, height_res);
  mesh_iterator.SetIterableMesh(std::move(it_mesh));
  return mesh_iterator.GetMesh();
}
