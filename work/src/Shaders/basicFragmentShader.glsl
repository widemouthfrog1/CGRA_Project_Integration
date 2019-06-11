#version 330 core

#define PI 3.14159265

uniform int uSelectedTree;
uniform int uIsTree;

uniform vec2 uHeightExtremes;
uniform float uBlendingScale;

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

float interpolateValue(float minValue, float maxValue, float valueToInterpolate){
	return (valueToInterpolate - minValue)/(maxValue - minValue);
}

void main(){
	
	vec3 materialColor;
	
	float maxHeight = uHeightExtremes.x;
	float minHeight = uHeightExtremes.y;
	float heightRange = maxHeight - minHeight;

	const int amountOfColors = 4;

	vec3 sandColor = vec3(0.75, 0.7, 0.5);
	vec3 grassColor = vec3(0.2, 0.7, 0.2);
	vec3 mountainColor = vec3(0.2, 0.1, 0);
	vec3 snowColor = vec3(0.85, 0.85, 0.85);
	vec3 waterColor = vec3(0.25, 0.65, 0.9);

	materialColor = snowColor;

	vec3 colorArray[amountOfColors];
	float colorHeightArray[amountOfColors];

	colorArray[0] = sandColor;
	colorHeightArray[0] = 0;

	colorArray[1] = grassColor;
	colorHeightArray[1] = 0.2;

	colorArray[2] = mountainColor;
	colorHeightArray[2] = 0.4;

	colorArray[3] = snowColor;
	colorHeightArray[3] = 0.8;

	float currentHeight = interpolateValue(minHeight, maxHeight, actualPosition.y);
	for(int arrayIndex = 0; arrayIndex < colorHeightArray.length(); ++arrayIndex){
		
		if(currentHeight > colorHeightArray[arrayIndex]){
			
			materialColor = colorArray[arrayIndex];

			if(arrayIndex - 1 >= 0){
				float blendingValue = (colorHeightArray[arrayIndex] - colorHeightArray[arrayIndex - 1]) / uBlendingScale;
				if(abs(currentHeight - colorHeightArray[arrayIndex]) < blendingValue){
					float drawStrength = 1 - interpolateValue(-blendingValue, blendingValue, abs(currentHeight - colorHeightArray[arrayIndex]));
					materialColor = materialColor * (1 - drawStrength) + colorArray[arrayIndex - 1] * drawStrength;
				}
			}

			if(arrayIndex + 1 < amountOfColors){
				float blendingValue = (colorHeightArray[arrayIndex + 1] - colorHeightArray[arrayIndex]) / uBlendingScale;
				if(abs(currentHeight - colorHeightArray[arrayIndex + 1]) < blendingValue){
					float drawStrength = 1 - interpolateValue(-blendingValue, blendingValue, abs(currentHeight - colorHeightArray[arrayIndex + 1]));
					materialColor = materialColor * (1 - drawStrength) + colorArray[arrayIndex + 1] * drawStrength;
				}
			}

		}
	}

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

	if(uIsTree == 1) outputColor = vec4(vec3(0, 0, 1), 1);
	if(uIsTree == 1 && uSelectedTree == 1) outputColor = vec4(vec3(1, 0, 0), 1);
}
