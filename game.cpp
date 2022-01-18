//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

#include "skydome.h"
#include "player.h"
#include "enemy.h"
#include "meshfield.h"
#include "shadow.h"
#include "bullet.h"
#include "playerUI.h"
#include "enemyHP.h"
#include "enemyPoint.h"
#include "minimap.h"
#include "textBox.h"
#include "pointmap.h"
#include "particle.h"
#include "enemyBullet.h"
#include "collision.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 20.0f, 2.0f, 10.0f, 10.0f);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// 空の初期化
	InitSkydome();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// 弾の初期化
	InitBullet();
	InitEnemyBullet();

	// ミニマップ用の初期化
	InitMiniMap();

	InitPoint();

	InitEnemyPoint();

	// プレイヤーUIの初期化
	InitUI();

	// エネミーのHPUIの初期化
	InitEnemyHP();

	// パーティクルの初期化
	InitParticle();

	InitTextBox();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_field);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// パーティクルの終了処理
	UninitParticle();

	// ミニマップの終了処理
	UninitMiniMap();

	UninitPoint();

	UninitEnemyPoint();

	// プレイヤーUIの終了処理
	UninitUI();

	// 弾の終了処理
	UninitBullet();
	UninitEnemyBullet();

	// 地面の終了処理
	UninitMeshField();

	// エネミーの終了処理
	UninitEnemy();

	// エネミーHPの終了処理
	UninitEnemyHP();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

	// 空の終了処理
	UninitSkydome();

	UninitTextBox();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	// 地面処理の更新
	UpdateMeshField();

	// 空の更新
	UpdateSkydome();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// エネミーHPの更新処理
	UpdateEnemyHP();

	// 弾の更新処理
	UpdateBullet();
	UpdateEnemyBullet();

	// パーティクルの更新処理
	UpdateParticle();

	// 影の更新処理
	UpdateShadow();

	// 当たり判定処理
	CheckHit();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if(enemy[i].use)
			UpdateEnemyPoint(enemy[i].pos.x, enemy[i].pos.z,i);
	}

	// ミニマップの更新
	UpdatePoint(player->pos.x,player->pos.z);

	UpdateMiniMap();

	// プレイヤーUIの更新
	UpdateUI();

	UpdateTextBox();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame0(void)
{
	// 3Dの物を描画する処理
	// 地面の描画処理
	DrawMeshField();

	DrawSkydome();

	// 影の描画処理
	DrawShadow();

	// エネミーの描画処理
	DrawEnemy();

	// プレイヤーの描画処理
	DrawPlayer();

	// 弾の描画処理
	DrawBullet();
	DrawEnemyBullet();

	// パーティクルの描画処理
	DrawParticle();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// ミニマップの描画
	DrawMiniMap();

	DrawPoint();

	DrawEnemyPoint();

	// プレイヤーUIの描画
	DrawUI();

	DrawTextBox();

	// エネミーHPの描画処理
	DrawEnemyHP();

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// プレイヤー視点
	pos = GetPlayer()->pos;
	//pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy = GetEnemy();		// エネミーのポインターを初期化
	PLAYER *player = GetPlayer();	// プレイヤーのポインターを初期化
	BULLET *bullet = GetBullet();	// 弾のポインターを初期化

	ENEMYHP *enemyHP = GetEnemyHP();
	ENEMYBULLET *enemyBullet = GetEnemyBullet();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//敵の有効フラグをチェックする
		if (enemy[i].use == FALSE)
			continue;

		//BCの当たり判定
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size) && player->isAttack)
		{
			if (enemy[i].mode == ENEMY_IDLE)
			{
				enemy[i].mode = ENEMY_BATTLE;
			}

			int dmg = (player->slashDmg - 6) + rand() % ((player->slashDmg + 6) - (player->slashDmg - 6));

			// 敵のHPが減らす
			enemy[i].hp -= dmg;

			enemyHP->width = (float)enemy[i].hp;

			PlaySound(SOUND_LABEL_SE_hit);
		}
	}

	// プレイヤーの弾と敵
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾の有効フラグをチェックする
		if (bullet[i].use == FALSE)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].fWidth, enemy[j].size))
			{
				if (enemy[j].mode == ENEMY_IDLE)
				{
					enemy[j].mode = ENEMY_BATTLE;
				}

				int dmg = (player->bulletDmg - 2) + rand() % ((player->bulletDmg + 2) - (player->bulletDmg - 2));

				// 当たったから未使用に戻す
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// 敵のHPが減らす
				enemy[j].hp -= dmg;

				enemyHP->width = (float)enemy[j].hp;

				PlaySound(SOUND_LABEL_SE_hit);

			}
		}

	}

	// エネミーの弾とプレイヤー
	for (int i = 0; i < MAX_EBULLET; i++)
	{
		//弾の有効フラグをチェックする
		if (enemyBullet[i].bUse == FALSE)
			continue;

		//BCの当たり判定
		if (CollisionBC(enemyBullet[i].pos, player->pos, enemyBullet[i].fSizeX, player->size))
		{
			int dmg = enemy[0].dmg;

			// 当たったから未使用に戻す
			enemyBullet[i].bUse = FALSE;
			ReleaseShadow(bullet[i].shadowIdx);

			// プレイヤーのHPが減らす
			player->hp -= dmg;

			PlaySound(SOUND_LABEL_SE_hurt);
		}

	}



	// エネミーが全部死亡したら状態遷移
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].hp <= 0) continue;
		enemy_count++;
	}

	// エネミーが０匹？
	if (enemy_count == 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

}


