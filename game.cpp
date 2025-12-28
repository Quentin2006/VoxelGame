#include "game.hpp"
#include "buffer.hpp"
#include "camera.hpp"
#include "frame_info.hpp"
#include "keyboard_movement_controller.hpp"
#include "object.hpp"
#include "simple_render_system.hpp"
#include "swapchain.hpp"

#include <cstdint>
#include <glm/geometric.hpp>
#include <memory>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>
#include <chrono>
#include <iostream>

struct GlobalUbo {
  glm::mat4 projectionView{1.f};
  glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

Game::Game() { loadObjects(); }

Game::~Game() {}
double Game::getFPS(float frameTime) {
  static double totalTime = 0.0;
  static uint64_t frameCount = 0;

  totalTime += frameTime;
  ++frameCount;

  if (totalTime >= 1) {
    int fps = frameCount / totalTime;
    std::cout << "\rFPS: " << fps << std::flush;
    totalTime = 0;
    frameCount = 0;
  }

  return frameCount / totalTime;
}

void Game::run() {
  std::vector<std::unique_ptr<Buffer>> uboBuffers(
      SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (auto &uboBuffer : uboBuffers) {
    uboBuffer = std::make_unique<Buffer>(device, sizeof(GlobalUbo), 1,
                                         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffer->map();
  };
  SimpleRenderSystem simpleRenderSystem{device,
                                        renderer.getSwapChainRenderPass()};

  Camera camera{};

  auto viewerObject = Object::createGameObject();
  KeyboardMovementController cameraController{&window};

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!window.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();

    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime -
                                                                   currentTime)
            .count();

    currentTime = newTime;

    getFPS(frameTime);

    cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime,
                                   viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,
                      viewerObject.transform.rotation);

    float aspect = renderer.getAspectRatio();

    camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1, 1000);

    if (auto commandBuffer = renderer.beginFrame()) {
      int frameIndex = renderer.getFrameIndex();
      FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};
      // update
      GlobalUbo ubo{};
      ubo.projectionView = camera.getProjection() * camera.getView();
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      renderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(frameInfo, objects);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}

void Game::makeFlatWorld(int xSize, int zSize, int yHeight) {
  std::shared_ptr<Model> model =
      Model::createModelFromFile(device, "./models/cube.obj");

  for (int x = -xSize / 2; x < xSize / 2; ++x) {
    for (int z = -zSize / 2; z < zSize / 2; ++z) {
      Object obj = Object::createGameObject();
      obj.model = model;
      obj.transform.scale = .5f;
      obj.transform.translation = {x, yHeight, z};
      objects.push_back(std::move(obj));
    }
  }
}

void Game::loadObjects() { makeFlatWorld(50, 50, 5); }
