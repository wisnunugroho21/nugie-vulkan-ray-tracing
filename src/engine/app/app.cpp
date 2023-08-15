#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <string>
#include <chrono>
#include <iostream>
#include <cstdlib>

#include <thread>

namespace nugiEngine {
	EngineApp::EngineApp() {
		this->renderer = std::make_unique<EngineHybridRenderer>(this->window, this->device);

		this->loadObjects();
		this->recreateSubRendererAndSubsystem();
	}

	EngineApp::~EngineApp() {}

	void EngineApp::renderLoop() {
		while (this->isRendering) {
			if (this->renderer->acquireFrame()) {
				uint32_t frameIndex = this->renderer->getFrameIndex();
				uint32_t imageIndex = this->renderer->getImageIndex();

				auto descSets = std::make_shared<std::vector<VkDescriptorSet>>();
				descSets->emplace_back(this->forwardPassDescSet->getDescriptorSets(frameIndex));

				this->rasterUniform->writeGlobalData(frameIndex, this->rasterUbo);
				auto commandBuffer = this->renderer->beginCommand();
				
				this->swapChainSubRenderer->beginRenderPass(commandBuffer, imageIndex);
				this->forwardPassRender->render(commandBuffer, this->forwardPassDescSet->getDescriptorSets(frameIndex), this->vertexModels);
				this->swapChainSubRenderer->endRenderPass(commandBuffer);

				this->renderer->endCommand(commandBuffer);
				this->renderer->submitRenderCommand(commandBuffer);

				if (!this->renderer->presentFrame()) {
					this->recreateSubRendererAndSubsystem();
					this->randomSeed = 0;

					continue;
				}				

				if (frameIndex + 1 == EngineDevice::MAX_FRAMES_IN_FLIGHT) {
					this->randomSeed++;
				}				
			}
		}
	}

	void EngineApp::run() {
		// auto currentTime = std::chrono::high_resolution_clock::now();
		uint32_t t = 0;

		// this->rayTraceUniforms->writeGlobalData(0, this->globalUbo);
		std::thread renderThread(&EngineApp::renderLoop, std::ref(*this));

		while (!this->window.shouldClose()) {
			this->window.pollEvents();

			/*auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

			if (t == 10) {
				std::string appTitle = std::string(APP_TITLE) + std::string(" | FPS: ") + std::to_string((1.0f / frameTime));
				glfwSetWindowTitle(this->window.getWindow(), appTitle.c_str());

				t = 0;
			} else {
				t++;
			}

			currentTime = newTime;*/
		}

		this->isRendering = false;
		renderThread.join();

		vkDeviceWaitIdle(this->device.getLogicalDevice());
	}

