//=============================================================================
//
// パーティクル処理 [EnemyBullet.h]
// Author : 
//
//=============================================================================
#pragma once

#define	MAX_EBULLET			(5)				// エネミーバレット最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命
	BOOL			bUse;			// 使用しているかどうか

} ENEMYBULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);

int SetEnemyBullet(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorEnemyBullet(int nIdxParticle, XMFLOAT4 col);

ENEMYBULLET *GetEnemyBullet(void);