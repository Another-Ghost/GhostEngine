#version 450 core

layout (location = 7) out vec4 FragColor; 
//out vec4 FragColor; 

in vec2 TexCoords;

//material
uniform sampler2D g_world_position;
uniform sampler2D g_world_normal;
uniform sampler2D g_basecolor;
uniform sampler2D g_ao_roughness_metalness;
uniform sampler2D g_emissive;

uniform float roughness;

//IBL
uniform samplerCube irradiance_maps[4];
uniform samplerCube light_prefilter_maps[4];
uniform samplerCube probe_depth_maps[4];
uniform sampler2D brdf_lut;

//shadow
uniform samplerCube point_depth_maps[4];
uniform float shadow_far_plane;
uniform bool b_shadow;

//light
struct LightInfo
{
vec4 position;
vec4 color;
};

layout (std430, binding = 3) buffer LightInfoArray
{
	LightInfo light_info_array[]; 
};

struct ProbeInfo
{
	vec4 probe_pos;
	vec4 aabb_pos;
	vec4 aabb_half_size;
};

layout (std430, binding = 5) buffer ProbeInfos
{
	ProbeInfo probe_infos[];
};

//camera
struct CameraInfo
{
    vec4 position;
    mat4 view;
    mat4 projection;
};

layout(std140, binding = 0) uniform Camera
{
    CameraInfo camera;
};


const float PI = 3.14159265359;


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
	//return F0 + (1.0 - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
	return F0 + (1.0 - F0) * pow(2, (-5.55473 * cos_theta - 6.98316) * cos_theta);
}

vec3 FresnelSchlickRoughness(float cos_theta, vec3 F0, float roughness)
{
    //return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(2, (-5.55473 * cos_theta - 6.98316) * cos_theta);
} 


struct IntersectionInfo
{
	bool b_intersected;
	vec3 position;
};

bool PointAABBIntersection(vec3 point_pos, vec3 aabb_pos, vec3 half_dimension){ 

	vec3 aabb_min = aabb_pos - half_dimension;
	vec3 aabb_max = aabb_pos + half_dimension;
   return

    (point_pos[0] >= aabb_min[0]) && (point_pos[0] <= aabb_max[0]) &&

    (point_pos[1] >= aabb_min[1]) && (point_pos[1] <= aabb_max[1]) &&

    (point_pos[2] >= aabb_min[2]) && (point_pos[2] <= aabb_max[2]);
}

float MaxElement(vec3 v)
{
	float ret = v.r > v.g ? v.r : v.g;
	return ret > v.b ? ret : v.b;
}

float GetAABBInfluenceWeight(vec3 point_pos, vec3 aabb_pos, vec3 half_dimension)
{
	vec3 local_dir = abs(point_pos - aabb_pos);
	local_dir /= half_dimension;
	return max(1.f - MaxElement(local_dir), 0.00001);
	//return 1.f - MaxElement(local_dir);
}

float GetSphereInfluenceWeight(vec3 point_pos, vec3 origin, float radius)
{
	float dist = length(point_pos - origin);
	  
	//return dist / radius;
	return max(1.f - dist / radius, 0.00001);
}

