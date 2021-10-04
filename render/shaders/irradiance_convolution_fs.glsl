#version 330 core
out vec4 Frag_Color;
in vec3 World_Pos;

uniform samplerCube environment_map;

const float PI = 3.14159265359;

void main()
{		
	// The world vector acts as the normal of a tangent surface
    // from the origin, aligned to WorldPos. Given this normal, calculate all
    // incoming radiance of the environment. The result of this radiance
    // is the radiance of light coming from -Normal direction, which is what
    // we use in the PBR shader to sample irradiance.
    vec3 N = normalize(World_Pos);  //L_o

    vec3 irradiance = vec3(0.0);   
    
    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);   
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));    //ѧϰ�����tbn�ռ�ķ�ʽ
       
    float sample_delta = 0.025;  //��С����ֵ������߾�ȷ��
    float nr_samples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sample_delta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sample_delta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangent_sample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));  //������תֱ�����깫ʽ
            // tangent space to world
            vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * N; 

            irradiance += texture(environment_map, sample_vec).rgb * cos(theta) * sin(theta);
            nr_samples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nr_samples));
    
    Frag_Color = vec4(irradiance, 1.0);
}