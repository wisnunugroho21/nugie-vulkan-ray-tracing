#pragma once

#include "../../vulkan/command/command_buffer.hpp"
#include "../../vulkan/device/device.hpp"
#include "../../vulkan/pipeline/graphic_pipeline.hpp"
#include "../../vulkan/buffer/buffer.hpp"
#include "../../vulkan/descriptor/descriptor.hpp"
#include "../../vulkan/swap_chain/swap_chain.hpp"
#include "../data/model/vertex_model.hpp"
#include "../utils/camera/camera.hpp"
#include "../general_struct.hpp"

#include <memory>
#include <vector>

namespace nugiEngine {
	class EngineForwardPassRenderSystem {
		public:
			EngineForwardPassRenderSystem(EngineDevice& device, std::shared_ptr<EngineRenderPass> renderPass, std::shared_ptr<EngineDescriptorSetLayout> descriptorSetLayouts);
			~EngineForwardPassRenderSystem();

			void render(std::shared_ptr<EngineCommandBuffer> commandBuffer, std::shared_ptr<VkDescriptorSet> descriptorSets, std::shared_ptr<EngineVertexModel> model);
		
		private:
			void createPipelineLayout(std::shared_ptr<EngineDescriptorSetLayout> descriptorSetLayouts);
			void createPipeline(std::shared_ptr<EngineRenderPass> renderPass);

			EngineDevice& appDevice;
			
			VkPipelineLayout pipelineLayout;
			std::unique_ptr<EngineGraphicPipeline> pipeline;
	};
}