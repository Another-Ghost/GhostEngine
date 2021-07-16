#version 450 core

out float OUT_FragColor;

in vec2 TexCoords;

uniform sampler2D ssao;

int half_size = 2;

void main()
{
	vec2 texel_size = 1.f / vec2(textureSize(ssao, 0));	//the second param in textureSize is lod level
	float result = 0.f;
	for(int x = -half_size; x < half_size; ++x)
	{
		for(int y = -half_size; y < half_size; ++y)
		{
			vec2 offset = vec2(float(x), float(y)) * texel_size;
			result += texture(ssao, TexCoords + offset).r;
		}
	}
	OUT_FragColor = result / (half_size * half_size * 4);
}