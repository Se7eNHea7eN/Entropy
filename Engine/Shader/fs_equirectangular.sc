$input v_pos
#include "../common/common.sh"

SAMPLER2D(equirectangularMap, 0);


vec2 SampleSphericalMap(vec3 v)
{
    const vec2 invAtan = vec2(0.1591, 0.3183);
    vec2 uv = vec2(atan2(v.z , v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(v_pos));
    vec3 color = texture2D(equirectangularMap, uv).xyz;
    
    gl_FragColor = vec4(color, 1.0);
}