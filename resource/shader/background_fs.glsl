#version 330 core
out vec4 Frag_Color;
in vec3 World_Pos;

uniform samplerCube environment_map;

void main()
{		
    vec3 env_color = texture(environment_map, World_Pos).rgb;
    
    // HDR tonemap and gamma correct
    env_color = env_color / (env_color + vec3(1.0));
    env_color = pow(env_color, vec3(1.0/2.2)); 
    
    Frag_Color = vec4(env_color, 1.0);
}