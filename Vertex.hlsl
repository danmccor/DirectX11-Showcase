#pragma pack_matrix(row_major)

cbuffer perObjectBuffer : register(b0)
{
	row_major float4x4 wvpMat;
	row_major float4x4 worldMat;
};


struct VS_INPUT
{
	float3 inPos : POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
	uint Instance : SV_INSTANCEID;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float2 outTexCoord : TEXCOORD;
	float3 outNormal : NORMAL;
	float3 outWorldPos : WORLD_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.outPosition = mul(float4(input.inPos, 1.0f), wvpMat);
	output.outTexCoord = input.inTexCoord;
	output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), worldMat));
	float3 eyer = -normalize(output.outPosition);
	output.outWorldPos = mul(float4(input.inPos, 1.0f), worldMat);
	
	return output;
}

