#include "game.h"
#include <GLFW/glfw3.h>

void Game::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window.getGLFWwindow());
  }
}
