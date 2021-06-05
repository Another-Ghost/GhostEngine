#version 330 core

out vec4 frag_color;


in vec2 Tex_coords;
in vec3 World_pos;
in vec3 Normal;


//material
uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metalness_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

//light
uniform vec3 light_position_array[1];
uniform vec3 light_color_array[1];

uniform vec3 cam_pos;

const float PI = 3.14159265359;


vec3 GetNormalFromMap()
{
	vec3 tangent_normal = texture(normal_map, Tex_coords).xyz * 2.0 - 1.0;	//normal in tangent space
	//return tangent_normal;
	
	vec3 d_p_x = dFdx(World_pos);
	vec3 d_p_y = dFdy(World_pos);
	vec2 d_tex_x = dFdx(Tex_coords);
	vec2 d_tex_y = dFdy(Tex_coords);

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

void main()
{
	vec3 albedo = pow(texture(albedo_map, Tex_coords).rgb, vec3(2.2)); //gamma correction
    float metalness  = texture(metalness_map, Tex_coords).r;
    float roughness = texture(roughness_map, Tex_coords).r;
    float ao = texture(ao_map, Tex_coords).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(cam_pos - World_pos);	//view direction/solid angle, point to outside
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)  
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	vec3 Lo = vec3(0.f);
	for(int i = 0; i < 1; ++i)
	{
		// calculate per-light radiance
		vec3 L = normalize(light_position_array[i] - World_pos); //light/incident direction
		vec3 H = normalize(V + L);
		float dist = length(light_position_array[i] - World_pos);
		float attenuation = 1.0 / (dist * dist);
		vec3 radiance = light_color_array[i] * attenuation;
	
		// Cook-Torrance BRDF
		float D = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelSchlick(H, V, F0);

		vec3 num = D * G * F;
		float denom = 4 * max(dot(N, V), 0.f) * max(dot(N, L), 0.f) + 0.001; // 0.001 to prevent divide by zero
		vec3 specular = num / denom;

		// kS is equal to Fresnel
		vec3 kS = F;

		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metalness;

		float NdotL = max(dot(N, L), 0.f);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + Lo;

	//HDR tone mapping
	color = color / (color + vec3(1.0));	//? replace to way of adjusting exposure 

	//gamma conrrection
	color = pow(color, vec3(1.0/2.2)); 

	frag_color = vec4(color, 1.0);
	//frag_color = vec4(texture(albedo_map, IN_tex_coords).xyz, 1.0);

}