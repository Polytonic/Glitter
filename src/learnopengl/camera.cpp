#include "learnopengl/camera.h"

Camera::Camera(DVec3 position, DVec3 up, double yaw, double pitch)
    : Front(DVec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(double posX, double posY, double posZ, double upX, double upY,
               double upZ, double yaw, double pitch)
    : Front(DVec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM) {
  Position = DVec3(posX, posY, posZ);
  WorldUp = DVec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

void Camera::SetPosition(DVec3 pos) { Position = pos; }

void Camera::SetFront(DVec3 front) {
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
  Pitch = glm::degrees(std::asin(Front.y));
  Yaw = glm::degrees(std::atan2(Front.z, Front.x));
}

void Camera::SetTracerOpts(CameraTracerOpts opts) {
  opts_ = opts;
  horiz_fov_ = opts_.vert_fov * ((double)opts_.w_px / opts_.h_px);
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

std::vector<Ray> Camera::GetScreenRays(int x_px, int y_px) {
  DVec3 screen_pos = Position + opts_.focal_length * Front;
  DVec3 lens = screen_pos + opts_.focal_length * Front;
  DVec3 screen_vert = opts_.focal_length * std::tan(opts_.vert_fov / 2.0) * Up;
  DVec3 screen_horiz = opts_.focal_length * std::tan(horiz_fov_ / 2.0) * Right;

  std::vector<DVec2> screen_space_px;
  double inv_subpix = 1.0 / opts_.subpix;
  if (opts_.subpix == 1) {
    DVec2 screen_space;
    screen_space.x = (x_px + 0.5) / opts_.w_px;
    screen_space.y = (y_px + 0.5) / opts_.h_px;
    screen_space_px.push_back(screen_space);
  } else {
    for (int subY = 0; subY < opts_.subpix; subY++) {
      for (int subX = 0; subX < opts_.subpix; subX++) {
        DVec2 screen_space;
        screen_space.x =
            (x_px + ((double)subX / opts_.subpix) + 0.5 * inv_subpix) /
            opts_.w_px;
        screen_space.y =
            (y_px + ((double)subY / opts_.subpix) + 0.5 * inv_subpix) /
            opts_.h_px;
        screen_space_px.push_back(screen_space);
      }
    }
  }

  std::vector<Ray> rays;
  for (DVec2 screen_space : screen_space_px) {
    DVec3 screen_point = screen_pos + (2 * screen_space.x - 1) * screen_horiz -
                         (2 * screen_space.y - 1) * screen_vert;
    DVec3 pix_vec = glm::normalize(screen_point - Position);
    rays.push_back({Position, pix_vec});
  }
  return rays;
}

void Camera::ProcessKeyboard(Camera_Movement direction, double deltaTime) {
  double velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD) Position += Front * velocity;
  if (direction == BACKWARD) Position -= Front * velocity;
  if (direction == LEFT) Position -= Right * velocity;
  if (direction == RIGHT) Position += Right * velocity;
  if (direction == UP) Position += Up * velocity;
  if (direction == DOWN) Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(double xoffset, double yoffset,
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

void Camera::ProcessMouseScroll(double yoffset) {
  return;
  if (Zoom >= 1.0f && Zoom <= 45.0f) Zoom -= yoffset;
  if (Zoom <= 1.0f) Zoom = 1.0f;
  if (Zoom >= 45.0f) Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  DVec3 front;
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
