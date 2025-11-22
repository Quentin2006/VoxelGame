#include "window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using namespace std;

Window::Window(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name} {
  initWindow();
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

GLFWwindow *Window::getGLFWwindow() const { return window; }

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {

  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to craete window surface");
  }
}

void Window::initWindow() {

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
}
