#version 330

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D texFrameBuf;

const float blurSize = 1.0 / 300.0;
const float threshold = 0.9;
const int maxBound = 5;

void main()
{
   vec4 sum = vec4(0.0);
   for (int x = -maxBound; x <= maxBound; x++)
   {
      for (int y = -maxBound; y <= maxBound; y++)
      {
         sum += texture(texFrameBuf, vec2(texCoord.x + x * blurSize, texCoord.y + y * blurSize)) / pow((maxBound*2.0 + 1.0), 2.0);
      } 
   }

   sum = step(threshold, sum) * sum;
   outColor = sum;
}