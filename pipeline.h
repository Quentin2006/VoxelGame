#pragma once

#include "device.hpp"
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

using std::string;
using std::vector;

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};
;

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
