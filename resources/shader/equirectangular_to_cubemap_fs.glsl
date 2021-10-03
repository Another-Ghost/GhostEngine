#version 330 core
out vec4 Frag_Color;
in vec3 World_Pos;

uniform sampler2D equirectangular_map;

const vec2 inv_atan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= inv_atan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(World_Pos));
    vec3 color = texture(equirectangular_map, uv).rgb;
    
    Frag_Color = vec4(color, 1.0);
}
