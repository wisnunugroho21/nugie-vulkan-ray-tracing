#include "material_model.hpp"

#include <cstring>
#include <iostream>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace nugiEngine {
	EngineMaterialModel::EngineMaterialModel(EngineDevice &device, std::shared_ptr<std::vector<Material>> materials, std::shared_ptr<EngineCommandBuffer> commandBuffer) : engineDevice{device} {
		this->createBuffers(materials, commandBuffer);
	}

	void EngineMaterialModel::createBuffers(std::shared_ptr<std::vector<Material>> materials, std::shared_ptr<EngineCommandBuffer> commandBuffer) {
		auto materialCount = static_cast<uint32_t>(materials->size());
		auto materialSize = static_cast<uint32_t>(sizeof(Transformation));

		EngineBuffer materialStagingBuffer {
			this->engineDevice,
			static_cast<VkDeviceSize>(materialSize),
			materialCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VMA_MEMORY_USAGE_AUTO,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
		};

		materialStagingBuffer.map();
		materialStagingBuffer.writeToBuffer(materials->data());

		this->materialBuffer = std::make_shared<EngineBuffer>(
			this->engineDevice,
			static_cast<VkDeviceSize>(materialSize),
			materialCount,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VMA_MEMORY_USAGE_AUTO,
			VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT
		);

		this->materialBuffer->copyBuffer(materialStagingBuffer.getBuffer(), static_cast<VkDeviceSize>(materialSize * materialCount), commandBuffer);
	} 
} // namespace nugiEngine

