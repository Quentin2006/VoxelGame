#include "game.hpp"
#include "camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "object.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>
#include <chrono>
#include <iostream>

Game::Game() { loadObjects(); }

Game::~Game() {}
double Game::getAvgFPS(float frameTime) {
  static double totalTime = 0.0;
  static uint64_t frameCount = 0;

  totalTime += frameTime;
  ++frameCount;

  return frameCount / totalTime;
}

void Game::run() {
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

    std::cout << "\rAVG FPS: " << std::fixed << getAvgFPS(frameTime)
              << std::flush;

    cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime,
                                   viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,
                      viewerObject.transform.rotation);

    float aspect = renderer.getAspectRatio();

    camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1, 1000);

    if (auto commandBuffer = renderer.beginFrame()) {
      renderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, objects, camera);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}

void Game::loadObjects() {

  std::shared_ptr<Model> model =
      Model::createModelFromFile(device, "./models/colored_cube.obj");

  Object obj = Object::createGameObject();

  obj.model = model;
  obj.transform.translation = {.0f, .0f, .0f};
  obj.transform.scale = {.5f, .5f, .5f};
  objects.push_back(std::move(obj));
}
