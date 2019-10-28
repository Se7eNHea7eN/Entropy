$input v_pos, v_normal, v_texcoord0
#include "light.sc"

#include "../common/common.sh"

SAMPLER2D(s_albedo, 0);
uniform vec3 u_cameraPos;
#define shininess 128
#define specularStrength 1.5

void main()
{
	vec3 albedo = texture2D(s_albedo, v_texcoord0).xyz;
	 //计算环境光
    vec3 ambient = vec3(0.1,0.1,0.1);
    vec3 diffuse = vec3(0.0,0.0,0.0);
    vec3 specular= vec3(0.0,0.0,0.0);

	int count = pointLightCount;
    //遍历点光源
    for(int i = 0 ; i < count ; i++ ){
        vec3 pointLightPosition = u_lightPosition[i].xyz;
        vec3 pointLightColor = u_lightColor[i].xyz;

        //法线向量单位化
        vec3 norm = normalize(v_normal);
        //入射光向量单位化
        vec3 lightDir = normalize(pointLightPosition - v_pos.xyz );

     // 计算阴影
       // float shadow = PointShadowCalculation(lightPosition,pointLight,pointLightCubeShadows[i]);
		float shadow = 0.0f;
        //计算法线和入射光向量的点积。两个向量都是单位向量，实际结果为夹角的余弦值。如果是钝角余弦值可能出现负值，这里最小值限制为0。
        float diff = max(dot(norm.xyz , lightDir.xyz ), 0.0);
        //夹角的余弦值和光照颜色相乘得到漫反射色值
        diffuse += (1.0-shadow) *diff * pointLightColor;

        //视向量单位化
        vec3 viewDir = normalize(u_cameraPos.xyz - v_pos.xyz);

                //计算出射向量
                //vec3 reflectDir = reflect(-lightDir, norm);
                //计算高光值 视向量和出射向量夹角余弦值的shininess次幂。
                //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        //优化为blinn-phong
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);

        specular += (1.0-shadow) * specularStrength * spec * pointLightColor;
    }

	vec3 color =  (ambient + diffuse + specular)  * albedo;
    gl_FragColor = vec4(color,1.0);
}