IntersectionInfo RayAABBIntersection(vec3 ray_ori, vec3 ray_dir, vec3 aabb_pos, vec3 half_dimension)
{

	IntersectionInfo info;
	info.b_intersected = false;

	//half_dimension = vec3(8, 8, 8);

	vec3 aabb_min = aabb_pos - half_dimension;
	vec3 aabb_max = aabb_pos + half_dimension;

	vec3 t_max = vec3(-1.f, -1.f, -1.f);
	vec3 t_min = vec3(-1.f, -1.f, -1.f);


	for (int i = 0; i < 3; ++i)	//calculate t_exit
	{
		if (ray_dir[i] > 0)
		{
			t_max[i] = (aabb_max[i] - ray_ori[i]) / ray_dir[i];
			t_min[i] = (aabb_min[i] - ray_ori[i]) / ray_dir[i];
		}
		else if (ray_dir[i] < 0)
		{
			t_max[i] = (aabb_min[i] - ray_ori[i]) / ray_dir[i];
			t_min[i] = (aabb_max[i] - ray_ori[i]) / ray_dir[i];
		}
	}


	float t_exit = t_max.r > t_max.g ? t_max.g : t_max.r;
    t_exit = t_exit > t_max.b ? t_max.b : t_exit; //check if the ray enters all pairs of slabs


	float t_enter = t_min.r > t_min.g ? t_min.r : t_min.g;
    t_enter = t_enter > t_min.b ? t_enter : t_min.b;

	
	if (t_exit < 0.f || t_enter >= t_exit )//|| t_enter > 0)
	{
		return info;
	}



	vec3 intersection = ray_ori + (ray_dir * t_exit);
	const float epsilon = 0.00001f;
//	for (int i = 0; i < 3; ++i)
//	{
//		if (intersection[i] + epsilon < aabb_min[i] ||
//			intersection[i] - epsilon > aabb_max[i])
//		{
//			return info;
//		}
//	}

	info.b_intersected = true;
	info.position = intersection;
	//info.distance = best_t;

	return info;
}

IntersectionInfo RaySphereIntersection(vec3 ray_ori, vec3 ray_dir, vec3 sphere_ori, float radius)
{
	IntersectionInfo info;
	info.b_intersected = false;

	vec3 dir = sphere_ori - ray_ori;

	float sphere_proj = dot(dir, ray_dir);

	if(sphere_proj < 0.f)
	{
		return info;
	}

	vec3 point = ray_ori + ray_dir * sphere_proj;

	float sphere_dist = length(point - sphere_ori);
	
	if(sphere_dist > radius)
	{
		return info;
	}

	float offset = sqrt(radius * radius - sphere_dist * sphere_dist);

	float ray_distance = sphere_proj - offset;
	info.position = ray_ori + ray_dir * ray_distance;
	info.b_intersected = true;

	return info;
}


// array of offset direction for sampling
vec3 grid_sampling_disk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 frag_pos, int i)
{
	vec3 frag_to_light = frag_pos - light_info_array[i].position.rgb;
	float current_depth = length(frag_to_light);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;

	float view_distance = length(camera.position.xyz - frag_pos);
	float disk_radius = (1.f + view_distance / shadow_far_plane) / shadow_far_plane;
	for(int j = 0; j < samples; ++j)
	{
		float closest_depth = texture(point_depth_maps[i], frag_to_light + grid_sampling_disk[j] * disk_radius).r;
		closest_depth *= shadow_far_plane;
		if(current_depth - bias > closest_depth)
		{
			shadow += 1.f;
		}
	}

	shadow /= float(samples);

	return shadow;
}

