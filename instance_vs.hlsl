cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gViewProj;
	float4x4 gTexTransform;
}; 

struct VertexIn
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
	row_major float4x4 World : WORLD;
	float4 Color : COLOR;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	
	vout.PosW = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3) vin.World);
		
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);
	
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	vout.Color = vin.Color;

	return vout;
}