#version 330

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D texFrameBuf;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;
const float threshold = 0.9;

void main()
{
	vec4 sum = vec4(0.0);
	for (int x = -4; x <= 4; x++)
	{
		for (int y = -4; y <= 4; y++)
		{
			sum += texture(texFrameBuf, vec2(texCoord.x + x * blurSizeH, texCoord.y + y * blurSizeV)) / 81.0;
		} 
	}
	sum = step(threshold, sum) * sum;
	outColor = sum;
}