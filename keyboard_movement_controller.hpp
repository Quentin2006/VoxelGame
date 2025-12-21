#pragma once

#include "object.hpp"
#include <GLFW/glfw3.h>

struct KeyboardKeyMapping {
  int moveForward = GLFW_KEY_W;
  int moveBackward = GLFW_KEY_S;
  int moveLeft = GLFW_KEY_A;
  int moveRight = GLFW_KEY_D;
  int moveUp = GLFW_KEY_SPACE;
  int moveDown = GLFW_KEY_LEFT_SHIFT;
};

class KeyboardMovementController {
public:
  KeyboardMovementController(Window *window);

  void moveInPlaneXZ(GLFWwindow *window, float dt, Object &gameObject);

  KeyboardKeyMapping keys{};
  float moveSpeed{3.f};
  float lookSpeed{10.f};

  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

private:
  double dx{.0f};
  double dy{.0f};
  double prevX{.0f};
  double prevY{.0f};
  bool firstMouse{true};

  void handleMouse(double xpos, double ypos);

  Window *window;
};
