#include "boids/simulation.hpp"

#include <cmath>
#include <iostream>

#include "GLFW/glfw3.h"
#include "glitter.hpp"

#include "boids/character.hpp"
#include "elementary_models.hpp"
#include "glm/gtx/quaternion.hpp"
#include "rt_render_util.hpp"
#include "texture_gen.hpp"

namespace {

BoidActor* first_boid;

const BoidPhysicsParams kDefaultPhysics = {
    /*min_speed=*/1.0,
    /*default_speed=*/5.0,
    /*max_speed=*/14.0,
    /*max_acceleration=*/10.0,
};

const double kNeighborThreshold = 10.0;

const BoidBehaviorParams kDefaultBehavior = {
    /*cage_distance=*/100.0,
    // The following is twice the distance required to decelerate at max
    // acceleration from max speed.
    /*cage_threshold=*/
    ((kDefaultPhysics.max_speed * kDefaultPhysics.max_speed) /
     (2 * kDefaultPhysics.max_acceleration)) *
        2,
    /*neighbor_threshold=*/
    kNeighborThreshold,
    // When plugged into the response equation, this cancels out the
    // distance squared (leading to max acceleration) at the point described
    // above.
    /*cage_response_multiplier=*/
    std::pow((kDefaultPhysics.max_speed * kDefaultPhysics.max_speed) /
                 (2 * kDefaultPhysics.max_acceleration),
             2.0),
    // When plugged into the response equation, this cancels out the distance
    // squared at the corresponding degress of separation, leading to max
    // acceleration.
    /*neighbor_collision_multiplier=*/
    std::pow(0.6, 2.0),
    // Use x * max acceleration to center when the flock is just inside our
    // neighbor threshold.
    /*centering_multiplier=*/1.0 / kNeighborThreshold,
    // Use max acceleration to match velocity when difference from average is
    // default_speed + max_speed and the other boid is x units away.
    /*velocity_multiplier=*/2.0 /
        (kDefaultPhysics.default_speed + kDefaultPhysics.max_speed),
};

DVec3 GetDampingAcceleration(const BoidPhysicsParams& params, DVec3 velocity) {
  double speed = glm::length(velocity);
  double too_fast_coeff =
      params.max_acceleration /
      std::pow((params.max_speed - params.default_speed), 2);
  double too_slow_coeff =
      params.max_acceleration /
      std::pow((params.default_speed - params.min_speed), 2);
  double damper;
  if (speed > params.default_speed) {
    damper = -1 * std::pow((speed - params.default_speed), 2) * too_fast_coeff;
  } else {
    damper = std::pow((params.default_speed - speed), 2) * too_slow_coeff;
  }
  return damper * glm::normalize(velocity);
}

std::unique_ptr<Model> GetBoundingSphere(double radius) {
  TexCanvas canvas = GetColorCanvas({255, 255, 255}, 400, 400);
  ApplyGrid(&canvas, 25, 25, 1, {0, 0, 255});
  Texture texture = canvas.ToTexture();

  std::unique_ptr<IterableMesh> it_mesh(new IterableSphere(radius));
  BasicMeshIterator mesh_iterator(50, 50);
  mesh_iterator.SetIterableMesh(std::move(it_mesh));
  MeshVertices mesh_vert = mesh_iterator.GetMesh();
  // ReverseNormals(&mesh_vert);
  glm::mat4 mesh_model_mat = glm::mat4(1.0f);
  return std::unique_ptr<Model>(new Model({Mesh(
      mesh_vert.vertices, mesh_vert.indices, {texture}, mesh_model_mat)}));
}

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
                     const BoidPhysicsParams& physics_params,
                     const BoidBehaviorParams& behavior,
                     std::unique_ptr<Model> boid_model)
    : position_(position),
      velocity_(velocity),
      physics_params_(physics_params),
      behavior_(behavior),
      boid_model_(std::move(boid_model)) {}

std::vector<DVec3> BoidActor::GetAvoidanceRequests(
    const std::vector<BoidActor>& boids) {
  std::vector<DVec3> requests;
  double from_center = glm::length(position_);
  double from_cage = behavior_.cage_distance - from_center;
  if (from_cage <= behavior_.cage_threshold) {
    DVec3 response =
        (-1.0 * physics_params_.max_acceleration *
         behavior_.cage_response_multiplier * glm::normalize(position_)) /
        glm::pow(from_cage, 2.0);
    requests.push_back(response);
  }

  for (const BoidActor& boid : boids) {
    if (&boid == this) {
      continue;
    }
    double separation = glm::distance(boid.position(), position());
    if (separation > behavior_.neighbor_threshold) {
      continue;
    }
    DVec3 response = (physics_params_.max_acceleration *
                      behavior_.neighbor_collision_multiplier *
                      glm::normalize(position() - boid.position())) /
                     glm::pow(separation, 2.0);
    requests.push_back(response);
  }
  return requests;
}

