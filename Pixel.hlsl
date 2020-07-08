cbuffer lightBuffer : register(b0)
{
	float3 lightDirection;
	float3 lightColor;
	float lightStrength;
	
	float3 dynamicLightColor;
	float dynamicLightStrength;
	float3 dynamicLightPosition;
	
	float lightAttenuation_a;
	float lightAttenuation_b;
	float lightAttenuation_c;

}

struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
	float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
	float3 ambientLight = lightColor * lightStrength;
	
	float3 appliedLight = ambientLight;
	
	float3 vectorLight = normalize(dynamicLightPosition - input.inWorldPos);
	
	float3 lightIntensity = max(dot(vectorLight, input.inNormal), 0);
	
	float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
	float attenuationFactor = 1 / (lightAttenuation_a + lightAttenuation_b * distanceToLight + lightAttenuation_c * pow(distanceToLight, 2));
	
	lightIntensity *= attenuationFactor;
	float diffuseLight = lightIntensity * dynamicLightStrength * dynamicLightColor;
	
	appliedLight += diffuseLight;
	
	float3 finalColor = sampleColor * appliedLight;
	finalColor += dot(lightDirection, input.inNormal) * lightColor * sampleColor;
	return float4(finalColor, 1.0f);
}