#ifndef EXAMPLE_SCENES_HPP
#define EXAMPLE_SCENES_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#include <functional>
#include <memory>
#include <random>

#include "rt_renderer.hpp"

using RtSceneFn = std::function<std::unique_ptr<RtRenderer>(
    std::default_random_engine* random_gen)>;

std::unique_ptr<RtRenderer> InProgressScene(
    std::default_random_engine* random_gen);

std::unique_ptr<RtRenderer> GetBoidsScene(
    std::default_random_engine* random_gen);

std::unique_ptr<RtRenderer> GetBasicFractalNoiseDemo(
    std::default_random_engine* random_gen);

std::unique_ptr<RtRenderer> GetSponzaDemo(
    std::default_random_engine* random_gen);

#endif
