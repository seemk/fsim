#version 330

in vec2 texCoord;
in vec2 flow;
out vec4 outColor;

uniform sampler2D texFrameBuf;
uniform float threshold = 0.5;
uniform bool colorCut = true;

const float gap = 0.2;
const float hgap = gap / 2;
const float boundMax = 0.5 + hgap;
const float boundMin = 0.5 - hgap;

void main()
{

   outColor = texture(texFrameBuf, texCoord);
   if(colorCut)
   {
   		outColor = step(threshold, outColor) * outColor;
   		if(outColor.z > 0.0)
	   	{
	   	   if((outColor.x >= boundMin && outColor.x <= boundMax)
		   && (outColor.y >= boundMin && outColor.y <= boundMax))
		{
		   	outColor = vec4(0.36,0.59,0.91, 1.0);
		}
		else
		{
			outColor = vec4(0.11,0.33,0.65, 1.0);
		}
	   }
   }
}