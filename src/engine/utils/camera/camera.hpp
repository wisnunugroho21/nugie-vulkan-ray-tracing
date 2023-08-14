#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace nugiEngine {
  class EngineCamera {
    public:
      void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
      void setPerspectiveProjection(float fovy, float aspect_ratio, float near, float far);

      void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
      void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
      void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

      const glm::mat4 getProjectionMatrix() const { return this->projectionMatrix; }
      const glm::mat4 getViewMatrix() const { return this->viewMatrix; }
      const glm::mat4 getInverseViewMatrix() const { return this->inverseViewMatrix; }

    private:
      glm::mat4 projectionMatrix{1.0f};
      glm::mat4 viewMatrix{1.0f};
      glm::mat4 inverseViewMatrix{1.0f};
  };
} // namespace nugiEngine

