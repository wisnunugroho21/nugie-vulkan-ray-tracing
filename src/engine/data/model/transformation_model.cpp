#include "transformation_model.hpp"

#include <cstring>
#include <iostream>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace nugiEngine {
	EngineTransformationModel::EngineTransformationModel(EngineDevice &device, std::shared_ptr<std::vector<Transformation>> transformations) : engineDevice{device} {
		this->createBuffers(transformations);
	}

	EngineTransformationModel::EngineTransformationModel(EngineDevice &device, std::vector<std::shared_ptr<TransformComponent>> transformationComponents) : engineDevice{device} {
		this->createBuffers(this->convertToMatrix(transformationComponents));
	}

	std::shared_ptr<std::vector<Transformation>> EngineTransformationModel::convertToMatrix(std::vector<std::shared_ptr<TransformComponent>> transformations) {
		auto newTransforms = std::make_shared<std::vector<Transformation>>();
		for (auto &&transform : transformations) {
			newTransforms->emplace_back(Transformation{ 
				transform->getPointMatrix(),
				transform->getDirMatrix(), 
				transform->getPointInverseMatrix(), 
				transform->getDirInverseMatrix(), 
				transform->getNormalMatrix() 
			});
		}

		return newTransforms;
	}

	void EngineTransformationModel::createBuffers(std::shared_ptr<std::vector<Transformation>> transformations) {
		auto transformationBufferSize = sizeof(Transformation) * transformations->size();

		EngineBuffer transformationStagingBuffer {
			this->engineDevice,
			static_cast<VkDeviceSize>(transformationBufferSize),
			1,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		transformationStagingBuffer.map();
		transformationStagingBuffer.writeToBuffer(transformations->data());

		this->transformationBuffer = std::make_shared<EngineBuffer>(
			this->engineDevice,
			static_cast<VkDeviceSize>(transformationBufferSize),
			1,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		this->transformationBuffer->copyBuffer(transformationStagingBuffer.getBuffer(), static_cast<VkDeviceSize>(transformationBufferSize));
	} 
} // namespace nugiEngine

