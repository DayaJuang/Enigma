
// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte境界用
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte境界用
};

cbuffer LightBuffer : register(b4)
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte境界用
};

// フォグ用バッファ
cbuffer FogBuffer : register(b5)
{
	FOG			Fog;
};

// 縁取り用バッファ
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
{
	float4 Camera;
}

cbuffer NormalMap : register(b8)
{
	int normalMap;
	int	padding[3];

}


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture1 : register( t0 );
Texture2D		g_Texture2 : register(t1);
Texture2D		g_normalTexture1 : register(t2);
Texture2D		g_normalTexture2 : register(t3);
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,
						 in  float3 inTangent		: TANGENT0,
						 in  float3 inBinormal		: BINORMAL0,

						 out float4 outDiffuse		: SV_Target )
{
	float4 color;
	float4 color1;
	float4 color2;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float light;
	float blend;
	float lightIntensity;

	float slope = 1.0f - inNormal.y;

	if (Material.noTexSampling == 0)
	{
		if (normalMap == 1)
		{
			float3 tempLightDir = float3(0.0f, 1.0f, 0.0f);

			// テキスチャー1のバンプマッピング
			color1 = g_Texture1.Sample(g_SamplerState, inTexCoord);

			bumpMap = g_normalTexture1.Sample(g_SamplerState, inTexCoord);
			bumpMap = (bumpMap * 2.0f) - 1.0f;
			
			bumpNormal = (bumpMap.x * inTangent) + (bumpMap.y * inBinormal) + (bumpMap.z * inNormal);
			bumpNormal = normalize(bumpNormal);

			lightIntensity = saturate(dot(bumpNormal, tempLightDir));

			color1 = saturate(color1 * lightIntensity);

			// テキスチャー２のバンプマッピング
			color2 = g_Texture2.Sample(g_SamplerState, inTexCoord);

			bumpMap = g_normalTexture2.Sample(g_SamplerState, inTexCoord);
			bumpMap = (bumpMap * 2.0f) - 1.0f;

			bumpNormal = (bumpMap.x * inTangent) + (bumpMap.y * inBinormal) + (bumpMap.z * inNormal);
			bumpNormal = normalize(bumpNormal);

			lightIntensity = saturate(dot(bumpNormal, tempLightDir));

			color2 = saturate(color2 * lightIntensity);


			if (slope < 0.2f)
			{
				blend = slope / 0.2f;
				color = lerp(color1, color2, blend);

			}
			else
			{
				color = color2;
			}
		}
		else
		{
			color = g_Texture1.Sample(g_SamplerState, inTexCoord);
		}

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 5; i++)
		{

			if (Light.Flags[i].y == 1)
			{
				if (Light.Flags[i].x == 1)
				{
					lightDir = normalize(Light.Direction[i].xyz);
					light = dot(lightDir, inNormal.xyz);

					light = 0.6 - 0.5 * light;
					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
				}
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}

				outColor += tempColor;
			}
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;
	}

	//フォグ
	float z = inPosition.z*inPosition.w;
	float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
	f = saturate(f);
	outDiffuse = f * color + (1 - f)*Fog.FogColor;
	outDiffuse.a = color.a;


	//縁取り
	if (fuchi == 1)
	{
		float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));

		if (angle > -0.3f)
		{
			outDiffuse.r  = 1.0f;
			outDiffuse.gb = 0.0f;			
		}
	}
}
