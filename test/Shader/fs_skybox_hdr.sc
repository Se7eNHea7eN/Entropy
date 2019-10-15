$input v_pos

#include "../common/common.sh"

SAMPLERCUBE(s_skybox, 0);

void main()
{		
    vec3 envColor = textureCube(s_skybox, normalize(v_pos)).xyz;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0,1.0,1.0));
    envColor = pow(envColor, vec3(1.0/2.2,1.0/2.2,1.0/2.2)); 
    
    gl_FragColor = vec4(sin(v_pos), 1.0);
}
