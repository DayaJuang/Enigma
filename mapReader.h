//=============================================================================
//
// メッシュ地面の処理 [mapReader.h]
// Author : 
//
//=============================================================================
#pragma once
#include <vector>

struct HEIGHTMAPINFO
{
	XMFLOAT4* bmColor;
	XMFLOAT3* heightMap;

	float tx, ty, tz;
	float bx, by, bz;
	int terrainWidth;
	int terrainHeight;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL LoadRawHeightMap(char * filename, HEIGHTMAPINFO & hmInfo);
BOOL LoadBitMap(char * filename, HEIGHTMAPINFO & hmInfo);
BOOL LoadColorMap(char * filename, HEIGHTMAPINFO & hmInfo);