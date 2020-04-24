#ifndef SHAPES_MUTATION_GENERATOR_HPP
#define SHAPES_MUTATION_GENERATOR_HPP

#include "learnopengl/glitter.hpp"

#include <random>
#include <vector>

class MutationGenerator {
 public:
  virtual double GetMutation(double u, double v) = 0;
};

class FractalNoiseGenerator : public MutationGenerator {
 public:
  // Note, not the actual max height!
  FractalNoiseGenerator(std::default_random_engine* random_gen, int iterations,
                        double peak_min_height, double peak_max_height);
  double GetMutation(double u, double v) override;

 private:
  void InitArea(int x_down, int y_down, int x_up, int y_up, int iteration);
  double GetRandDisplacement(int iteration);

  // Constructor parameters.
  std::default_random_engine* random_gen_;
  int iterations_;
  int num_points_;
  double peak_min_height_;
  double peak_max_height_;

  std::vector<std::vector<double>> data_;
};

#endif
