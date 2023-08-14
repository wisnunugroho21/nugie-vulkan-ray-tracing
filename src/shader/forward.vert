#version 460

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragColor;

layout(set = 0, binding = 0) uniform readonly RasterUbo {
	mat4 modelViewProjection;
} ubo;

void main() {
    gl_Position = modelViewProjection * vec4(position, 1.0f);
    
    fragPosition = position.xyz;
    fragColor = color.xyz;
}