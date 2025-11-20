#include "device.hpp"
#include "pipeline.h"
#include "window.h"

class Game {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 800;

  void run();

private:
  Window window{WIDTH, HEIGHT, "Test"};
  Device device{window};
  Pipeline pipeline{device, "shaders/simple_shader.vert.spv",
                    "shaders/simple_shader.frag.spv",
                    Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
