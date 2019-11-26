#ifndef MUTATION_GENERATOR_HPP
#define MUTATION_GENERATOR_HPP

#include "glitter.hpp"

#include <random>
#include <vector>

class MutationGenerator {
 public:
  virtual float GetMutation(float u, float v) = 0;
};

class FractalNoiseGenerator : public MutationGenerator {
 public:
  // Note, not the actual max height!
  FractalNoiseGenerator(std::default_random_engine* random_gen, int iterations,
                        float peak_min_height, float peak_max_height);
  float GetMutation(float u, float v) override;

 private:
  void InitArea(int x_down, int y_down, int x_up, int y_up, int iteration);
  float GetRandDisplacement(int iteration);

  // Constructor parameters.
  std::default_random_engine* random_gen_;
  int iterations_;
  int num_points_;
  float peak_min_height_;
  float peak_max_height_;

  std::vector<std::vector<float>> data_;
};

#endif
