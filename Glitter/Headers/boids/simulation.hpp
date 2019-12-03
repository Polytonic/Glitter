#ifndef BOIDS_SIMULATION_HPP
#define BOIDS_SIMULATION_HPP

#include <memory>
#include <random>
#include <unordered_map>

#include "glitter.hpp"

#include "dynamic_renderable.hpp"
#include "learnopengl/model.h"

DVec3 RandomPosition(std::default_random_engine* random_gen, double axis_min,
                     double axis_max);
DVec3 RandomVelocity(std::default_random_engine* random_gen, double magnitude);

class BoidActor {
 public:
  BoidActor(DVec3 position, DVec3 velocity);
  DVec3 position() const { return position_; }
  DVec3 velocity() const { return velocity_; }
  void Tick(double delta_sec, const std::vector<BoidActor>& boids);

 private:
  DVec3 position_;
  DVec3 velocity_;
  DVec3 last_acceleration_ = DVec3(0.0);
};

class BoidsSimulation : public DynamicRenderable, public CameraEventHandler {
 public:
  BoidsSimulation(std::default_random_engine random_gen,
                  unsigned int num_boids);

  void Tick(double delta_sec) override;
  void Draw(ShaderSet shaders, glm::mat4 model_mat) override;
  void KeyboardEvents(GLFWwindow* window) override;
  void TickUpdateCamera(Camera* camera, double delta_time) override;

 private:
  std::default_random_engine random_gen_;
  std::vector<BoidActor> boids_;
  std::unique_ptr<Model> boid_model_;
  std::unordered_map<int, bool> key_states_;

  bool follow_boid_ = false;
  int boid_to_follow_ = 0;
};

#endif
