#include "forward_pass_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <string>

namespace nugiEngine {
	EngineForwardPassRenderSystem::EngineForwardPassRenderSystem(EngineDevice& device, std::shared_ptr<EngineRenderPass> renderPass, std::shared_ptr<std::vector<EngineDescriptorSetLayout>> descriptorSetLayouts)
		: appDevice{device}
	{
		this->createPipelineLayout(descriptorSetLayouts);
		this->createPipeline(renderPass);
	}

	EngineForwardPassRenderSystem::~EngineForwardPassRenderSystem() {
		vkDestroyPipelineLayout(this->appDevice.getLogicalDevice(), this->pipelineLayout, nullptr);
	}

	void EngineForwardPassRenderSystem::createPipelineLayout(std::shared_ptr<std::vector<EngineDescriptorSetLayout>> descriptorSetLayouts) {
		std::vector<VkDescriptorSetLayout> descSetLayouts{};
		for (uint32_t i = 0; i < descriptorSetLayouts->size(); i++) {
			descSetLayouts.emplace_back((*descriptorSetLayouts)[i].getDescriptorSetLayout());
		}
		
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(this->appDevice.getLogicalDevice(), &pipelineLayoutInfo, nullptr, &this->pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void EngineForwardPassRenderSystem::createPipeline(std::shared_ptr<EngineRenderPass> renderPass) {
		assert(this->pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		this->pipeline = EngineGraphicPipeline::Builder(this->appDevice, renderPass, this->pipelineLayout)
			.setDefault("shader/forward_pass.vert.spv", "shader/forward_pass.frag.spv")
			.setMultisampleInfo(multisampleInfo)
			.build();
	}

	void EngineForwardPassRenderSystem::render(std::shared_ptr<EngineCommandBuffer> commandBuffer, std::shared_ptr<std::vector<VkDescriptorSet>> descriptorSets, std::shared_ptr<EngineVertexModel> model) {
		this->pipeline->bind(commandBuffer->getCommandBuffer());

		vkCmdBindDescriptorSets(
			commandBuffer->getCommandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			this->pipelineLayout,
			0,
			static_cast<uint32_t>(descriptorSets->size()),
			descriptorSets->data(),
			0,
			nullptr
		);

		model->bind(commandBuffer);
		model->draw(commandBuffer);
	}
}