void main()
{
	vec3 world_pos = texture(g_world_position, TexCoords).rgb; 

	vec3 albedo = pow(texture(g_basecolor, TexCoords).rgb, vec3(2.2)); //base color是在sRGB空间的，本不需要结尾的gamma correction，但为了此处就先进行一个逆gamma correction
	//vec3 albedo = vec3(1.f, 1.f, 1.f);
	float ao = texture(g_ao_roughness_metalness, TexCoords).r;
    //float metalness  = texture(metalness_map, Tex_Coords).r;
	float metalness  = texture(g_ao_roughness_metalness, TexCoords).b;
	//float metalness  = 1.f;
    //float roughness = texture(roughness_map, Tex_Coords).r;
	float roughness = texture(g_ao_roughness_metalness, TexCoords).g;

    //float ao = 1.f;
	vec3 emissive = pow(texture(g_emissive, TexCoords).rgb, vec3(2.2));	

	vec3 N = texture(g_world_normal, TexCoords).rgb;
	vec3 V = normalize(camera.position.xyz - world_pos);	//	view direction/solid angle, point to outside
	vec3 R = reflect(-V, N);	//light direction
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)  
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	vec3 Lo = vec3(0.f);	
	for(int i = 0; i < light_info_array.length(); ++i)
	//for(int i = 0; i < 1; ++i)
	{
		// calculate per-light radiance
		vec3 L = normalize(light_info_array[i].position.xyz - world_pos); //light/incident direction
		vec3 H = normalize(V + L);
		float dist = length(light_info_array[i].position.xyz - world_pos);
		float attenuation = 1.0 / (dist * dist);
		vec3 radiance = light_info_array[i].color.xyz * attenuation;

		//radiance *= 10;
	
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

		//float shadow = b_shadow ? ShadowCalculation(world_pos, i) : 0.f;
		float shadow = ShadowCalculation(world_pos, i);
		vec3 Lo_i = (kD * albedo / PI + specular) * radiance * NdotL;
		
		Lo_i *= (1.0 - shadow);

		Lo += Lo_i; 

	}

	//环境光部分与直接光源是独立的，互不影响
	vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kS = F;
	
	//vec3 Fr = max(vec3(1.0 - roughness), F0) - F0; 
	//vec3 kS = F0 + Fr * pow(1.0-max(dot(N, V), 0.0), 5.0);
	//vec3 F = FresnelSchlick(N, V, F0);
    //vec3 kD = 1.0 - kS;
    //kD *= 1.0 - metalness;	

	vec3 irradiance = vec3(0.f);
	int num_intersected_probes = 0;
	float irradiance_weight_sum = 0.f;
	for(int i = 0; i < probe_infos.length(); ++i)
	{
		IntersectionInfo info = RayAABBIntersection(world_pos, V, probe_infos[i].aabb_pos.rgb, probe_infos[i].aabb_half_size.rgb);
		//IntersectionInfo info = RaySphereIntersection(world_pos, R, aabb_pos.rgb, aabb_half_size.r);
		//IntersectionInfo info = RaySphereIntersection(world_pos, R, aabb_pos.rgb, 12.f);

		if(info.b_intersected)	//move into function /?
		{
			vec3 parallax_corrected_sample_dir = normalize(info.position - probe_infos[i].probe_pos.rgb);

//			if(texture(probe_depth_maps[i], parallax_corrected_sample_dir).r >= 1.0f)
//			{
//				continue;
//			}

			//parallax_corrected_sample_dir.x = -parallax_corrected_sample_dir.x;
			vec3 sample_value = texture(irradiance_maps[i], parallax_corrected_sample_dir).rgb; //改为用写入的深度值判断 /？
			
//			if(sample_value != vec3(0))
//			//if(sample_value.r>0.0001 && sample_value.g > 0.0001 && sample_value.b > 0.0001)
//			{
			float weight = GetSphereInfluenceWeight(world_pos, probe_infos[i].aabb_pos.rgb, length(probe_infos[i].aabb_half_size.rgb));

			irradiance += sample_value * weight;
			irradiance_weight_sum += weight;

			//irradiance += sample_value;
			++num_intersected_probes;
//			}
			//prefiltered_radiance = textureLod(light_prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;    

		}
	}
	irradiance /= num_intersected_probes == 0 ? max(irradiance_weight_sum, 0.00001) : 1.f;

	//vec3 diffuse = irradiance * albedo;	//albedo表示表面颜色/基础反射率/折射吸收系数

	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;

	vec3 prefiltered_radiance = vec3(0.f);
	num_intersected_probes = 0;
	float radiance_weight_sum = 0.f;
	for(int i = 0; i < probe_infos.length(); ++i)
	{
//		if(!PointAABBIntersection(world_pos, probe_infos[i].aabb_pos.rgb, probe_infos[i].aabb_half_size.rgb))
//		{
//			continue;
//		}

		IntersectionInfo info = RayAABBIntersection(world_pos, R, probe_infos[i].aabb_pos.rgb, probe_infos[i].aabb_half_size.rgb);
		//IntersectionInfo info = RaySphereIntersection(world_pos, R, aabb_pos.rgb, aabb_half_size.r);
		//IntersectionInfo info = RaySphereIntersection(world_pos, R, aabb_pos.rgb, 12.f);

		if(info.b_intersected)	//move into function /?
		{
			vec3 parallax_corrected_sample_dir = normalize(info.position - probe_infos[i].probe_pos.rgb);
			//parallax_corrected_sample_dir.x = -parallax_corrected_sample_dir.x;

//			if(texture(probe_depth_maps[i], parallax_corrected_sample_dir).r >= 1.0f)
//			{
//				continue;
//			}
			vec3 sample_pos = texture(probe_depth_maps[i], parallax_corrected_sample_dir).rgb;
			if(!PointAABBIntersection(sample_pos, probe_infos[i].aabb_pos.rgb, probe_infos[i].aabb_half_size.rgb))
			{
				continue;
			}


			vec3 sample_value = textureLod(light_prefilter_maps[i], parallax_corrected_sample_dir,  roughness * MAX_REFLECTION_LOD).rgb;  


			//if(sample_value != vec3(0))
			//if(sample_value.r != 0 && sample_value.g != 0 && sample_value.b != 0)
			//float weight = GetAABBInfluenceWeight(world_pos, probe_infos[i].aabb_pos.rgb, probe_infos[i].aabb_half_size.rgb);
			float weight = GetSphereInfluenceWeight(world_pos, probe_infos[i].aabb_pos.rgb, length(probe_infos[i].aabb_half_size.rgb));
			//float weight = GetSphereInfluenceWeight(sample_pos, probe_infos[i].aabb_pos.rgb, length(probe_infos[i].aabb_half_size.rgb));


			prefiltered_radiance += sample_value * weight;
			radiance_weight_sum += weight;
			++num_intersected_probes;

			//prefiltered_radiance = textureLod(light_prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;    
		}
	}
	if(num_intersected_probes > 0)
	{
		prefiltered_radiance /= max(radiance_weight_sum, 0.00001);
	}
	//prefiltered_radiance /= num_intersected_probes == 0 ? 1 : num_intersected_probes;


