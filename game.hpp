#pragma once

#include "device.hpp"
#include "model.hpp"
#include "pipeline.hpp"
#include "swapchain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

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
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();
  void loadModels();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void freeCommandBuffers();

  void sierpinski(Model::Vertex a, Model::Vertex b, Model::Vertex c, int depth,
                  std::vector<Model::Vertex> &points);

  Window window{WIDTH, HEIGHT, "Hello Vulkan!"};
  Device device{window};
  std::unique_ptr<SwapChain> swapChain;
  std::unique_ptr<Pipeline> pipeline;

  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;

  std::unique_ptr<Model> model;
};
