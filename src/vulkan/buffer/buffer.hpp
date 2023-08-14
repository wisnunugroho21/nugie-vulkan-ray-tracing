#pragma once

#include <vk_mem_alloc.h>

#include "../device/device.hpp"
#include "../command/command_buffer.hpp"
 
namespace nugiEngine {
 
class EngineBuffer {
 public:
  EngineBuffer(
      EngineDevice& device,
      VkDeviceSize instanceSize,
      uint32_t instanceCount,
      VkBufferUsageFlags bufferUsage,
      VmaMemoryUsage memoryUsageFlags,
      VmaAllocationCreateFlags memoryPropertyFlags,
      VkDeviceSize minOffsetAlignment = 1);
  ~EngineBuffer();
 
  EngineBuffer(const EngineBuffer&) = delete;
  EngineBuffer& operator=(const EngineBuffer&) = delete;

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags memoryPropertyFlags);
  void copyBuffer(VkBuffer srcBuffer, VkDeviceSize size, std::shared_ptr<EngineCommandBuffer> commandBuffer = nullptr);
  void copyBufferToImage(VkImage image, uint32_t width, uint32_t height, uint32_t layerCount, std::shared_ptr<EngineCommandBuffer> commandBuffer = nullptr);
 
  VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  void unmap();
 
  void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  void readFromBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
 
  void writeToIndex(void* data, int index);
  void readFromIndex(void* data, int index);
  VkResult flushIndex(int index);
  VkDescriptorBufferInfo descriptorInfoForIndex(int index);
  VkResult invalidateIndex(int index);
 
  VkBuffer getBuffer() const { return buffer; }
  void* getMappedMemory() const { return mapped; }
  uint32_t getInstanceCount() const { return instanceCount; }

  VkDeviceSize getInstanceSize() const { return instanceSize; }
  VkDeviceSize getAlignmentSize() const { return instanceSize; }
  VkDeviceSize getBufferSize() const { return bufferSize; }
 
 private:
  static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);
 
  EngineDevice& engineDevice;

  void* mapped = nullptr;
  VkBuffer buffer = VK_NULL_HANDLE;

  VmaAllocation allocation;
  VmaAllocationInfo allocationInfo;
 
  VkDeviceSize bufferSize, instanceSize, alignmentSize;
  uint32_t instanceCount;
};
 
}  // namespace lve