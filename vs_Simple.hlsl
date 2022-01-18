

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

//=============================================================================
// 頂点シェーダ
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,
						  in  float3 inTangent		: TANGENT0,
						  in  float3 inBinormal		: BINORMAL0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
						  out float3 outTangent		: TANGENT0,
						  out float3 outBinormal	: BINORMAL0
																)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	outTangent = mul(float3(1.0f,1.0f,1.0f), World);
	outTangent = normalize(outTangent);

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	outBinormal = mul(float3(1.0f, 1.0f, 1.0f), World);
	outBinormal = normalize(outBinormal);
}