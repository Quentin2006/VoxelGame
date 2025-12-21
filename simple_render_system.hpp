#pragma once

#include "camera.hpp"
#include "device.hpp"
#include "object.hpp"
#include "pipeline.hpp"

// std
#include <memory>
#include <vector>

class SimpleRenderSystem {
public:
  SimpleRenderSystem(Device &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<Object> &gameObjects,
                         const Camera &camera);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  Device &device;

  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};