std::vector<DVec3> BoidActor::GetVelocityMatchingRequests(
    const std::vector<BoidActor>& boids) {
  std::vector<DVec3> requests;
  for (const BoidActor& boid : boids) {
    if (&boid == this) {
      continue;
    }
    double separation = glm::distance(boid.position(), position_);
    if (separation > behavior_.neighbor_threshold) {
      continue;
    }
    double weight = std::pow(separation, -2);
    DVec3 velocity_diff = boid.velocity() - velocity_;
    DVec3 request = weight * velocity_diff * physics_params_.max_acceleration *
                    behavior_.velocity_multiplier;
    requests.push_back(request);
  }
  return requests;
}

DVec3 BoidActor::GetCenteringRequest(const std::vector<BoidActor>& boids) {
  DVec3 average_pos(0.0);
  double total_weight = 0.0;
  for (const BoidActor& boid : boids) {
    if (&boid == this) {
      continue;
    }
    if (glm::distance(boid.position(), position()) >
        behavior_.neighbor_threshold) {
      continue;
    }
    average_pos += boid.position();
    total_weight += 1;
  }
  if (total_weight == 0) {
    return DVec3(0);
  }
  average_pos = average_pos / total_weight;
  DVec3 request = (average_pos - position_) * physics_params_.max_acceleration *
                  behavior_.centering_multiplier;
  return request;
}

namespace {
enum class ReqSource {
  kCentering,
  kVelocity,
  kAvoidance,
};

std::string ToString(ReqSource s) {
  switch (s) {
    case ReqSource::kCentering:
      return "center";
    case ReqSource::kVelocity:
      return "vlocty";
    case ReqSource::kAvoidance:
      return "avoids";
  }
  return "notfound";
}
}  // namespace

void BoidActor::Tick(double delta_sec, const std::vector<BoidActor>& boids) {
  DVec3 acceleration(0);
  std::vector<std::pair<DVec3, ReqSource>> requests;
  {
    DVec3 centering_request = GetCenteringRequest(boids);
    std::vector<DVec3> avoidance_requests = GetAvoidanceRequests(boids);
    std::vector<DVec3> velocity_requests = GetVelocityMatchingRequests(boids);
    requests.reserve(avoidance_requests.size() + velocity_requests.size() + 1);
    requests.push_back(
        std::make_pair(centering_request, ReqSource::kCentering));
    for (DVec3 e : avoidance_requests) {
      requests.push_back(std::make_pair(e, ReqSource::kAvoidance));
    }
    for (DVec3 e : velocity_requests) {
      requests.push_back(std::make_pair(e, ReqSource::kVelocity));
    }
  }

  std::sort(requests.begin(), requests.end(),
            [](const std::pair<DVec3, ReqSource>& a,
               const std::pair<DVec3, ReqSource>& b) {
              return glm::length2(a.first) > glm::length2(b.first);
            });
  int components = 0;
  std::string sources;

  bool maxed_out = false;
  double total_magnitude = 0;
  for (int i = 0; i < requests.size() && !maxed_out; i++) {
    components++;
    sources.append(ToString(requests[i].second) + ", ");
    acceleration += requests[i].first;
    total_magnitude += glm::length(requests[i].first);
    maxed_out = total_magnitude >= physics_params_.max_acceleration;
  }
  if (this == first_boid) {
    // std::cout << "components: " << components << " " << sources << std::endl;
  }

  if (glm::length(acceleration) >= physics_params_.max_acceleration) {
    acceleration =
        physics_params_.max_acceleration * glm::normalize(acceleration);
  }

  velocity_ += delta_sec * acceleration;
  last_acceleration_ = acceleration;
  DVec3 damping_acceleration =
      GetDampingAcceleration(physics_params_, velocity_);
  velocity_ += delta_sec * damping_acceleration;
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
  bounding_sphere_ = GetBoundingSphere(kDefaultBehavior.cage_distance);
  std::cout << "neighbor_threshold: " << kDefaultBehavior.neighbor_threshold
            << std::endl;
  std::cout << "cage_threshold: " << kDefaultBehavior.cage_threshold
            << std::endl;
  for (int i = 0; i < num_boids; i++) {
    boids_.push_back(BoidActor(
        RandomPosition(&random_gen_, -10, 10),
        RandomVelocity(&random_gen_, kDefaultPhysics.min_speed),
        kDefaultPhysics, kDefaultBehavior,
        GetBoidCharacter(&random_gen_, GetRandomBasicColor(&random_gen_),
                         GetRandomBasicColor(&random_gen_),
                         GetRandomBasicColor(&random_gen_))));
  }
  first_boid = &boids_[0];
}

void BoidsSimulation::Tick(double delta_sec) {
  for (int i = 0; i < boids_.size(); i++) {
    boids_[i].Tick(delta_sec, boids_);
  }
}

void BoidsSimulation::Draw(ShaderSet shaders, glm::mat4 model_mat) {
  bounding_sphere_->Draw(shaders, model_mat);
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
