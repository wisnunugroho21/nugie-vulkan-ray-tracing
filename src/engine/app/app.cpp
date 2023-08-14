#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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
				this->forwardPassRender->render(commandBuffer, descSets, this->vertexModels);
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
		auto currentTime = std::chrono::high_resolution_clock::now();
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
		uint32_t transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 555.0f, 1.0f}, 1u, transformIndex });

		indices->emplace_back(4u);
		indices->emplace_back(5u);
		indices->emplace_back(6u);
		indices->emplace_back(6u);
		indices->emplace_back(7u);
		indices->emplace_back(4u);

		// ----------------------------------------------------------------------------
		
		// bawah
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		uint32_t transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 555.0f, 1.0f}, 1u, transformIndex });

		indices->emplace_back(8u);
		indices->emplace_back(9u);
		indices->emplace_back(10u);
		indices->emplace_back(10u);
		indices->emplace_back(11u);
		indices->emplace_back(8u);

		// ----------------------------------------------------------------------------
		
		// atas
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		uint32_t transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 0.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 555.0f, 1.0f}, 1u, transformIndex });

		indices->emplace_back(12u);
		indices->emplace_back(13u);
		indices->emplace_back(14u);
		indices->emplace_back(14u);
		indices->emplace_back(15u);
		indices->emplace_back(12u);

		// ----------------------------------------------------------------------------
		
		// depan
		transforms->emplace_back(TransformComponent{ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f) });
		uint32_t transformIndex = static_cast<uint32_t>(transforms->size() - 1);

		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 0.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{0.0f, 555.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 555.0f, 555.0f, 1.0f}, 1u, transformIndex });
		vertices->emplace_back(Vertex{ glm::vec4{555.0f, 0.0f, 555.0f, 1.0f}, 1u, transformIndex });

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
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(40.0f), (float) width / (float) height, 0.1f, 2000.0f);
			
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
			
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(278.0f, 278.0f, -800.0f), // Camera is at (4,3,3), in World Space
			glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
			glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
		);
			
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);

		// Our ModelViewProjection : multiplication of our 3 matrices
		this->rasterUbo.modelViewProjection = Projection * View * Model; // Remember, matrix multiplication is the other way around
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