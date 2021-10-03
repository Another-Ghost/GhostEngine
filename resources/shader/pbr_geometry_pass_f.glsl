#version 450 core

layout (location = 0) out vec3 GWorldPosition;
layout (location = 1) out vec3 GWorldNormal;
layout (location = 2) out vec3 GBaseColor;
layout (location = 3) out vec3 GViewPosition;
layout (location = 4) out vec3 GViewNormal;
layout (location = 5) out vec3 GAORoughnessMetalness;
layout (location = 6) out vec3 GEmissive;
layout (location = 7) out vec3 GProbeWorldPosition;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 WorldNormal;
in vec3 ViewPos;
in vec3 ViewNormal;

//material
uniform sampler2D basecolor_map;
uniform sampler2D normal_map;
uniform sampler2D ao_map;
uniform sampler2D metalness_roughness_map;
uniform sampler2D emissive_map;

vec3 GetNormalFromMap(vec3 normal)
{
	vec3 tangent_normal = texture(normal_map, TexCoords).xyz * 2.0 - 1.0; //vec3(0.5, 0.5, 1) * 2 - 1.0;//texture(normal_map, Tex_Coords).xyz * 2.0 - 1.0;	//normal in tangent space
	//return tangent_normal;
	
	vec3 d_p_x = dFdx(WorldPos);
	vec3 d_p_y = dFdy(WorldPos);
	vec2 d_tex_x = dFdx(TexCoords);
	vec2 d_tex_y = dFdy(TexCoords);

	vec3 N = normalize(normal);
	vec3 T = normalize(d_p_x* d_tex_y.t - d_p_y * d_tex_x.t); //? t

	vec3 B  = -normalize(cross(N, T));  //ide插件错误检测存在的问题
	mat3 TBN = mat3(T, B, N); 

	return normalize(TBN * tangent_normal);
}

void main()
{
//	GBaseColor = pow(texture(basecolor_map, TexCoords).rgb, vec3(2.2));
	GBaseColor = texture(basecolor_map, TexCoords).rgb;	//do gamma correction in lighting pass
	GAORoughnessMetalness.r = texture(ao_map, TexCoords).r;
	GAORoughnessMetalness.g = texture(metalness_roughness_map, TexCoords).b;
	GAORoughnessMetalness.b  = texture(metalness_roughness_map, TexCoords).g;
//	GEmissive = pow(texture(emissive_map, TexCoords).rgb, vec3(2.2));
	GEmissive = texture(emissive_map, TexCoords).rgb;

	GWorldPosition = WorldPos;
	GWorldNormal = GetNormalFromMap(WorldNormal);
	GViewPosition = ViewPos;
	GViewNormal = GetNormalFromMap(ViewNormal);

	GProbeWorldPosition =  WorldPos;
}
