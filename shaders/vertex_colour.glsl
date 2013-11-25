#version 330

layout(std140) uniform DefaultUniforms
{
	mat4 MVP;
};

layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 color;

out vec4 fColor;

void main()
{
    fColor = color;
    gl_Position = MVP * vec4(pos, 0.0, 1.0);

}

