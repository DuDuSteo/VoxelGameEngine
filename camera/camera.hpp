#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <vector>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum class Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

class Camera
{
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  float lastX;
  float lastY;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);

  glm::mat4 GetViewMatrix();
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);
  void ProcessKeyboard(float xpos, float ypos, float deltaTime,
                       bool firstMouse);
  void ProcessMouseMovement(float xpos, float ypos, bool firstMouse,
                            bool constrainPitch = true);
  void ProcessMouseScroll(float yoffset, float deltaTime);

private:
  void updateCameraVectors();
};