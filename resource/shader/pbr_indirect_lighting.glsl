#version 450 core

layout (location = 7) out vec4 FragColor; 

in vec2 TexCoords;

//material
uniform sampler2D g_world_position;
uniform sampler2D g_world_normal;
uniform sampler2D g_basecolor;
uniform sampler2D g_ao_roughness_metalness;
uniform sampler2D g_emissive;

//IBL
uniform samplerCube irradiance_map;
uniform samplerCube light_prefilter_map;
uniform sampler2D brdf_lut;


//shadow
uniform samplerCube point_depth_maps[8];
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


layout(std140, binding = 5) uniform ProbeAABB
{
	vec4 probe_pos;
	vec4 aabb_pos;
	vec4 aabb_half_size;
};


const float PI = 3.14159265359;

vec3 FresnelSchlick(vec3 H, vec3 V, vec3 F0)
{
	float cos_theta = max(dot(H, V), 0.0);
	return F0 + (1.0 - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cos_theta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
} 


struct IntersectionInfo
{
	bool b_intersected;
	vec3 position;
};

IntersectionInfo RayAABBIntersection(vec3 ray_ori, vec3 ray_dir, vec3 aabb_pos, vec3 half_dimension)
{

	IntersectionInfo info;
	info.b_intersected = false;

	half_dimension = vec3(8, 8, 8);

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

	if (t_exit < 0.f || t_enter >= t_exit || t_enter > 0)
	{
		return info;
	}

	vec3 intersection = ray_ori + (ray_dir * t_exit);
	const float epsilon = 0.00001f;
	for (int i = 0; i < 3; ++i)
	{
		if (intersection[i] + epsilon < aabb_min[i] ||
			intersection[i] - epsilon > aabb_max[i])
		{
			return info;
		}
	}

	info.b_intersected = true;
	info.position = intersection;

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



void main()
{
	vec3 world_pos = texture(g_world_position, TexCoords).rgb; 

	vec3 albedo = pow(texture(g_basecolor, TexCoords).rgb, vec3(2.2)); //gamma correction
	float ao = texture(g_ao_roughness_metalness, TexCoords).r;
	float metalness  = texture(g_ao_roughness_metalness, TexCoords).b;
	float roughness = texture(g_ao_roughness_metalness, TexCoords).g;

	vec3 emissive = pow(texture(g_emissive, TexCoords).rgb, vec3(2.2));	

	vec3 N = texture(g_world_normal, TexCoords).rgb;
	vec3 V = normalize(camera.position.xyz - world_pos);	//	view direction/solid angle, point to outside
	vec3 R = reflect(-V, N);	//light direction
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)  
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	//环境光部分与直接光源是独立的，互不影响
	vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kS = F;
	
	vec3 irradiance = texture(irradiance_map, N).rgb;

	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;

	vec3 prefiltered_radiance = vec3(0);
	IntersectionInfo info = RayAABBIntersection(world_pos, R, aabb_pos.rgb, aabb_half_size.rgb);

	if(info.b_intersected)	//move into function /?
	{
		vec3 parallax_corrected_sample_dir = normalize(info.position - probe_pos.rgb);
		prefiltered_radiance = textureLod(light_prefilter_map, parallax_corrected_sample_dir,  roughness * MAX_REFLECTION_LOD).rgb;  
	}

	vec2 brdf  = texture(brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 FssEss = kS * brdf.x + brdf.y;

	float Ess = brdf.x + brdf.y;
	float Ems = 1 - Ess;
	vec3 Favg = F0 + (1 - F0) / 21;
	vec3 Fms = FssEss * Favg / (1 - (1 - Ess) * Favg);

	vec3 Edss = vec3(1.f) - (FssEss + Fms * Ems);
	vec3 kD =  Edss * albedo * (1 - metalness);	//mutiplying (1 - metalness) is due to only dielectrics have this Kd term
	//the base color map is the combination of diffuse part of dielectrics and specular part of conductor

	vec3 color = FssEss * prefiltered_radiance + (Fms * Ems + kD) * irradiance; 
	//vec3 color = Edss;

	color = color * ao;
	color += emissive;

	//HDR tone mapping
	color = color / (color + vec3(1.0));	//? replace to way of adjusting exposure 

	//gamma conrrection
	color = pow(color, vec3(1.0/2.2)); 


	FragColor = vec4(color, 1.0);
}