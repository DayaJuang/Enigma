//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "enemy.h"
#include "enemyHP.h"
#include "enemyPoint.h"
#include "enemyBullet.h"
#include "shadow.h"
#include "meshfield.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/enemy.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(150.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー

static BOOL				g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = TRUE;

		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(7.0f, 7.0f, 7.0f);

		g_Enemy[i].spd = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// 線形補間用のタイマーをクリア
		g_Enemy[i].tbl_adr = NULL;		// 再生するアニメデータの先頭アドレスをセット
		g_Enemy[i].tbl_size = 0;		// 再生するアニメデータのレコード数をセット

		g_Enemy[i].hp = 1250;
		g_Enemy[i].dmg = 100;
		g_Enemy[i].mode = ENEMY_IDLE;

		g_Enemy[i].use = TRUE;			// TRUE:生きてる

	}

	g_Enemy[0].pos = XMFLOAT3(60000.0f, 5700.0f, 20000.0f);
	g_Enemy[1].pos = XMFLOAT3(-55500.0f, 4700.0f, 16000.0f);
	g_Enemy[2].pos = XMFLOAT3(-50000.0f,7800.0f, -25000.0f);
	g_Enemy[3].pos = XMFLOAT3(32500.0f, 6000.0f, -17500.0f);
	g_Enemy[4].pos = XMFLOAT3(15000.0f, 5000.0f, -66000.0f);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	ENEMYHP *enemyHP = GetEnemyHP();
	ENEMYPOINT *ePoint = GetEnemyPoint();

	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].hp <= 0)
		{
			g_Enemy[i].use = FALSE;
			ePoint[i].use = FALSE;
		}

		if (enemyHP->use && g_Enemy[i].mode == ENEMY_IDLE)
		{
			g_Enemy[i].use = FALSE;
		}
		else if (!enemyHP->use && g_Enemy[i].hp > 0)
		{
			g_Enemy[i].use = TRUE;
		}

		if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
		{									// Yes
			// レイキャストして足元の高さを求める
			XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// ぶつかったポリゴンの法線ベクトル（向き）
			XMFLOAT3 hitPosition;

			hitPosition.y = g_Enemy[i].pos.y - ENEMY_OFFSET_Y;	// 外れた時用に初期化しておく
			bool ans = RayHitField(g_Enemy[i].pos, &hitPosition, &normal);
			g_Enemy[i].pos.y = hitPosition.y + ENEMY_OFFSET_Y;

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			if (g_Enemy[i].mode == ENEMY_BATTLE)
			{
				SetEnemyHP(g_Enemy[i].hp);

				XMFLOAT3 pos;
				XMFLOAT3 move;
				float fAngle, fLength;
				int nLife;
				float fSize;
				float g_fWidthBase = 50.0f;				// 基準の幅
				float g_fHeightBase = 100.0f;			// 基準の高さ


				pos = { g_Enemy[i].pos.x,g_Enemy[i].pos.y - 50.0f, g_Enemy[i].pos.z };

				fAngle = (float)(rand() % 628 - 314) / 100.0f;
				fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
				move.x = sinf(fAngle) * fLength;
				move.y = 0.0f;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 100 + 150;

				fSize = (float)(rand() % 30 + 20);

				// ビルボードの設定
				SetEnemyBullet(pos, move, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife);

			}

			//////////////////////////////////////////////////////////////////////
			// 姿勢制御
			//////////////////////////////////////////////////////////////////////

			XMVECTOR vx, nvx, up;
			XMVECTOR quat;
			float len, angle;

			// ２つのベクトルの外積を取って任意の回転軸を求める
			g_Enemy[i].upVector = normal;
			up = { 0.0f, 1.0f, 0.0f, 0.0f };
			vx = XMVector3Cross(up, XMLoadFloat3(&g_Enemy[i].upVector));

			// 求めた回転軸からクォータニオンを作り出す
			nvx = XMVector3Length(vx);
			XMStoreFloat(&len, nvx);
			nvx = XMVector3Normalize(vx);
			angle = asinf(len);
			quat = XMQuaternionRotationNormal(nvx, angle);

			// 前回のクォータニオンから今回のクォータニオンまでの回転を滑らかにする
			quat = XMQuaternionSlerp(XMLoadFloat4(&g_Enemy[i].quaternion), quat, 0.05f);

			// 今回のクォータニオンの結果を保存する
			XMStoreFloat4(&g_Enemy[i].quaternion, quat);

		}
	}

#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("\nEnemy:X:%f Y:%f HP:%d\n", g_Enemy[2].pos.x, g_Enemy[2].pos.y, g_Enemy[0].hp);
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_BACK);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		if (g_Enemy[i].mode == ENEMY_BATTLE)
		{
			SetFuchi(1);
		}

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// クォータニオンを反映
		XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Enemy[i].quaternion));
		mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Enemy[i].model);

		SetFuchi(0);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
