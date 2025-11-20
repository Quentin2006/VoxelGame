#pragma once

#include "device.hpp"
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

using std::string;
using std::vector;

struct PipelineConfigInfo {};

class Pipeline {
public:
  Pipeline(Device &device, const string &vertFilePath,
           const string &fragFilePath, const PipelineConfigInfo &configInfo);
  ~Pipeline() {}

  Pipeline(const Pipeline &) = delete;
  void operator=(const Pipeline &) = delete;

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

private:
  static vector<char> readFile(const string &filepath);

  void createGraphicsPipeline(const string &vertFilePath,
                              const string &fragFilePath,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(const vector<char> &code,
                          VkShaderModule *shaderModule);

  Device &device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};
