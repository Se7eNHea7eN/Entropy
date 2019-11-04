$input v_pos, v_normal, v_texcoord0 , v_tangent
#include "light.sc"

#include "common/common.sh"

uniform vec4 u_params[2];
#define useNormalMap int(u_params[0].x)
#define isEmissive int(u_params[0].y)
#define useIBL int(u_params[0].z)
#define useDepthMap int(u_params[1].x)
#define heightScale u_params[1].y

SAMPLER2D(s_albedo, 0);
SAMPLER2D(s_normal, 1);

SAMPLER2D(s_metallic, 2);
SAMPLER2D(s_roughness, 3);
SAMPLER2D(s_ao, 4);

SAMPLER2D(s_emissive, 5);

SAMPLERCUBE(s_irradianceMap, 6);
SAMPLERCUBE(s_prefilterMap, 7);
SAMPLER2D(s_brdfLUT, 8);

SAMPLER2D(s_depthMap, 9);

uniform vec3 u_cameraPos;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8.;
    const float maxLayers = 32.;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)

    vec2 P = heightScale * viewDir.xy  / viewDir.z ; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture2D(s_depthMap, currentTexCoords).x;
     
	int loopTime = 0;
    while(currentLayerDepth < currentDepthMapValue && loopTime < 128)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture2D(s_depthMap, currentTexCoords).x;  
        // get depth of next layer
        currentLayerDepth += layerDepth;
		loopTime++;
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture2D(s_depthMap, prevTexCoords).x - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}


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

	vec3 V = normalize(u_cameraPos - v_pos);

	vec2 texCoords = v_texcoord0;

	if(useDepthMap > 0){
		texCoords = ParallaxMapping(v_texcoord0,  V);   
	}
	
    vec3 albedo = texture2D(s_albedo, texCoords).xyz;
    float metallic = texture2D(s_metallic, texCoords).x;
    float roughness = texture2D(s_roughness, texCoords).x;
    float ao = texture2D(s_ao, texCoords).x;



    vec3 N;
    if(useNormalMap > 0){
		vec3 Normal = normalize(v_normal);
		vec3 Tangent = normalize(v_tangent);
		vec3 Bitangent = -normalize(cross(Normal,Tangent));
		mat3 TBN = mat3(Tangent,Bitangent, Normal);
		#if !(BGFX_SHADER_LANGUAGE_HLSL | BGFX_SHADER_LANGUAGE_SPRIV)
			TBN = transpose(TBN); 
		#endif
        vec3 BumpMapNormal = normalize(2.0 * texture2D(s_normal, texCoords).xyz - vec3(1.0, 1.0, 1.0));
        N = normalize(mul(TBN,BumpMapNormal));
    }
    else
        N = normalize(v_normal);

    vec3 R = reflect(-V, N); 

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
        vec4 emissive = texture2D(s_emissive, texCoords);
        Lo = mix(Lo,emissive.xyz,emissive.w);
    }
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  

    vec3 ambient ;
    if(useIBL > 0){
        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureCubeLod(s_prefilterMap, R,  roughness * MAX_REFLECTION_LOD).xyz;    
        vec2 brdf  = texture2D(s_brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).xy;
        vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

        vec3 irradiance = textureCube(s_irradianceMap, N).xyz;
        vec3 diffuse      = irradiance * albedo;
        ambient = (kD * diffuse + specular) * ao;
    }else
        ambient = vec3(0.03,0.03,0.03) * albedo * ao;
  

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0,1.0,1.0));
    color = pow(abs(color), vec3(1.0/2.2,1.0/2.2,1.0/2.2));

    gl_FragColor = vec4(color, 1.0);
}