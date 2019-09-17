#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light{
    vec4 lightPosition;
    vec4 lightColor;
    vec3 lightDirection;
    float lightIntensity;
    int  lightDistAttenCurveType;
    float lightDistAttenCurveParams[5];
    int  lightAngleAttenCurveType;
    float lightAngleAttenCurveParams[5];
}allLights[MAX_LIGHTS];