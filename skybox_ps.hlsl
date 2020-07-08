
struct SKYMAP_PS_INPUT
{
	float4 inPosition : SV_POSITION;
	float3 inTexCoord : TEXCOORD;
};

SamplerState objSamplerState : SAMPLE;
TextureCube SkyMap;


float4 main(SKYMAP_PS_INPUT input) : SV_TARGET
{
	return SkyMap.Sample(objSamplerState, input.inTexCoord);
}