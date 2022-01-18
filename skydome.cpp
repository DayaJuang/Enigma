//=============================================================================
//
// モデル処理 [Skydome.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "skydome.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_SKYDOME		"data/MODEL/skydome.obj"			// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SKYDOME		skydome;						// プレイヤー

static BOOL			g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkydome(void)
{
	LoadModel(MODEL_SKYDOME, &skydome.model);
	skydome.load = TRUE;
	
	skydome.pos = { 0.0f, 0.0f, 0.0f };
	skydome.rot = { 0.0f, 0.0f, 0.0f };
	skydome.scl = { 800.0f, 800.0f, 800.0f };
	
	skydome.use = TRUE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSkydome(void)
{
	if (g_Load == FALSE) return;

	// モデルの解放処理
	if (skydome.load)
	{
		UnloadModel(&skydome.model);
		skydome.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSkydome(void)
{
	skydome.rot.y += 0.0001f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkydome(void)
{
	
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(skydome.scl.x, skydome.scl.y, skydome.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(skydome.rot.x, skydome.rot.y, skydome.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(skydome.pos.x, skydome.pos.y, skydome.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&skydome.mtxWorld, mtxWorld);


	// モデル描画
	DrawModel(&skydome.model);

	SetLightEnable(TRUE);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
SKYDOME *GetSkydome(void)
{
	return &skydome;
}

