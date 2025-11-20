#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

using std::string;

class Window {
public:
  Window(int w, int h, string name);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  bool shouldClose();

  GLFWwindow *getGLFWwindow() const;

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  void initWindow();

  int width;
  int height;

  GLFWwindow *window;
  string windowName;
};
