#include "keyboard_movement_controller.hpp"
#include <limits>

// static map from window to controller
static std::unordered_map<GLFWwindow *, KeyboardMovementController *>
    controllers;

KeyboardMovementController::KeyboardMovementController(Window *window)
    : window(window) {
  GLFWwindow *win = window->getGLFWwindow();
  controllers[win] = this;
  glfwSetCursorPosCallback(win, mouse_callback);
}

void KeyboardMovementController::mouse_callback(GLFWwindow *window, double xpos,
                                                double ypos) {
  auto it = controllers.find(window);
  if (it == controllers.end())
    return;
  KeyboardMovementController *self = it->second;
  self->handleMouse(xpos, ypos);
}

void KeyboardMovementController::handleMouse(double xpos, double ypos) {
  if (firstMouse) {
    prevX = xpos;
    prevY = ypos;
    firstMouse = false;
    return;
  }

  dx = xpos - prevX;
  dy = ypos - prevY;

  prevX = xpos;
  prevY = ypos;
}

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow *window, float dt,
                                               Object &gameObject) {
  glm::vec3 rotate{0};
  rotate.x -= dy;
  rotate.y += dx;

  dx = 0;
  dy = 0;

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.rotation += lookSpeed * dt * rotate;
  }

  gameObject.transform.rotation.x =
      glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
  gameObject.transform.rotation.y =
      glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

  float yaw = gameObject.transform.rotation.y;
  const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
  const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
  const glm::vec3 upDir{0.f, -1.f, 0.f};

  glm::vec3 moveDir{0.f};
  if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
    moveDir += forwardDir;
  if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
    moveDir -= forwardDir;
  if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
    moveDir -= rightDir;
  if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
    moveDir += rightDir;
  if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
    moveDir += upDir;
  if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
    moveDir -= upDir;
  if (glfwGetKey(window, keys.increaseLookSpeed) == GLFW_PRESS) {
    ++lookSpeed;
  }
  if (glfwGetKey(window, keys.decreaseLookSpeed) == GLFW_PRESS &&
      lookSpeed > 1) {
    --lookSpeed;
  }

  if (glfwGetKey(window, keys.increaseMoveSpeed) == GLFW_PRESS) {
    ++moveSpeed;
  }
  if (glfwGetKey(window, keys.decreaseMoveSpeed) == GLFW_PRESS &&
      moveSpeed > 1) {
    --moveSpeed;
  }

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.translation +=
        moveSpeed * dt * glm::normalize(moveDir);
  }
}
