#include "game.h"

void Game::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
    glfwSwapBuffers(window.getGLFWwindow());
  }
}
