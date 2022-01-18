//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY		(5)			// エネミーの数

#define	ENEMY_SIZE		(200.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	INTERPOLATION_DATA	*tbl_adr;			// アニメデータのテーブル先頭アドレス

	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス

	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	// クォータニオン
	XMFLOAT4			quaternion;	// クォータニオン
	XMFLOAT3			upVector;	// 自分が立っている所

	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	DX11_MODEL			model;				// モデル情報

	BOOL				use;
	BOOL				load;

	float				move_time;			// 実行時間
	float				spd;				// 移動スピード
	float				size;				// 当たり判定の大きさ

	int					shadowIdx;			// 影のインデックス番号
	int					hp;
	int					dmg;
	int					mode;
	int					tbl_size;			// 登録したテーブルのレコード総数

};

enum
{
	ENEMY_IDLE,
	ENEMY_BATTLE
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemyHP(int enemyHP);

ENEMY *GetEnemy(void);

