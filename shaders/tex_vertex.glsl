#version 330

layout(std140) uniform DefaultUniforms
{
	mat4 MVP;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoord;
layout(location = 2) in vec2 flowColour;

out vec2 texCoord;
out vec2 flow;

void main()
{
	texCoord = textureCoord;
	flow = flowColour;
	gl_Position = vec4(position, 0.0f, 1.0f);
	
}