//	vec3 prefiltered_radiance = vec3(0);
//	info = RayAABBIntersection(world_pos, R, aabb_pos.rgb, aabb_half_size.rgb);
//	//IntersectionInfo info = RaySphereIntersection(world_pos, R, aabb_pos.rgb, aabb_half_size.r);
//	//IntersectionInfo info = RaySphereIntersection(world_pos, R, aabb_pos.rgb, 12.f);
//
//	if(info.b_intersected)	//move into function /?
//	{
//		vec3 parallax_corrected_sample_dir = normalize(info.position - probe_pos.rgb);
//		//parallax_corrected_sample_dir.x = -parallax_corrected_sample_dir.x;
//		prefiltered_radiance = textureLod(light_prefilter_map, parallax_corrected_sample_dir,  roughness * MAX_REFLECTION_LOD).rgb;  
//		
//		//prefiltered_radiance = textureLod(light_prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;    
//	}



    //vec3 prefiltered_radiance = textureLod(light_prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    
	vec2 brdf  = texture(brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 FssEss = kS * brdf.x + brdf.y;

	float Ess = brdf.x + brdf.y;
	float Ems = 1 - Ess;
	vec3 Favg = F0 + (1 - F0) / 21;
	vec3 Fms = FssEss * Favg / (1 - (1 - Ess) * Favg);

	vec3 Ed = vec3(1.f) - (FssEss + Fms * Ems);
	vec3 kD =  Ed * albedo * (1 - metalness);	//mutiplying (1 - metalness) is due to only dielectrics have this Kd term
	//the base color map is the combination of diffuse part of dielectrics and specular part of conductor

	//vec3 kD = vec3(0);

	vec3 color = FssEss * prefiltered_radiance + (Fms * Ems + kD) * irradiance; 

	//color =  FssEss * prefiltered_radiance;
	//color = (Fms * Ems + kD) * irradiance; 
	//color = prefiltered_radiance;

	//vec3 color = Edss;

	color = (color + Lo) * ao;


	//color = color * ao;
	//color = Lo * ao;
	

	color += emissive;
	//color = color * ao + Lo;
	//color += Lo ;


	//HDR tone mapping
	color = color / (color + vec3(1.0));	//? replace to way of adjusting exposure 

	//gamma conrrection
	color = pow(color, vec3(1.0/2.2)); 


	FragColor = vec4(color, 1.0);
}