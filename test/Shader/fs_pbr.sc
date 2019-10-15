$input v_pos, v_normal, v_texcoord0 , v_tangent
#include "light.sc"

#include "../common/common.sh"

uniform vec4 u_params[1];
#define useNormalMap int(u_params[0].x)
#define isEmissive int(u_params[0].y)
#define useIrradianceMap int(u_params[0].z)

SAMPLER2D(s_albedo, 0);
SAMPLER2D(s_normal, 1);

SAMPLER2D(s_metallic, 2);
SAMPLER2D(s_roughness, 3);
SAMPLER2D(s_ao, 4);

SAMPLER2D(s_emissive, 5);

SAMPLERCUBE(s_irradianceMap, 6);

uniform vec3 u_cameraPos;


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    const float PI = 3.14159265359;

    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    //return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    return F0 + (1.0 - F0) * pow(2,(-5.55473*cosTheta - 6.98316) * cosTheta);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness,1.0 - roughness,1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

void main()
{		
    const float PI = 3.14159265359;

    vec3 albedo = texture2D(s_albedo, v_texcoord0).xyz;
    float metallic = texture2D(s_metallic, v_texcoord0).x;
    float roughness = texture2D(s_roughness, v_texcoord0).x;
    float ao = texture2D(s_ao, v_texcoord0).x;

    vec3 N;
    if(useNormalMap > 0){
        vec3 Normal = normalize(v_normal);
        vec3 Tangent = normalize(v_tangent);
        vec3 Bitangent = -normalize(cross(Normal,Tangent));

        mat3 TBN =  mat3(Tangent,Bitangent, Normal);
        #if BGFX_SHADER_LANGUAGE_HLSL | BGFX_SHADER_LANGUAGE_SPRIV
            TBN = transpose( TBN ); 
        #endif
        vec3 BumpMapNormal = normalize(2.0 * texture2D(s_normal, v_texcoord0).xyz - vec3(1.0, 1.0, 1.0));

        N = normalize(mul(TBN,BumpMapNormal));
    }
    else
        N = normalize(v_normal);

    vec3 V = normalize(u_cameraPos - v_pos);

    vec3 F0 = vec3(0.04,0.04,0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0,0.0,0.0);
    for(int i = 0; i < pointLightCount; ++i)
    {
        vec3 pointLightPosition = u_lightPosition[i].xyz;
        vec3 pointLightColor = u_lightColor[i].xyz;


        vec3 L = normalize(pointLightPosition - v_pos.xyz);
        vec3 H = normalize(V + L);
        float distance = length(pointLightPosition - v_pos.xyz);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLightColor * attenuation;

        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);     
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)+ 0.001;
        vec3 specular =  nominator / denominator; // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
        vec3 kS = F;

        vec3 kD = vec3(1.0,1.0,1.0) - kS;

        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   
    

    if(isEmissive > 0 ){
        vec4 emissive = texture2D(s_emissive, v_texcoord0);
        Lo = mix(Lo,emissive.xyz,emissive.w);
    }

    vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  

    vec3 ambient ;
    if(useIrradianceMap > 0){
        vec3 irradiance = textureCube(s_irradianceMap, N).xyz;
        vec3 diffuse      = irradiance * albedo;
        ambient = (kD * diffuse) * ao;
    }else
        ambient = vec3(0.03,0.03,0.03) * albedo * ao;
  

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0,1.0,1.0));
    color = pow(abs(color), vec3(1.0/2.2,1.0/2.2,1.0/2.2));

    gl_FragColor = vec4(color, 1.0);
}