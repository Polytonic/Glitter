#include "learnopengl/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM) {
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

void Camera::SetPosition(glm::vec3 pos) { Position = pos; }

void Camera::SetFront(glm::vec3 front) {
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
  Pitch = glm::degrees(std::asin(Front.y));
  Yaw = glm::degrees(std::atan2(Front.z, Front.x));
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD) Position += Front * velocity;
  if (direction == BACKWARD) Position -= Front * velocity;
  if (direction == LEFT) Position -= Right * velocity;
  if (direction == RIGHT) Position += Right * velocity;
  if (direction == UP) Position += Up * velocity;
  if (direction == DOWN) Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
  return;
  if (Zoom >= 1.0f && Zoom <= 45.0f) Zoom -= yoffset;
  if (Zoom <= 1.0f) Zoom = 1.0f;
  if (Zoom >= 45.0f) Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(
      Front, WorldUp));  // Normalize the vectors, because their length gets
                         // closer to 0 the more you look up or down which
                         // results in slower movement.
  Up = glm::normalize(glm::cross(Right, Front));
}
