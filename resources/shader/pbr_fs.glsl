#version 330 core

out vec4 frag_color;

in vec2 Tex_Coords;
in vec3 World_Pos;
in vec3 Normal;


//material
uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metalness_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

//IBL
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut;

//light
uniform vec3 light_position_array[1];
uniform vec3 light_color_array[1];

uniform vec3 cam_pos;

const float PI = 3.14159265359;


vec3 GetNormalFromMap()
{
	vec3 tangent_normal = texture(normal_map, Tex_Coords).xyz * 2.0 - 1.0;	//normal in tangent space
	//return tangent_normal;
	
	vec3 d_p_x = dFdx(World_Pos);
	vec3 d_p_y = dFdy(World_Pos);
	vec2 d_tex_x = dFdx(Tex_Coords);
	vec2 d_tex_y = dFdy(Tex_Coords);

	vec3 N = normalize(Normal);
	vec3 T = normalize(d_p_x* d_tex_y.t - d_p_y * d_tex_x.t); //? t

	vec3 B  = -normalize(cross(N, T));  //ide插件错误检测存在的问题
	mat3 TBN = mat3(T, B, N); 

	return normalize(TBN * tangent_normal);
}





float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float alpha = roughness * roughness;
	float alpha_2 = alpha * alpha;
	float NdotH = max(dot(N, H), 0.f);
	float NdotH_2 = NdotH * NdotH;

	float numerator = alpha_2;
	float denominator = (NdotH_2 * (alpha_2 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;

	return numerator / denominator;
}

float GeometrySchlikGGX(float NdotV, float roughness)
{
	float alpha = (roughness + 1.0);
	float k_direct = (alpha * alpha) / 8.0;
	
	float num = NdotV;
	float denom = NdotV * (1.0 - k_direct) + k_direct;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.f);
	float NdotL = max(dot(N, L), 0.f);

	float ggx_v = GeometrySchlikGGX(NdotV, roughness);
	float ggx_l = GeometrySchlikGGX(NdotL, roughness);
	
	return ggx_v * ggx_l;
}

vec3 FresnelSchlick(vec3 H, vec3 V, vec3 F0)
{
	float cos_theta = max(dot(H, V), 0.0);
	return F0 + (1.0 - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cos_theta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
} 

void main()
{
	vec3 albedo = pow(texture(albedo_map, Tex_Coords).rgb, vec3(2.2)); //gamma correction
    float metalness  = texture(metalness_map, Tex_Coords).r;
    float roughness = texture(roughness_map, Tex_Coords).r;
    float ao = texture(ao_map, Tex_Coords).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(cam_pos - World_Pos);	//view direction/solid angle, point to outside
	vec3 R = reflect(-V, N);
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)  
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	vec3 Lo = vec3(0.f);
	for(int i = 0; i < 1; ++i)
	{
		// calculate per-light radiance
		vec3 L = normalize(light_position_array[i] - World_Pos); //light/incident direction
		vec3 H = normalize(V + L);
		float dist = length(light_position_array[i] - World_Pos);
		float attenuation = 1.0 / (dist * dist);
		vec3 radiance = light_color_array[i] * attenuation;
	
		// Cook-Torrance BRDF
		float D = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelSchlick(H, V, F0);

		vec3 num = D * G * F;
		float denom = 4 * max(dot(N, V), 0.f) * max(dot(N, L), 0.f); 
		vec3 specular = num / max(denom, 0.001); // 0.001 to prevent divide by zero

		// kS is equal to Fresnel
		vec3 kS = F;

		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metalness;

		float NdotL = max(dot(N, L), 0.f);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	//环境光部分与直接光源是独立的，互不影响
	vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

	vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metalness;	


	vec3 irradiance = texture(irradiance_map, N).rgb;
    vec3 diffuse = irradiance * albedo;	//albedo表示表面颜色/基础反射率/折射吸收系数

	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefiltered_color = textureLod(prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefiltered_color * (F * brdf.x + brdf.y);

	vec3 ambient = (kD * diffuse + specular) * ao;	//? 指定光线较难逃逸出来的暗色边缘 为什么只作用于环境光，而没有作用于直接光照？
	//vec3 ambient = (kD * diffuse + specular);

	vec3 color = ambient + Lo;	
	//vec3 color = ambient;
	//vec3 color = Lo;

	//HDR tone mapping
	color = color / (color + vec3(1.0));	//? replace to way of adjusting exposure 

	//gamma conrrection
	color = pow(color, vec3(1.0/2.2)); 

	frag_color = vec4(color, 1.0);
	//frag_color = vec4(texture(albedo_map, IN_tex_coords).xyz, 1.0);

}