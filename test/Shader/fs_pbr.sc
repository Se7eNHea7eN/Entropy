$input v_pos, v_normal, v_texcoord0
#include "light.sc"

#include "../common/common.sh"

// material parameters
uniform vec3 u_albedo;

uniform vec4 u_params;
#define u_metallic u_params.x
#define u_roughness u_params.y
#define u_ao u_params.z

uniform vec3 u_cameraPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float u_roughness)
{
    float a = u_roughness*u_roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for u_roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float u_roughness)
{
    float r = (u_roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float u_roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, u_roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, u_roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_cameraPos - v_pos);

    vec3 F0 = vec3(0.04,0.04,0.04); 
    F0 = mix(F0, u_albedo, u_metallic);

    vec3 Lo = vec3(0.0,0.0,0.0);
    for(int i = 0; i < pointLightCount; ++i)
    {

        PointLight pointLight = allPointLights[i];

        vec3 L = normalize(pointLight.lightPosition.xyz - v_pos.xyz);
        vec3 H = normalize(V + L);
        float distance = length(pointLight.lightPosition.xyz - v_pos.xyz);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLight.lightColor.xyz    * attenuation;

        float NDF = DistributionGGX(N, H, u_roughness);
        float G   = GeometrySmith(N, V, L, u_roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
        vec3 kS = F;

        vec3 kD = vec3(1.0,1.0,1.0) - kS;

        kD *= 1.0 - u_metallic;	  

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * u_albedo / PI + specular) * radiance * NdotL;
    }   
    

    vec3 ambient = vec3(0.03,0.03,0.03) * u_albedo * u_ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0,1.0,1.0));
    color = pow(color, vec3(1.0/2.2,1.0/2.2,1.0/2.2));

    gl_FragColor = vec4(color, 1.0);
}