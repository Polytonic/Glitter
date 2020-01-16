#include "mutation_generator.hpp"

#include <cmath>
#include <iostream>

#include "interpolation.hpp"

namespace {

constexpr double kInitVal = 1000.0;
}

FractalNoiseGenerator::FractalNoiseGenerator(
    std::default_random_engine* random_gen, int iterations,
    double peak_min_height, double peak_max_height)
    : random_gen_(random_gen),
      iterations_(iterations),
      num_points_((1 << iterations_) + 1),
      peak_min_height_(peak_min_height),
      peak_max_height_(peak_max_height),
      data_(num_points_, std::vector<double>(num_points_, kInitVal)) {
  data_[0][0] = GetRandDisplacement(0);
  data_[0][num_points_ - 1] = GetRandDisplacement(0);
  data_[num_points_ - 1][0] = GetRandDisplacement(0);
  data_[num_points_ - 1][num_points_ - 1] = GetRandDisplacement(0);
  InitArea(0, 0, num_points_ - 1, num_points_ - 1, 1);
}

double FractalNoiseGenerator::GetMutation(double u, double v) {
  u = std::max(u, 0.0);
  v = std::max(v, 0.0);
  u = std::min(u, 1.0);
  v = std::min(v, 1.0);
  double x = u * (num_points_ - 1);
  double y = v * (num_points_ - 1);
  int x_down = (int)std::floor(x);
  int y_down = (int)std::floor(y);
  int x_up = (int)std::ceil(x);
  int y_up = (int)std::ceil(y);
  InterParam param;
  param.x1 = x_down;
  param.y1 = y_down;
  param.x2 = x_up;
  param.y2 = y_up;
  param.v11 = data_[x_down][y_down];
  param.v12 = data_[x_down][y_up];
  param.v21 = data_[x_up][y_down];
  param.v22 = data_[x_up][y_up];
  return InterpolateValue(param, {x, y});
}

void FractalNoiseGenerator::InitArea(int x_min, int y_min, int x_max, int y_max,
                                     int iteration) {
  if (iteration > iterations_) {
    return;
  }
  int x_inter = x_min + ((x_max - x_min) / 2);
  int y_inter = y_min + ((y_max - y_min) / 2);
  if (data_[x_inter][y_min] == kInitVal) {
    data_[x_inter][y_min] = (data_[x_min][y_min] + data_[x_max][y_min]) / 2.0 +
                            GetRandDisplacement(iteration);
  }
  if (data_[x_inter][y_max] == kInitVal) {
    data_[x_inter][y_max] = (data_[x_min][y_max] + data_[x_max][y_max]) / 2.0 +
                            GetRandDisplacement(iteration);
  }
  if (data_[x_min][y_inter] == kInitVal) {
    data_[x_min][y_inter] = (data_[x_min][y_min] + data_[x_min][y_max]) / 2.0 +
                            GetRandDisplacement(iteration);
  }
  if (data_[x_max][y_inter] == kInitVal) {
    data_[x_max][y_inter] = (data_[x_max][y_min] + data_[x_max][y_max]) / 2.0 +
                            GetRandDisplacement(iteration);
  }
  if (data_[x_inter][y_inter] == kInitVal) {
    data_[x_inter][y_inter] = (data_[x_min][y_min] + data_[x_min][y_max] +
                               data_[x_max][y_min] + data_[x_max][y_max]) /
                                  4.0 +
                              GetRandDisplacement(iteration);
  }
  InitArea(x_min, y_min, x_inter, y_inter, iteration + 1);
  InitArea(x_min, y_inter, x_inter, y_max, iteration + 1);
  InitArea(x_inter, y_min, x_max, y_inter, iteration + 1);
  InitArea(x_inter, y_inter, x_max, y_max, iteration + 1);
}

double FractalNoiseGenerator::GetRandDisplacement(int iteration) {
  double min, max;
  if (iteration == 0) {
    min = peak_min_height_;
    max = peak_max_height_;
  } else {
    iteration -= 1;
    iteration = std::max(iteration, 0);
    min = -1.0 * ((peak_max_height_ - peak_min_height_) / (1 << iteration));
    max = (peak_max_height_ - peak_min_height_) / (1 << iteration);
  }
  std::uniform_real_distribution<double> dist(min, max);
  return dist(*random_gen_);
}
