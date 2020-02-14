#ifndef EXAMPLE_SCENES_HPP
#define EXAMPLE_SCENES_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#include <functional>
#include <memory>
#include <random>

#include "rt_renderer.hpp"

using RtSceneFn = std::function<std::unique_ptr<RtRenderer>(
    bool windowed_mode, std::default_random_engine* random_gen)>;

std::unique_ptr<RtRenderer> HelixGarlicNanoScene(
    bool windowed_mode, std::default_random_engine* random_gen);

std::unique_ptr<RtRenderer> GetBoidsScene(
    bool windowed_mode, std::default_random_engine* random_gen);

std::unique_ptr<RtRenderer> GetBasicFractalNoiseDemo(
    bool windowed_mode, std::default_random_engine* random_gen);

std::unique_ptr<RtRenderer> GetSponzaDemo(
    bool windowed_mode, std::default_random_engine* random_gen);

#endif
