#include "model.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

Model::Model(Device &device, const std::vector<Vertex> &vertices)
    : device(device) {
  createVertexBuffers(vertices);
}

Model::~Model() {
  vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
  vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void Model::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void Model::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be at least 3");

  // the total # of bytes we need to allocate
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

  // allocates memory on gpu
  device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      vertexBuffer, vertexBufferMemory);

  void *data;
  vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);

  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(device.device(), vertexBufferMemory);
}

std::vector<VkVertexInputBindingDescription>
Model::Vertex::getBindingDesctiptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescs(1);
  bindingDescs[0].binding = 0;
  bindingDescs[0].stride = sizeof(Vertex);
  bindingDescs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescs;
}

std::vector<VkVertexInputAttributeDescription>
Model::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescs(2);
  attributeDescs[0].binding = 0;
  attributeDescs[0].location = 0;
  attributeDescs[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescs[0].offset = offsetof(Vertex, position);

  attributeDescs[1].binding = 0;
  attributeDescs[1].location = 1;
  attributeDescs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescs[1].offset = offsetof(Vertex, color);

  return attributeDescs;
}
