#version 460

// ------------- layout ------------- 

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
  uint randomSeed;
} push;

void main() {
  outColor = fragColor;
}