#version 460

#include "struct.glsl"

layout(location = 0) in vec4 position;
layout(location = 1) in uint materialIndex;
layout(location = 2) in uint transformIndex;

layout(location = 0) out vec3 positionFrag;
layout(location = 1) flat out uint materialIndexFrag;

layout(set = 0, binding = 0) uniform readonly RasterUbo {
	mat4 viewProjection;
} ubo;

layout(set = 0, binding = 1) buffer readonly TransformationSsbo {
  Transformation transformations[];
};

void main() {
	vec4 positionWorld = transformations[transformIndex].pointMatrix * position;
	gl_Position = ubo.viewProjection * positionWorld;

	positionFrag = positionWorld.xyz;
	materialIndexFrag = materialIndex;
}