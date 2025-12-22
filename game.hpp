#pragma once

#include "device.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <vector>
#include <vulkan/vulkan_core.h>

class Game {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 800;

  Game();
  ~Game();

  Game(const Game &) = delete;
  Game operator=(const Game &) = delete;

  void run();

private:
  void loadObjects();
  double getFPS(float frameTime);

  Window window{WIDTH, HEIGHT, "Hello Vulkan!"};
  Device device{window};
  Renderer renderer{window, device};

  std::vector<Object> objects;
};
