#version 330

layout(points) in;
layout(line_strip, max_vertices = 33) out;

in vec3 vertexColor[];
in mat4 mvMat[];
uniform float radius;
out vec3 geometryVertexColor;

const float PI = 3.1415926;

void main() {
    geometryVertexColor = vertexColor[0];
    float vertices = 16.0;
    for (int i = 0; i <= vertices; i++)
    {
        float ang = PI * 2.0 / vertices * i;
        vec4 offset = mvMat[0] * vec4(radius * sin(ang), radius * cos(ang), 0.0, 0.0);
        gl_Position = gl_in[0].gl_Position + offset;
        EmitVertex();
    }

    EndPrimitive();
}