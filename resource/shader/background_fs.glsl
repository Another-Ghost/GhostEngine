#version 330 core
//out vec4 Frag_Color;
layout (location = 2) out vec4 g_color;

in vec3 WorldPos;
//in vec3 FragPos;
uniform samplerCube environment_map;

void main()
{		
    vec3 env_color = texture(environment_map, WorldPos).rgb;
    
    // HDR tonemap and gamma correct
    env_color = env_color / (env_color + vec3(1.0));
    env_color = pow(env_color, vec3(1.0/2.2)); 
    
    //Frag_Color = vec4(env_color, 1.0);

    //Frag_Color = vec4(WorldPos, 1);
    //g_color = vec4(env_color, 1.0);
    g_color = vec4(env_color, 1.0); //trick 没有写进g_position,使其值为默认值>0, 做ssao时会被跳过
}