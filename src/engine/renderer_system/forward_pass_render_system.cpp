#include "forward_pass_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <string>

namespace nugiEngine {
	EngineForwardPassRenderSystem::EngineForwardPassRenderSystem(EngineDevice& device, std::shared_ptr<EngineRenderPass> renderPass, std::shared_ptr<EngineDescriptorSetLayout> descriptorSetLayouts)
		: appDevice{device}
	{
		this->createPipelineLayout(descriptorSetLayouts);
		this->createPipeline(renderPass);
	}

	EngineForwardPassRenderSystem::~EngineForwardPassRenderSystem() {
		vkDestroyPipelineLayout(this->appDevice.getLogicalDevice(), this->pipelineLayout, nullptr);
	}

	void EngineForwardPassRenderSystem::createPipelineLayout(std::shared_ptr<EngineDescriptorSetLayout> descriptorSetLayout) {
		VkDescriptorSetLayout descSetLayout = descriptorSetLayout->getDescriptorSetLayout();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1u;
		pipelineLayoutInfo.pSetLayouts = &descSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(this->appDevice.getLogicalDevice(), &pipelineLayoutInfo, nullptr, &this->pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void EngineForwardPassRenderSystem::createPipeline(std::shared_ptr<EngineRenderPass> renderPass) {
		assert(this->pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		this->pipeline = EngineGraphicPipeline::Builder(this->appDevice, renderPass, this->pipelineLayout)
			.setDefault("shader/forward_pass.vert.spv", "shader/forward_pass.frag.spv")
			.build();
	}

	void EngineForwardPassRenderSystem::render(std::shared_ptr<EngineCommandBuffer> commandBuffer, VkDescriptorSet descriptorSet, std::shared_ptr<EngineVertexModel> model) {
		this->pipeline->bind(commandBuffer->getCommandBuffer());

		vkCmdBindDescriptorSets(
			commandBuffer->getCommandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			this->pipelineLayout,
			0,
			1u,
			&descriptorSet,
			0,
			nullptr
		);

		model->bind(commandBuffer);
		model->draw(commandBuffer);
	}
}