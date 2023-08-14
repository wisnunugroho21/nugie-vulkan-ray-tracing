#include "transformation_model.hpp"

#include <cstring>
#include <iostream>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace nugiEngine {
	EngineTransformationModel::EngineTransformationModel(EngineDevice &device, std::shared_ptr<std::vector<Transformation>> transformations, std::shared_ptr<EngineCommandBuffer> commandBuffer) : engineDevice{device} {
		this->createBuffers(transformations, commandBuffer);
	}

	EngineTransformationModel::EngineTransformationModel(EngineDevice& device, std::shared_ptr<std::vector<TransformComponent>> transformationComponents, std::shared_ptr<EngineCommandBuffer> commandBuffer) : engineDevice{device} {
		this->createBuffers(this->convertToMatrix(transformationComponents), commandBuffer);
	}

	std::shared_ptr<std::vector<Transformation>> EngineTransformationModel::convertToMatrix(std::shared_ptr<std::vector<TransformComponent>> transformationComponents) {
		auto transforms = std::make_shared<std::vector<Transformation>>();
		for (uint32_t i = 0; i < transformationComponents->size(); i++) {
			transforms->emplace_back(Transformation{ 
				(*transformationComponents)[i].getPointMatrix(),
				(*transformationComponents)[i].getDirMatrix(), 
				(*transformationComponents)[i].getPointInverseMatrix(), 
				(*transformationComponents)[i].getDirInverseMatrix(), 
				(*transformationComponents)[i].getNormalMatrix() 
			});
		}

		return transforms;
	}

	void EngineTransformationModel::createBuffers(std::shared_ptr<std::vector<Transformation>> transformations, std::shared_ptr<EngineCommandBuffer> commandBuffer) {
		auto transformsCount = static_cast<uint32_t>(transformations->size());
		auto transformsSize = static_cast<uint32_t>(sizeof(Transformation));

		EngineBuffer transformationStagingBuffer {
			this->engineDevice,
			static_cast<VkDeviceSize>(transformsSize),
			transformsCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VMA_MEMORY_USAGE_AUTO,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
		};

		transformationStagingBuffer.map();
		transformationStagingBuffer.writeToBuffer(transformations->data());

		this->transformationBuffer = std::make_shared<EngineBuffer>(
			this->engineDevice,
			static_cast<VkDeviceSize>(transformsSize),
			transformsCount,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VMA_MEMORY_USAGE_AUTO,
			VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT
		);

		this->transformationBuffer->copyBuffer(transformationStagingBuffer.getBuffer(), static_cast<VkDeviceSize>(transformsSize * transformsCount), commandBuffer);
	} 
} // namespace nugiEngine

