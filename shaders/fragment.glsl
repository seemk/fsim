#version 330

in vec3 vertexColor;
in vec3 geometryVertexColor;
out vec4 outColor;

void main()
{
	outColor = vec4(vertexColor + geometryVertexColor, 1.0);

}