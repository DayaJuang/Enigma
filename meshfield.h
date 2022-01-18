//=============================================================================
//
// メッシュ地面の処理 [meshfield.h]
// Author : 
//
//=============================================================================
#pragma once
#include "mapReader.h"

#define FIELD_BORDER_X (100000.0f)
#define FIELD_BORDER_Z (100000.0f)

struct TEMP_VERTEX
{
	float x, y, z;
	float tu, tv;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(XMFLOAT3 pos, XMFLOAT3 rot, float sclXZ, float sclY, float fBlockSizeX, float fBlockSizeZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3 *HitPosition, XMFLOAT3 *Normal);
void CalculateFieldNormal(HEIGHTMAPINFO &heightMap);

void CalculateTerrainVector(void);

float GetMapWidth(void);
float GetMapHeight(void);
