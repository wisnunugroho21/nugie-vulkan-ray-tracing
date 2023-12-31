#version 460

#include "struct.glsl"

layout(location = 0) in vec3 positionFrag;
layout(location = 1) flat in uint materialIndexFrag;

layout(location = 0) out vec4 albedoColorResource;

layout(set = 0, binding = 2) buffer readonly MaterialSsbo {
  Material materials[];
};

void main() {
  albedoColorResource = vec4(materials[materialIndexFrag].baseColor, 1.0f);
}