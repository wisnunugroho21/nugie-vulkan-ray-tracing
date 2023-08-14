#pragma once

#include <glm/glm.hpp>

namespace nugiEngine {
  struct CameraTransformation {
    alignas(16) glm::vec3 origin{0.0f};
    alignas(16) glm::vec3 direction{0.0f};
  };
  
  struct CameraRay {
    alignas(16) glm::vec3 origin{0.0f};
    alignas(16) glm::vec3 horizontal{0.0f};
    alignas(16) glm::vec3 vertical{0.0f};
    alignas(16) glm::vec3 lowerLeftCorner{0.0f};
  };
  
  class EngineCamera {
    public:
      EngineCamera(uint32_t width, uint32_t height);

      void setViewDirection(glm::vec3 position, glm::vec3 direction, float vfov, glm::vec3 vup = glm::vec3{0.0f, 1.0f, 0.0f});
      void setViewTransformation(CameraTransformation cameraTransformation, float vfov, glm::vec3 vup = glm::vec3{0.0f, 1.0f, 0.0f});
      void setViewTarget(glm::vec3 position, glm::vec3 target, float vfov, glm::vec3 vup = glm::vec3{0.0f, 1.0f, 0.0f});

      CameraRay getCameraRay() const { return this->cameraRay; }
      CameraTransformation getCameraTransformation() const { return this->cameraTransformation; }

    private:
      CameraRay cameraRay;
      CameraTransformation cameraTransformation;

      uint32_t width, height;
  };
} // namespace nugiEngine

