#include "pipeline.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using namespace std;

Pipeline::Pipeline(Device &_device, const string &vertFilePath,
                   const string &fragFilePath,
                   const PipelineConfigInfo &configInfo)
    : device(_device) {
  createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

vector<char> Pipeline::readFile(const string &filepath) {
  ifstream file{filepath, ios::ate | ios::binary};

  if (!file.is_open()) {
    throw runtime_error("Failed to open file " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());

  vector<char> buffer(fileSize);

  file.seekg(0);

  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width,
                                                       uint32_t height) {
  PipelineConfigInfo configInfo{};

  return configInfo;
}

void Pipeline::createGraphicsPipeline(const string &vertFilePath,
                                      const string &fragFilePath,
                                      const PipelineConfigInfo &configInfo) {
  auto vertCode = readFile(vertFilePath);
  auto fragCode = readFile(fragFilePath);

  cout << "VERT SIZE: " << vertCode.size() << " FRAG SIZE: " << fragCode.size()
       << endl;
}

void Pipeline::createShaderModule(const vector<char> &code,
                                  VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(device.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw runtime_error("Failed to create shader module");
  }
}
