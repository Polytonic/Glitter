#include "boids/simulation.hpp"

#include <iostream>

#include "GLFW/glfw3.h"
#include "glitter.hpp"

#include "boids/character.hpp"
#include "glm/gtx/quaternion.hpp"
#include "rt_render_util.hpp"
#include "texture_gen.hpp"

namespace {

constexpr double min_speed = 1.0;
constexpr double max_speed = 10.0;
constexpr double max_acceleration = 2.0;

}  // namespace

DVec3 RandomPosition(std::default_random_engine* random_gen, double axis_min,
                     double axis_max) {
  std::uniform_real_distribution<double> dist(axis_min, axis_max);
  return DVec3(dist(*random_gen), dist(*random_gen), dist(*random_gen));
}

DVec3 RandomVelocity(std::default_random_engine* random_gen, double magnitude) {
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  DVec3 velocity(dist(*random_gen), dist(*random_gen), dist(*random_gen));
  return magnitude * glm::normalize(velocity);
}

BoidActor::BoidActor(DVec3 position, DVec3 velocity,
                     std::unique_ptr<Model> boid_model)
    : position_(position),
      velocity_(velocity),
      boid_model_(std::move(boid_model)) {}

void BoidActor::Tick(double delta_sec, const std::vector<BoidActor>& boids) {
  DVec3 average_pos(0.0);
  double total_weight = 0.0;
  for (const BoidActor& boid : boids) {
    if (&boid == this) {
      continue;
    }
    average_pos += boid.position();
    total_weight += 1;
  }
  average_pos = average_pos / total_weight;
  DVec3 acceleration = average_pos - position_;
  acceleration = max_acceleration * glm::normalize(acceleration);
  velocity_ += delta_sec * acceleration;
  last_acceleration_ = acceleration;
  if (glm::length(velocity_) < min_speed) {
    velocity_ = min_speed * glm::normalize(velocity_);
  }
  if (glm::length(velocity_) > max_speed) {
    velocity_ = max_speed * glm::normalize(velocity_);
  }
  position_ += delta_sec * velocity_;
}

void BoidActor::Draw(ShaderSet shaders, glm::mat4 model_mat) {
  glm::mat4 pos_mat = glm::translate(glm::mat4(1.0f), (glm::vec3)position());
  glm::mat4 rot_mat = glm::toMat4(glm::quatLookAt(
      glm::normalize(-1.0f * (glm::vec3)velocity()), glm::vec3(0, 1, 0)));
  boid_model_->Draw(shaders, model_mat * pos_mat * rot_mat);
}

BoidsSimulation::BoidsSimulation(std::default_random_engine random_gen,
                                 unsigned int num_boids)
    : random_gen_(random_gen) {
  for (int i = 0; i < num_boids; i++) {
    boids_.push_back(BoidActor(
        RandomPosition(&random_gen_, -2, 2), RandomVelocity(&random_gen_, 1.0),
        GetBoidCharacter(&random_gen_, GetRandomBasicColor(&random_gen_),
                         GetRandomBasicColor(&random_gen_),
                         GetRandomBasicColor(&random_gen_))));
  }
}

void BoidsSimulation::Tick(double delta_sec) {
  for (int i = 0; i < boids_.size(); i++) {
    boids_[i].Tick(delta_sec, boids_);
  }
}

void BoidsSimulation::Draw(ShaderSet shaders, glm::mat4 model_mat) {
  for (int i = 0; i < boids_.size(); i++) {
    boids_[i].Draw(shaders, model_mat);
  }
}

void BoidsSimulation::KeyboardEvents(GLFWwindow* window) {
  if (KeyNewlyPressed(window, &key_states_, GLFW_KEY_L)) {
    follow_boid_ = !follow_boid_;
  }
  if (KeyNewlyPressed(window, &key_states_, GLFW_KEY_LEFT_BRACKET)) {
    boid_to_follow_ = (boid_to_follow_ + (boids_.size() - 1)) % boids_.size();
  }
  if (KeyNewlyPressed(window, &key_states_, GLFW_KEY_RIGHT_BRACKET)) {
    boid_to_follow_ = (boid_to_follow_ + 1) % boids_.size();
  }
}

void BoidsSimulation::TickUpdateCamera(Camera* camera, double delta_time) {
  if (follow_boid_) {
    const BoidActor& boid = boids_[boid_to_follow_];
    DVec3 right = glm::cross(boid.velocity(), DVec3(0.0, 1.0, 0.0));
    DVec3 boid_up = glm::cross(right, boid.velocity());
    camera->SetPosition(boid.position() -
                        2.0 * glm::normalize(boid.velocity()) +
                        0.5 * glm::normalize(boid_up));
    camera->SetFront(glm::normalize(boid.velocity()));
  }
}
