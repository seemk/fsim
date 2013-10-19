#version 330

layout(points) in;
layout(line_strip, max_vertices = 33) out;

in vec3 vertexColor[];
out vec3 geometryVertexColor;

const float PI = 3.1415926;

void main() {
    geometryVertexColor = vertexColor[0];
    float vertices = 16.0;
    for (int i = 0; i <= vertices; i++)
    {
        float ang = PI * 2.0 / vertices * i;
        vec4 offset = 0.025 * vec4(sin(ang) * 0.3, cos(ang) * 0.4, 0.0, 0.0);
        gl_Position = gl_in[0].gl_Position + offset;
        EmitVertex();
    }

    EndPrimitive();
}