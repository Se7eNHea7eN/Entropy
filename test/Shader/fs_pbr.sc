$input v_pos, v_normal, v_texcoord0
#include "light.sc"

#include "../common/common.sh"

SAMPLER2D(s_albedo, 0);
SAMPLER2D(s_metallic, 1);
SAMPLER2D(s_roughness, 2);
SAMPLER2D(s_ao, 3);
SAMPLER2D(s_normal, 4);

uniform vec3 u_cameraPos;

const float PI = 3.14159265359;



vec3 getNormalFromMap(vec3 position,vec2 texCoord , vec3 vnormal)
{
    vec3 tangentNormal = texture2D(s_normal, texCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(position);
    vec3 Q2  = dFdy(position);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);

    vec3 N   = normalize(vnormal);
    vec3 T  = normalize(Q1*st2.y - Q2*st1.y);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(mul(TBN , tangentNormal));
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
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
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
    vec3 albedo = texture2D(s_albedo, v_texcoord0).xyz;
    float metallic = texture2D(s_metallic, v_texcoord0).x;
    float roughness = texture2D(s_roughness, v_texcoord0).x;
    float ao = texture2D(s_ao, v_texcoord0).x;

    //vec3 N = normalize(v_normal);
    vec3 N = getNormalFromMap(v_pos,v_texcoord0,v_normal);

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
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
        vec3 kS = F;

        vec3 kD = vec3(1.0,1.0,1.0) - kS;

        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   
    

    vec3 ambient = vec3(0.03,0.03,0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0,1.0,1.0));
    color = pow(color, vec3(1.0/2.2,1.0/2.2,1.0/2.2));

    gl_FragColor = vec4(color, 1.0);
}