#define MAX_LIGHTS 10
uniform int pointLightCount;
uniform struct PointLight{
    vec4 lightPosition;
    vec4 lightColor;
    vec3 lightDirection;
}allPointLights[MAX_LIGHTS];