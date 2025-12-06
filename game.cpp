#include "game.hpp"
#include "model.hpp"

#include <array>
#include <stdexcept>

Game::Game() {
  loadModels();
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}

Game::~Game() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void Game::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(device.device());
}

void Game::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void Game::createPipeline() {
  auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapchain.width(),
                                                            swapchain.height());
  pipelineConfig.renderPass = swapchain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(
      device, "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv", pipelineConfig);
}

void Game::createCommandBuffers() {
  commandBuffers.resize(swapchain.imageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapchain.getRenderPass();
    renderPassInfo.framebuffer = swapchain.getFrameBuffer(i);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    pipeline->bind(commandBuffers[i]);

    model->bind(commandBuffers[i]);
    model->draw(commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }
}

void Game::drawFrame() {
  uint32_t imageIndex;
  auto result = swapchain.acquireNextImage(&imageIndex);
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }
  result =
      swapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

void Game::loadModels() {
  std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  // sierpinski({{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
  //            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
  //            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}, 3, vertices);

  model = std::make_unique<Model>(device, vertices);
}

void Game::sierpinski(Model::Vertex a, Model::Vertex b, Model::Vertex c,
                      int depth, std::vector<Model::Vertex> &points) {
  if (depth == 0) {
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    return;
  }

  // Compute midpoints of each side
  Model::Vertex ab = {{(a.position.x + b.position.x) * 0.5f,
                       (a.position.y + b.position.y) * 0.5f}};
  Model::Vertex bc = {{(b.position.x + c.position.x) * 0.5f,
                       (b.position.y + c.position.y) * 0.5f}};
  Model::Vertex ca = {{(c.position.x + a.position.x) * 0.5f,
                       (c.position.y + a.position.y) * 0.5f}};

  // Recurse on the 3 smaller triangles
  sierpinski(a, ab, ca, depth - 1, points);
  sierpinski(ab, b, bc, depth - 1, points);
  sierpinski(ca, bc, c, depth - 1, points);
}