	void EngineApp::loadObjects() {
		auto materials = std::make_shared<std::vector<Material>>();
		auto vertices = std::make_shared<std::vector<Vertex>>();
		auto indices = std::make_shared<std::vector<uint32_t>>();
		auto transforms = std::make_shared<std::vector<TransformComponent>>();

		// ----------------------------------------------------------------------------

		// kanan
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		uint32_t transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 555.0f, 1.0f}, 1u, transformIndex });

		indices->emplace_back(0u);
		indices->emplace_back(1u);
		indices->emplace_back(2u);
		indices->emplace_back(2u);
		indices->emplace_back(3u);
		indices->emplace_back(0u);

		// ----------------------------------------------------------------------------
		
		// kiri
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 2u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 0.0f, 1.0f}, 2u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 555.0f, 1.0f}, 2u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 555.0f, 1.0f}, 2u, transformIndex });

		indices->emplace_back(4u);
		indices->emplace_back(5u);
		indices->emplace_back(6u);
		indices->emplace_back(6u);
		indices->emplace_back(7u);
		indices->emplace_back(4u);

		// ----------------------------------------------------------------------------
		
		// bawah
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 0.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 555.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 555.0f, 1.0f}, 0u, transformIndex });

		indices->emplace_back(8u);
		indices->emplace_back(9u);
		indices->emplace_back(10u);
		indices->emplace_back(10u);
		indices->emplace_back(11u);
		indices->emplace_back(8u);

		// ----------------------------------------------------------------------------
		
		// atas
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 0.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 0.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 555.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 555.0f, 1.0f}, 0u, transformIndex });

		indices->emplace_back(12u);
		indices->emplace_back(13u);
		indices->emplace_back(14u);
		indices->emplace_back(14u);
		indices->emplace_back(15u);
		indices->emplace_back(12u);

		// ----------------------------------------------------------------------------
		
		// depan
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 555.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 555.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 555.0f, 1.0f}, 0u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 555.0f, 1.0f}, 0u, transformIndex });

		indices->emplace_back(16u);
		indices->emplace_back(17u);
		indices->emplace_back(18u);
		indices->emplace_back(18u);
		indices->emplace_back(19u);
		indices->emplace_back(16u);

		// ----------------------------------------------------------------------------

		materials->emplace_back(Material{ glm::vec3(0.73f, 0.73f, 0.73f), glm::vec3(0.0f), 0.0f, 0.1f, 0.5f, 0u, 0u });
		materials->emplace_back(Material{ glm::vec3(0.12f, 0.45f, 0.15f), glm::vec3(0.0f), 0.0f, 0.1f, 0.5f, 0u, 0u });
		materials->emplace_back(Material{ glm::vec3(0.65f, 0.05f, 0.05f), glm::vec3(0.0f), 0.0f, 0.1f, 0.5f, 0u, 0u });
		materials->emplace_back(Material{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), 0.0f, 0.1f, 0.5f, 1u, 0u });

		// ----------------------------------------------------------------------------

		this->materialModel = std::make_unique<EngineMaterialModel>(this->device, materials);
		this->transformationModel = std::make_unique<EngineTransformationModel>(this->device, transforms);
		this->vertexModels = std::make_unique<EngineVertexModel>(this->device, vertices, indices);

		this->colorTextures.emplace_back(std::make_unique<EngineTexture>(this->device, "textures/viking_room.png", VK_FILTER_LINEAR, 
			VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_TRUE, VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK, VK_COMPARE_OP_NEVER, 
			VK_SAMPLER_MIPMAP_MODE_LINEAR));
	}

	void EngineApp::updateCamera(uint32_t width, uint32_t height) {
		glm::vec3 position = glm::vec3(278.0f, 278.0f, -800.0f);
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 800.0f);
		glm::vec3 vup = glm::vec3(0.0f, 1.0f, 0.0f);

		float near = 0.1f;
		float far = 2000.0f;

		constexpr float theta = glm::radians(40.0f);
		float tanHalfFovy = glm::tan(theta / 2.0f);
		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		glm::vec3 w = glm::normalize(direction);
		glm::vec3 u = glm::normalize(glm::cross(w, vup));
		glm::vec3 v = glm::cross(w, u);

		auto view = glm::mat4{1.0f};
		view[0][0] = u.x;
    view[1][0] = u.y;
    view[2][0] = u.z;
    view[0][1] = v.x;
    view[1][1] = v.y;
    view[2][1] = v.z;
    view[0][2] = w.x;
    view[1][2] = w.y;
    view[2][2] = w.z;
    view[3][0] = -glm::dot(u, position);
    view[3][1] = -glm::dot(v, position);
    view[3][2] = -glm::dot(w, position);

		auto projection = glm::mat4{0.0f};
		projection[0][0] = 1.f / (aspectRatio * tanHalfFovy);
    projection[1][1] = 1.f / (tanHalfFovy);
    projection[2][2] = far / (far - near);
    projection[2][3] = 1.f;
    projection[3][2] = -(far * near) / (far - near);

		this->rasterUbo.viewProjection = projection * view;
	}

	void EngineApp::recreateSubRendererAndSubsystem() {
		uint32_t width = this->renderer->getSwapChain()->width();
		uint32_t height = this->renderer->getSwapChain()->height();

		this->rasterUniform = std::make_unique<EngineRasterUniform>(this->device);
		this->updateCamera(width, height);

		this->swapChainSubRenderer = std::make_unique<EngineSwapChainSubRenderer>(this->device, this->renderer->getSwapChain()->getswapChainImages(), 
			this->renderer->getSwapChain()->getSwapChainImageFormat(), static_cast<int>(this->renderer->getSwapChain()->imageCount()), 
			width, height);

		VkDescriptorBufferInfo forwardPassbuffersInfo[2] {
			this->transformationModel->getTransformationInfo(),
			this->materialModel->getMaterialInfo()
		};

		this->forwardPassDescSet = std::make_unique<EngineForwardPassDescSet>(this->device, this->renderer->getDescriptorPool(), this->rasterUniform->getBuffersInfo(), forwardPassbuffersInfo);
		this->forwardPassRender = std::make_unique<EngineForwardPassRenderSystem>(this->device, this->swapChainSubRenderer->getRenderPass(), this->forwardPassDescSet->getDescSetLayout());
	}
}