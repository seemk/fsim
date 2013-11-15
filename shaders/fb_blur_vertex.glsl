#version 330

layout(std140) uniform DefaultUniforms
{
	mat4 MVP;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoord;

out vec2 texCoord;

void main()
{
	texCoord = textureCoord;
	gl_Position = vec4(position, 0.0f, 1.0f);
	
}