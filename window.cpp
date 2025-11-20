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

    throw std::runtime_error("Failed to create window surface");
  }
}

void Window::initWindow() {
  // NOTE: Window not appearing becuse of odd wayland things
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

  if (window == nullptr) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window);
}
