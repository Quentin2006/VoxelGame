#include "game.hpp"
#include <GLFW/glfw3.h>

void Game::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
  }
}
