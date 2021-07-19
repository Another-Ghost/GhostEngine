#version 450 core
out float OUT_FragColor;

in vec2 TexCoords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D noise_texture;

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

//struct WindowInfo
//{
//	int width;
//	int height;
//};
layout(std140, binding = 1) uniform Window
{
//    WindowInfo window;
	int width;
	int height;
};


layout (std430, binding = 2) buffer SSAO
{
	vec4 sample_array[];
};


float radius = 0.5;
float bias = 0.025;

uniform int noise_tex_size;

void main()
{
    //?
    //const vec2 noise_scale = vec2(window.width/noise_tex_size, window.height/noise_tex_size); 
    const vec2 noise_scale = vec2(1280.0/float(noise_tex_size), 720.0/float(noise_tex_size)); 

    vec3 frag_pos = texture(g_position, TexCoords).xyz; //view space
    if(frag_pos.z < 0.f)
    {
        vec3 normal = normalize(texture(g_normal, TexCoords).xyz);  //view space
        vec3 random_vec = normalize(texture(noise_texture, TexCoords * noise_scale).xyz);   //����ֵ�����1����noise_texture����ΪREPEAT������Ч�����൱�ڰ�noise texture�Ĵ�Сƽ���������

        //?
        vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));    //Schmidt������
        vec3 bitangent = cross(normal, tangent);    
        mat3 TBN = mat3(tangent, bitangent, normal);

        float occlusion = 0.f;

        for(int i = 0; i < sample_array.length(); ++i)
        {
            vec3 sample_pos = TBN * sample_array[i].xyz;
            sample_pos = frag_pos + sample_pos * radius;    //sample_pos��С��0����λ����֮�䣬ͨ��radius�������Ʋ����뾶
        
            vec4 offset = vec4(sample_pos, 1.0);
            offset = camera.projection * offset; // from view to clip-space frag_pos��view�ռ�ģ����Բ���Ҫ����view matrix
            offset.xyz /= offset.w; // perspective divide ��λ��
            offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
            float sample_depth = texture(g_position, offset.xy).z;

            float range_check = smoothstep(0.f, 1.f, radius / abs(frag_pos.z - sample_depth));  //clamp sample_depth between (0, 1), but why smooth /?
            occlusion += (sample_depth >= sample_pos.z + bias ? 1.0 : 0.0) * range_check; //ignore sample points that too near the origin point
        }

            occlusion = 1.f - (occlusion / sample_array.length());  //output result 1 means totally no occlusion

            OUT_FragColor = occlusion;
    }
    else
        OUT_FragColor = 1.f;






    //OUT_FragColor = sample_array.length() / 128.0;
}