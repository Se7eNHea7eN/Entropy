#define MAX_LIGHTS 10
uniform vec4 u_pointLightCount;
//uniform int pointLightCount;
#define pointLightCount int(u_pointLightCount[0])

uniform vec4 u_lightPosition[MAX_LIGHTS];
uniform vec4 u_lightColor[MAX_LIGHTS];