#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "tracer/intersectable.hpp"

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN,
};

// Default camera values
const double YAW = -90.0f;
const double PITCH = 0.0f;
const double SPEED = 5.0f;
const double SENSITIVITY = 0.1f;
const double ZOOM = 45.0f;

struct CameraArrangement {
  glm::vec3 position;
  glm::vec3 view_dir;
};

struct CameraTracerOpts {
  int h_px;
  int w_px;
  double focal_length;
  double focus_distance;
  double vert_fov;
  int subpix = 1;
};

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  // Camera Attributes
  DVec3 Position;
  DVec3 Front;
  DVec3 Up;
  DVec3 Right;
  DVec3 WorldUp;
  // Euler Angles
  double Yaw;
  double Pitch;
  // Camera options
  double MovementSpeed;
  double MouseSensitivity;
  double Zoom;

  // Constructor with vectors
  Camera(DVec3 position = DVec3(0.0f, 0.0f, 0.0f),
         DVec3 up = DVec3(0.0f, 1.0f, 0.0f), double yaw = YAW,
         double pitch = PITCH);

  // Constructor with scalar values
  Camera(double posX, double posY, double posZ, double upX, double upY,
         double upZ, double yaw, double pitch);

  DVec3 front() const { return Front; }
  DVec3 position() const { return Position; }
  const CameratracerOpts& opts() { return opts_; }

  void SetPosition(DVec3 pos);

  void SetFront(DVec3 front);

  void SetTracerOpts(CameraTracerOpts opts);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix();

  std::vector<Ray> GetScreenRays(int x_px, int y_px);

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(Camera_Movement direction, double deltaTime);

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(double xoffset, double yoffset,
                            GLboolean constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(double yoffset);

 private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();

  CameraTracerOpts opts_;

  double horiz_fov_;
};

#endif
