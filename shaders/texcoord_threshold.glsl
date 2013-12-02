#version 330

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D texFrameBuf;
uniform float threshold = 0.9;

void main()
{
   outColor = texture(texFrameBuf, texCoord);
   outColor = step(threshold, outColor) * outColor;
}