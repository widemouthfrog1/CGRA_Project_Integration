#version 330 core

uniform int uSelectedTree;

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec3 viewDirection;
in vec3 materialColor;
in vec3 actualPosition;

out vec4 outputColor;

float orenNayar(vec3 fragmentNormal, vec3 lightDirection, vec3 viewDirection, vec3 materialDiffuseColor, float materialRoughness){

	float materialRoughnessSquared = materialRoughness * materialRoughness;

	float A = 1 - 0.5 * (materialRoughnessSquared / (materialRoughnessSquared + 0.33));
	float B = 0.45 * (materialRoughnessSquared / (materialRoughnessSquared + 0.09));

	float thetaI = acos(dot(fragmentNormal, lightDirection));
	float thetaR = acos(dot(fragmentNormal, viewDirection));
	float circularAngle = dot(normalize((viewDirection - fragmentNormal) * dot (viewDirection, fragmentNormal)), normalize((lightDirection - fragmentNormal) * dot(lightDirection, fragmentNormal)));

	float alpha = max(thetaI, thetaR);
	float beta = min(thetaI, thetaR);

	float secondPart = A + (B * max(0, circularAngle) * sin(alpha) * tan(beta));

	float diffuseIntensity = dot(fragmentNormal, lightDirection) * secondPart;

	return diffuseIntensity;
}

void main(){

    vec3 materialColor = vec3(0.5, 0.5, 0.5);

    if(uSelectedTree == 1) materialColor = vec3(1, 0, 0);

    vec3 lightColor = vec3(1, 1, 1);
    vec3 lightDirection = vec3(1, -1, 0);

    float NdotL = dot(fragmentNormal, lightDirection);

    float ambientIntensity = 0.2;
    vec3 ambientColor = materialColor * lightColor * ambientIntensity;

    float diffuseRoughness = 0.4;
    float diffuseIntensity = NdotL < 0 ? 0 : max(0, orenNayar(fragmentNormal, lightDirection, viewDirection, materialColor, diffuseRoughness));
    vec3 diffuseColor = materialColor * lightColor * diffuseIntensity * dot(fragmentNormal, lightDirection);

    vec3 shaderColor = ambientColor + diffuseColor;

    outputColor = vec4(shaderColor, 1);

    outputColor = vec4(vec3(1, 0, 0), 1);
    
}
