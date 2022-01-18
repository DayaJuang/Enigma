//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER		(1)					// プレイヤーの数
#define MAX_SKILL		(2)					// プレイヤーの数

#define	PLAYER_SIZE		(100.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adr;	// アニメデータのテーブル先頭アドレス

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス
	
	XMFLOAT4			quaternion;	// クォータニオン
	XMFLOAT3			upVector;	// 自分が立っている所

	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	DX11_MODEL			model;		// モデル情報

	float				spd;		// 移動スピード
	float				dir;		// 向き
	float				size;		// 当たり判定の大きさ
	float				move_time;	// 実行時間

	int					shadowIdx;	// 影のIndex
	int					tbl_size;	// 登録したテーブルのレコード総数
	int					action;
	int					hp;
	int					mp;
	int					maxHp;
	int					maxMp;
	int					bulletDmg;
	int					slashDmg;
	int					waitTime;

	BOOL				canMove;
	BOOL				isAttack;
	BOOL				use;
	BOOL				load;

	// 親は、NULL、子供は親のアドレスを入れる
	PLAYER				*parent;	// 自分が親ならNULL、自分が子供なら親のplayerアドレス
};

struct SKILL
{
	float cooldown;
	float actionCooldown;
	int mpNeeded;
};

enum
{
	BODY,
	NECK,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	HEAD,
	FRONT_HAIR,
	BACK_HAIR,
	LEFT_ARM,
	RIGHT_ARM,
	LEFT_ELBOW,
	RIGHT_ELBOW,
	LEFT_HAND,
	RIGHT_HAND,
	LEFT_THIGH,
	RIGHT_THIGH,
	LEFT_LEG,
	RIGHT_LEG,
	LEFT_KNEE,
	RIGHT_KNEE,
	LEFT_FOOT,
	RIGHT_FOOT,
	SWORD,
	SWORD_ATTACK,
	GUN,
	GUN_RIGHT,
	GUN_LEFT,

	MAX_PARTS
};

enum
{
	IDLE,
	MOVE,
	RUN,
	DASH,
	ATTACK,
	SHOOT
};

enum
{
	SKILL_DASH,
	SKILL_SLASH,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetAnimation(int playerAction);
void SetTableAddress(int index, INTERPOLATION_DATA *animName);

void PlayerRun(void);
void PlayerWalk(void);
void InitParts(void);
void BackToField(void);

PLAYER *GetPlayer(void);
PLAYER *GetPlayerParts(void);

