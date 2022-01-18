//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "playerUI.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "meshfield.h"
#include "fade.h"
#include "textBox.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER_BODY				"data/MODEL/body.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_SWORD				"data/MODEL/sword.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_SWORDATTACK		"data/MODEL/swordAttack.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_GUN				"data/MODEL/gunBack.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_GUNR				"data/MODEL/gun.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_GUNL				"data/MODEL/gun.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_NECK				"data/MODEL/neck.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_SHOULDER			"data/MODEL/rigthShoulder.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_HEAD				"data/MODEL/head.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_FRONTHAIR			"data/MODEL/frontHair.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_BACKHAIR			"data/MODEL/backHair.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_LEFTARM			"data/MODEL/leftArm.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_RIGHTARM			"data/MODEL/rightArm.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_ELBOW				"data/MODEL/elbow.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_LEFTHAND			"data/MODEL/leftHand.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_RIGHTHAND			"data/MODEL/rightHand.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_THIGH				"data/MODEL/thigh.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_LEFTLEG			"data/MODEL/leftLeg.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_RIGHTLEG			"data/MODEL/rightLeg.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_KNEE				"data/MODEL/knee.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_LEFTFOOT			"data/MODEL/leftFoot.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_RIGHTFOOT			"data/MODEL/rightFoot.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(10.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define PLAYER_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(36.5f)							// プレイヤーの足元をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;						// プレイヤー

static PLAYER		g_Parts[MAX_PARTS];		// プレイヤーのパーツ用

static SKILL		g_Skills[MAX_SKILL];

static BOOL			g_Load = FALSE;

#pragma region INTERPOLATION DATA

//=============================================================================
// アイドルアニメーション
//=============================================================================

static INTERPOLATION_DATA idle_tbl_neck[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA idle_tbl_backHair[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA idle_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA idle_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -2.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -2.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA idle_tbl_armL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA idle_tbl_handL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


static INTERPOLATION_DATA idle_tbl_armR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA idle_tbl_handR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


static INTERPOLATION_DATA idle_tbl_legL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),   	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA idle_tbl_legR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA idle_tbl_footL[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA idle_tbl_footR[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


//=============================================================================
// 歩くアニメーション
//=============================================================================

static INTERPOLATION_DATA walk_tbl_neck[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA walk_tbl_backHair[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA walk_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f,-1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f,-1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.5f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA walk_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -2.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -2.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA walk_tbl_armL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		          XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),       XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),       XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),       XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA walk_tbl_handL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f,0.0f, 0.0f),			    XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f,0.0f, 0.0f),			    XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


static INTERPOLATION_DATA walk_tbl_armR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA walk_tbl_handR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f,0.0f, 0.0f),			     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f,0.0f, 0.0f),			     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f,0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


static INTERPOLATION_DATA walk_tbl_legL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),	    XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA walk_tbl_legR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		         XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA walk_tbl_footL[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA walk_tbl_footR[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};



//=============================================================================
// 走るアニメーション
//=============================================================================

static INTERPOLATION_DATA move_tbl_neck[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA move_tbl_backHair[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA move_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-1.5f, 1.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-1.5f, 1.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 0.5f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 1.5f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA move_tbl_armL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA move_tbl_handL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f,-XM_PI / 8.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f,-XM_PI / 8.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f,-XM_PI / 8.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA move_tbl_armR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA move_tbl_handR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, XM_PI / 8.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, XM_PI / 8.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA move_tbl_legL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),	    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),	    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA move_tbl_footL[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),		 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA move_tbl_legR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		         XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};



static INTERPOLATION_DATA move_tbl_footR[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

//=============================================================================
// ダッシュアニメーション
//=============================================================================

static INTERPOLATION_DATA dash_tbl_neck[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

static INTERPOLATION_DATA dash_tbl_backHair[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -7.0f, -0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, -0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(0.0f, -6.8f, -1.0f),  XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, -7.0f, -0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, -7.0f, -0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

static INTERPOLATION_DATA dash_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),			   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),			   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-5.0f, 0.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f,-5.0f, 5.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f,-3.0f,10.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f,-5.0f, 0.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(0.0f,-5.0f, 0.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f,-5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),			   XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};


static INTERPOLATION_DATA dash_tbl_armL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),           XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),           XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),           XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),           XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

static INTERPOLATION_DATA dash_tbl_handL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f,0.0f, 0.0f),                XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f,0.0f, 0.0f),                XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};


static INTERPOLATION_DATA dash_tbl_armR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),	  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

static INTERPOLATION_DATA dash_tbl_handR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),                  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),                  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),          XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, XM_PI / 8.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, XM_PI / 8.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),          XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),                  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),                  XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

static INTERPOLATION_DATA dash_tbl_legL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 3.0f, 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(XM_PI / 3.0f, 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f, 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),          	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};


static INTERPOLATION_DATA dash_tbl_footL[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),		     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),		     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};


static INTERPOLATION_DATA dash_tbl_legR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

static INTERPOLATION_DATA dash_tbl_footR[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },

	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 2.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
};

//=============================================================================
// 攻撃 1 アニメーション
//=============================================================================

static INTERPOLATION_DATA attack1_neck[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),            XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 10, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 10, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 10, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),            XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA attack1_backHair[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA attack1_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-1.0f, 1.0f),  XMFLOAT3(XM_PI / 12, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-2.0f, 3.0f),  XMFLOAT3(XM_PI / 10, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f,-3.0f, 5.0f),  XMFLOAT3(XM_PI / 8, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-4.0f, 7.0f),  XMFLOAT3(XM_PI / 6, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-5.0f, 9.0f),  XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.0f,-6.0f, 11.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-7.0f, 11.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.0f,-7.0f, 11.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA attack1_armL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 1.5f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2.5f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 3.5f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 4.5f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 5.5f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 6.5f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 6.5f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 14.5f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 14.5f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA attack1_handL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f, 0.0f, -XM_PI / 5.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA attack1_armR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 1.4f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2.4f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 3.4f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 4.4f,  0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 5.4f,  0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 6.4f,  0.0f, 0.0f),             XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 8.4f, 0.0f, 0.0f),              XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 14.4f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 14.4f, 0.0f, 0.0f),	          XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA attack1_handR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(-XM_PI / 3.0f, 0.0f, XM_PI / 5.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA attack1_legL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 12.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 8.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 6.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 4.0f , 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 2.0f , 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 1.5f , 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 1.5f , 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 1.5f , 0.0f, 0.0f),  	XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(-XM_PI / 1.5f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA attack1_footL[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 3 , 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};


static INTERPOLATION_DATA attack1_legR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f , 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

static INTERPOLATION_DATA attack1_footR[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 12.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 8.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 6.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },

	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f , 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(XM_PI / 4.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 3 },
};

//=============================================================================
// 弾発射 アニメーション
//=============================================================================

static INTERPOLATION_DATA shoot_tbl_neck[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA shoot_tbl_backHair[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -7.0f, 0.3f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA shoot_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA shoot_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -2.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -2.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.5f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, -10.0f),  XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA shoot_tbl_armL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(2.5f, 3.5f, -1.0f),  XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA shoot_tbl_handL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


static INTERPOLATION_DATA shoot_tbl_armR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),       XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-2.5f, 3.5f, -1.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA shoot_tbl_handR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-0.3f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};


static INTERPOLATION_DATA shoot_tbl_legL[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),   	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA shoot_tbl_legR[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-1.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),        XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA shoot_tbl_footL[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(-0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

static INTERPOLATION_DATA shoot_tbl_footR[] = {	// pos, rot, scl, frame
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{  XMFLOAT3(0.5f, -3.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),    	XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
};

#pragma endregion


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;
	LoadModel(MODEL_PLAYER_BODY, &g_Player.model);

	// プレイヤーの初期化
	g_Player.pos = { 38500.0f, 11450.0f, -89800.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 2.0f, 2.0f, 2.0f };

	g_Player.spd = 0.0f;			// 移動スピードクリア
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	g_Player.maxHp = g_Player.hp = 500;
	g_Player.maxMp = g_Player.mp = 450;
	g_Player.bulletDmg = 20;
	g_Player.slashDmg = 50;
	g_Player.waitTime = 0;
	g_Player.isAttack = FALSE;

	g_Player.use = TRUE;

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	// 階層アニメーション用の初期化処理
	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる
	g_Player.canMove = TRUE;

	// パーツの初期化
	InitParts();

	// スキル初期化
	// ダッシュ
	g_Skills[SKILL_DASH].cooldown = 0.0f;
	g_Skills[SKILL_DASH].actionCooldown = 0.0f;
	g_Skills[SKILL_DASH].mpNeeded = 50;

	// スラッシュ
	g_Skills[SKILL_SLASH].cooldown = 10.0f;
	g_Skills[SKILL_SLASH].actionCooldown = 0.0f;
	g_Skills[SKILL_SLASH].mpNeeded = 200;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		
		g_Player.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	TEXTBOX *textBox = GetTextBox();

	int oldAnim = g_Player.action;	// アニメーションを格納

	// スキルクールダウン処理
	for (int i = 0; i < MAX_SKILL; i++)
	{
		if (g_Skills[i].actionCooldown > 0.0f)
		{
			g_Skills[i].actionCooldown -= 0.1f;

			if (g_Skills[i].actionCooldown < 0.0f)
				g_Skills[i].actionCooldown = 0.0f;
		}
	}

	// キーボードのインプット処理
	if ((GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT)) && g_Player.canMove)
	{	// 左へ移動
		g_Player.dir = XM_PI / 2;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}
	if ((GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT)) && g_Player.canMove)
	{	// 右へ移動
		g_Player.dir = -XM_PI / 2;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}

	if ((GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP)) && g_Player.canMove)
	{	// 上へ移動
		g_Player.dir = XM_PI;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}
	if ((GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN)) && g_Player.canMove)
	{	// 下へ移動
		g_Player.dir = 0.0f;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}

	// ダッシュ処理
	if ((GetKeyboardTrigger(DIK_D) || IsButtonPressed(0, BUTTON_B)) && g_Player.mp >= g_Skills[SKILL_DASH].mpNeeded && !textBox[FRAME_BOX].use)
	{
		g_Player.canMove = FALSE;
		g_Player.mp -= g_Skills[SKILL_DASH].mpNeeded;
		g_Player.action = DASH;
	}

	// スラッシュ処理
	if ((GetKeyboardTrigger(DIK_X) || IsButtonPressed(0, BUTTON_Y)) && g_Player.mp >= g_Skills[SKILL_SLASH].mpNeeded && g_Skills[SKILL_SLASH].actionCooldown <= 0.0f && !textBox[FRAME_BOX].use)
	{
		PlaySound(SOUND_LABEL_SE_swing);

		g_Player.canMove = FALSE;
		g_Player.mp -= g_Skills[SKILL_SLASH].mpNeeded;
		g_Player.action = ATTACK;

		g_Skills[SKILL_SLASH].actionCooldown = 10.0f;
	}

	// アニメーションを切り替わる
	SetAnimation(g_Player.action);

	if (g_Player.action == ATTACK)
	{
		g_Parts[SWORD].use = FALSE;
		g_Parts[SWORD_ATTACK].use = TRUE;
	}
	else
	{
		g_Parts[SWORD].use = TRUE;
		g_Parts[SWORD_ATTACK].use = FALSE;
	}

	if (g_Player.action == SHOOT)
	{
		g_Parts[GUN].use = FALSE;
		g_Parts[GUN_LEFT].use = TRUE;
		g_Parts[GUN_RIGHT].use = TRUE;
	}
	else
	{
		g_Parts[GUN].use = TRUE;
		g_Parts[GUN_LEFT].use = FALSE;
		g_Parts[GUN_RIGHT].use = FALSE;
	}

	//	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// 入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	// 弾発射処理
	if ((GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_X)) && !textBox[FRAME_BOX].use)
	{
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 8.0f;
		g_Player.canMove = FALSE;
		g_Player.action = SHOOT;
		SetBullet(pos, g_Player.rot);
	}

	// フィールドから離れるかどうか確認
	if (g_Player.pos.x < -(FIELD_BORDER_X) || g_Player.pos.x > (FIELD_BORDER_X) || g_Player.pos.z > (FIELD_BORDER_Z) || g_Player.pos.z < -(FIELD_BORDER_Z))
	{
		g_Player.canMove = FALSE;
		ShowTextBox(TRUE);
	}

	if (textBox[FRAME_BOX].use == TRUE)
	{
		if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SOUND_LABEL_SE_click);

			textBox[CURSOR].cursoPos = (textBox[CURSOR].cursoPos - 1) % 3;

			if (textBox[CURSOR].cursoPos < 0)
				textBox[CURSOR].cursoPos = CURSOR_MAX_POSITION - 1;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SOUND_LABEL_SE_click);

			textBox[CURSOR].cursoPos = (textBox[CURSOR].cursoPos + 1) % CURSOR_MAX_POSITION;
		}

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			switch (textBox[CURSOR].cursoPos)
			{
			case 0:
				SetFade(FADE_OUT, MODE_TITLE);
				break;

			case 1:
				ShowTextBox(FALSE);
				textBox[CURSOR].cursoPos = 0;
				BackToField();
				break;

			default:
				break;
			}
		}

		switch (textBox[CURSOR].cursoPos)
		{
		case 0:
			textBox[CURSOR].position = textBox[YES_TEXT].position;
			break;

		case 1:
			textBox[CURSOR].position = textBox[NO_TEXT].position;
			break;

		default:
			break;
		}
	}

	// レイキャストして足元の高さを求める
	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// ぶつかったポリゴンの法線ベクトル（向き）
	XMFLOAT3 hitPosition;

	hitPosition.y = g_Player.pos.y - PLAYER_OFFSET_Y;	// 外れた時用に初期化しておく
	bool ans = RayHitField(g_Player.pos, &hitPosition, &normal);
	g_Player.pos.y = hitPosition.y + PLAYER_OFFSET_Y;

	g_Player.spd *= 0.3f;

	if (g_Player.spd <= 0.1f && g_Player.canMove)
	{
		g_Player.action = IDLE;
		g_Player.canMove = TRUE;
	}

	// プレイヤーアクションが変わった場合、アニメーションフレームをリセット
	if (g_Player.action != oldAnim)
	{
		for (int i = 0; i < MAX_PARTS; i++)
		{
			g_Parts[i].move_time = 0.0f;
		}
	}

	// 階層アニメーション
	for (int i = 0; i < MAX_PARTS; i++)
	{
		// 使われているなら処理する
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tbl_adr != NULL))
		{
			// 移動処理
			int		index = (int)g_Parts[i].move_time;
			float	time = g_Parts[i].move_time - index;
			int		size = g_Parts[i].tbl_size;

			float dt = 1.0f / g_Parts[i].tbl_adr[index].frame;	// 1フレームで進める時間
			g_Parts[i].move_time += dt;					// アニメーションの合計時間に足す

			// ダッシュ用（フレーム3と4のときに動く）
			if (g_Player.action == DASH && (index == 3 || index == 4))
			{
				g_Player.spd = VALUE_MOVE * 5.0f;
			}

			if (g_Player.action == ATTACK && (index == 4))
			{
				g_Player.isAttack = TRUE;
			}
			else
			{
				g_Player.isAttack = FALSE;
			}

			if (index > (size - 2))	// ゴールをオーバーしていたら、最初へ戻す
			{
				g_Parts[i].move_time = 0.0f;
				index = 0;

				if (!g_Player.canMove)
				{
					g_Player.action = IDLE;
					g_Player.canMove = TRUE;

					return;
				}
			}

			// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
			XMVECTOR p1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].pos);	// 次の場所
			XMVECTOR p0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].pos);	// 現在の場所
			XMVECTOR vec = p1 - p0;
			XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

			// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
			XMVECTOR r1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].rot);	// 次の角度
			XMVECTOR r0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].rot);	// 現在の角度
			XMVECTOR rot = r1 - r0;
			XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

			// scaleを求める S = StartX + (EndX - StartX) * 今の時間
			XMVECTOR s1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].scl);	// 次のScale
			XMVECTOR s0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].scl);	// 現在のScale
			XMVECTOR scl = s1 - s0;
			XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);

		}
	}

	// プレイヤーのHPやMPバーの更新
	UI *playerUI = GetUI();

	// HPバーの更新処理
	playerUI[HP_BAR].width = (float) g_Player.hp;

	// プレイヤーのHPがマイナスにならないように
	if (playerUI[HP_BAR].width < 0.0f)
	{
		playerUI[HP_BAR].width = 0.0f;

		g_Player.use = FALSE;

		SetFade(FADE_OUT, MODE_RESULT);

	}
	else if (playerUI[HP_BAR].width > g_Player.maxHp)
	{
		// プレイヤーのHPが最大の値よりオーバーしないように
		playerUI[HP_BAR].width = (float)g_Player.maxHp;
	}

	// MPバーの更新処理
	playerUI[MP_BAR].width = (float) g_Player.mp;

	// プレイヤーのMPがマイナスにならないように
	if (playerUI[MP_BAR].width < 0.0f)
	{
		playerUI[MP_BAR].width = 0.0f;
	}
	else if (playerUI[MP_BAR].width > g_Player.maxMp)
	{
		// プレイヤーのMPが最大の値よりオーバーしないように
		playerUI[MP_BAR].width = (float)g_Player.maxMp;
	}

	// プレイヤーのMPを回復処理
	if (g_Player.action != ATTACK && g_Player.action != RUN && g_Player.action != SHOOT && g_Player.action != DASH)
	{
		g_Player.waitTime += 1;

		// mpを回復する
		if (g_Player.waitTime >= 120 && g_Player.mp < g_Player.maxMp)
		{
			g_Player.mp += 2;
		}
	}
	else
	{
		g_Player.waitTime = 0;
	}

	//{	// ポイントライトのテスト
	//	LIGHT *light = GetLightData(1);
	//	XMFLOAT3 pos = g_Player.pos;
	//	pos.y += 20.0f;

	//	light->Position = pos;
	//	light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type = LIGHT_TYPE_POINT;
	//	light->Enable = TRUE;
	//	SetLightData(1, light);
	//}

	//////////////////////////////////////////////////////////////////////
	// 姿勢制御
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;

	// ２つのベクトルの外積を取って任意の回転軸を求める
	g_Player.upVector = normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));

	// 求めた回転軸からクォータニオンを作り出す
	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	angle = asinf(len);
	quat = XMQuaternionRotationNormal(nvx, angle);

	// 前回のクォータニオンから今回のクォータニオンまでの回転を滑らかにする
	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.05f);

	// 今回のクォータニオンの結果を保存する
	XMStoreFloat4(&g_Player.quaternion, quat);

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Player.pos;
	XMFLOAT3 rot = g_Player.rot;
	pos.y -= (PLAYER_OFFSET_Y - 0.05f);
	SetPositionShadow(g_Player.shadowIdx, pos);

#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	PrintDebugProc("\nPlayer HP:%d", g_Player.hp);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	if (g_Player.use)
	{
		// カリング設定を戻す
		SetCullingMode(CULL_MODE_BACK);

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// クォータニオンを反映
		XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.quaternion));
		mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

		// モデル描画
		//DrawModel(&g_Player.model);

		// パーツの階層アニメーション
		for (int i = 0; i < MAX_PARTS; i++)
		{
			
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
			{
				mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
				// ↑
				// g_Player.mtxWorldを指している
			}

			XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

			// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
			if (g_Parts[i].use == FALSE) continue;

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// モデル描画
			DrawModel(&g_Parts[i].model);

		}
	}
}

// アニメーションを切り替わる
void SetAnimation(int playerAction)
{
	switch (playerAction)
	{
	case IDLE:

		SetTableAddress(NECK, idle_tbl_neck);
		SetTableAddress(BACK_HAIR, idle_tbl_backHair);
		SetTableAddress(BODY, idle_tbl_body);
		SetTableAddress(SWORD, idle_tbl_sword);
		SetTableAddress(RIGHT_SHOULDER, idle_tbl_armR);
		SetTableAddress(LEFT_SHOULDER, idle_tbl_armL);
		SetTableAddress(RIGHT_ELBOW, idle_tbl_handR);
		SetTableAddress(LEFT_ELBOW, idle_tbl_handL);
		SetTableAddress(RIGHT_THIGH, idle_tbl_legR);
		SetTableAddress(RIGHT_KNEE, idle_tbl_footR);
		SetTableAddress(LEFT_THIGH, idle_tbl_legL);
		SetTableAddress(LEFT_KNEE, idle_tbl_footL);

		break;

	case MOVE:

		SetTableAddress(NECK, walk_tbl_neck);
		SetTableAddress(BACK_HAIR, walk_tbl_backHair);
		SetTableAddress(BODY, walk_tbl_body);
		SetTableAddress(SWORD, walk_tbl_sword);
		SetTableAddress(RIGHT_SHOULDER, walk_tbl_armR);
		SetTableAddress(LEFT_SHOULDER, walk_tbl_armL);
		SetTableAddress(RIGHT_ELBOW, walk_tbl_handR);
		SetTableAddress(LEFT_ELBOW, walk_tbl_handL);
		SetTableAddress(RIGHT_THIGH, walk_tbl_legR);
		SetTableAddress(RIGHT_KNEE, walk_tbl_footR);
		SetTableAddress(LEFT_THIGH, walk_tbl_legL);
		SetTableAddress(LEFT_KNEE, walk_tbl_footL);

		break;

	case RUN:

		SetTableAddress(NECK, move_tbl_neck);
		SetTableAddress(BACK_HAIR, move_tbl_backHair);
		SetTableAddress(BODY, move_tbl_body);
		SetTableAddress(RIGHT_SHOULDER, move_tbl_armR);
		SetTableAddress(LEFT_SHOULDER, move_tbl_armL);
		SetTableAddress(RIGHT_ELBOW, move_tbl_handR);
		SetTableAddress(LEFT_ELBOW, move_tbl_handL);
		SetTableAddress(RIGHT_THIGH, move_tbl_legR);
		SetTableAddress(RIGHT_KNEE, move_tbl_footR);
		SetTableAddress(LEFT_THIGH, move_tbl_legL);
		SetTableAddress(LEFT_KNEE, move_tbl_footL);

		break;

	case DASH:

		SetTableAddress(NECK, dash_tbl_neck);
		SetTableAddress(BACK_HAIR, dash_tbl_backHair);
		SetTableAddress(BODY, dash_tbl_body);
		SetTableAddress(RIGHT_SHOULDER, dash_tbl_armR);
		SetTableAddress(LEFT_SHOULDER, dash_tbl_armL);
		SetTableAddress(RIGHT_ELBOW, dash_tbl_handR);
		SetTableAddress(LEFT_ELBOW, dash_tbl_handL);

		SetTableAddress(RIGHT_THIGH, dash_tbl_legR);
		SetTableAddress(RIGHT_KNEE, dash_tbl_footR);
		SetTableAddress(LEFT_THIGH, dash_tbl_legL);
		SetTableAddress(LEFT_KNEE, dash_tbl_footL);

		break;

	case ATTACK:

		SetTableAddress(NECK, attack1_neck);
		SetTableAddress(BACK_HAIR, attack1_backHair);
		SetTableAddress(BODY, attack1_body);
		SetTableAddress(RIGHT_SHOULDER, attack1_armR);
		SetTableAddress(LEFT_SHOULDER, attack1_armL);
		SetTableAddress(RIGHT_ELBOW, attack1_handR);
		SetTableAddress(LEFT_ELBOW, attack1_handL);

		SetTableAddress(RIGHT_THIGH, attack1_legR);
		SetTableAddress(RIGHT_KNEE, attack1_footR);
		SetTableAddress(LEFT_THIGH, attack1_legL);
		SetTableAddress(LEFT_KNEE, attack1_footL);

		break;

	case SHOOT:

		SetTableAddress(NECK, shoot_tbl_neck);
		SetTableAddress(BACK_HAIR, shoot_tbl_backHair);
		SetTableAddress(BODY, shoot_tbl_body);
		SetTableAddress(RIGHT_SHOULDER, shoot_tbl_armR);
		SetTableAddress(LEFT_SHOULDER, shoot_tbl_armL);
		SetTableAddress(RIGHT_ELBOW, shoot_tbl_handR);
		SetTableAddress(LEFT_ELBOW, shoot_tbl_handL);

		SetTableAddress(RIGHT_THIGH, shoot_tbl_legR);
		SetTableAddress(RIGHT_KNEE, shoot_tbl_footR);
		SetTableAddress(LEFT_THIGH, shoot_tbl_legL);
		SetTableAddress(LEFT_KNEE, shoot_tbl_footL);

		break;

	default:
		break;
	}

}

// 線形保管を設定する
void SetTableAddress(int index, INTERPOLATION_DATA *animName)
{
	g_Parts[index].tbl_adr = animName;
}

// プレイヤー移動処理
void PlayerRun(void)
{
	g_Player.spd = VALUE_MOVE * 4.0f;
	g_Player.action = RUN;
}

void PlayerWalk(void)
{
	g_Player.spd = VALUE_MOVE;
	g_Player.action = MOVE;
}

// パーツの初期化
void InitParts(void)
{
	// パーツの初期化
	for (int i = 0; i < MAX_PARTS; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = &g_Player;		// ← ここに親のアドレスを入れる

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].tbl_adr = NULL;		// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i].move_time = 0.0f;	// 実行時間をクリア
		g_Parts[i].tbl_size = 0;		// 再生するアニメデータのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = 0;
	}

	g_Parts[BODY].use = TRUE;
	g_Parts[BODY].parent = &g_Player;												// 親をセット
	g_Parts[BODY].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Parts[BODY].tbl_adr = walk_tbl_body;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[BODY].tbl_size = sizeof(walk_tbl_body) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[BODY].load = 1;
	LoadModel(MODEL_PLAYER_BODY, &g_Parts[BODY].model);

	g_Parts[NECK].use = TRUE;
	g_Parts[NECK].parent = &g_Parts[BODY];											// 親をセット
	g_Parts[NECK].pos = XMFLOAT3(0.0f, 5.0f, 0.0f);
	g_Parts[NECK].tbl_adr = move_tbl_neck;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[NECK].tbl_size = sizeof(move_tbl_neck) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[NECK].load = 1;
	LoadModel(MODEL_PLAYER_NECK, &g_Parts[NECK].model);

	g_Parts[LEFT_SHOULDER].use = TRUE;
	g_Parts[LEFT_SHOULDER].parent = &g_Parts[BODY];											// 親をセット
	g_Parts[LEFT_SHOULDER].pos = XMFLOAT3(2.5f, 3.5f, -1.0f);
	g_Parts[LEFT_SHOULDER].tbl_adr = move_tbl_armL;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_SHOULDER].tbl_size = sizeof(move_tbl_armL) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_SHOULDER].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER, &g_Parts[LEFT_SHOULDER].model);

	g_Parts[RIGHT_SHOULDER].use = TRUE;
	g_Parts[RIGHT_SHOULDER].parent = &g_Parts[BODY];											// 親をセット
	g_Parts[RIGHT_SHOULDER].pos = XMFLOAT3(-2.5f, 3.5f, -1.0f);
	g_Parts[RIGHT_SHOULDER].tbl_adr = move_tbl_armR;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_SHOULDER].tbl_size = sizeof(move_tbl_armR) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_SHOULDER].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER, &g_Parts[RIGHT_SHOULDER].model);

	g_Parts[HEAD].use = TRUE;
	g_Parts[HEAD].parent = &g_Parts[NECK];				// 親をセット
	g_Parts[HEAD].pos = XMFLOAT3(0.0f, 5.3f, -2.0f);
	g_Parts[HEAD].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[HEAD].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[HEAD].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &g_Parts[HEAD].model);

	g_Parts[FRONT_HAIR].use = TRUE;
	g_Parts[FRONT_HAIR].parent = &g_Parts[HEAD];				// 親をセット
	g_Parts[FRONT_HAIR].pos = XMFLOAT3(0.0f, -3.5f, 2.0f);
	g_Parts[FRONT_HAIR].tbl_adr = NULL;							// 再生するアニメデータの先頭アドレスをセット
	g_Parts[FRONT_HAIR].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[FRONT_HAIR].load = 1;
	LoadModel(MODEL_PLAYER_FRONTHAIR, &g_Parts[FRONT_HAIR].model);

	g_Parts[BACK_HAIR].use = TRUE;
	g_Parts[BACK_HAIR].parent = &g_Parts[HEAD];												// 親をセット
	g_Parts[BACK_HAIR].pos = XMFLOAT3(0.0f, -7.0f, 0.3f);
	g_Parts[BACK_HAIR].tbl_adr = move_tbl_backHair;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[BACK_HAIR].tbl_size = sizeof(move_tbl_backHair) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[BACK_HAIR].load = 1;
	LoadModel(MODEL_PLAYER_BACKHAIR, &g_Parts[BACK_HAIR].model);

	g_Parts[LEFT_ARM].use = TRUE;
	g_Parts[LEFT_ARM].parent = &g_Parts[LEFT_SHOULDER];		// 親をセット
	g_Parts[LEFT_ARM].pos = XMFLOAT3(0.5f, -2.0f, 0.0f);
	g_Parts[LEFT_ARM].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_ARM].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_ARM].load = 1;
	LoadModel(MODEL_PLAYER_LEFTARM, &g_Parts[LEFT_ARM].model);

	g_Parts[RIGHT_ARM].use = TRUE;
	g_Parts[RIGHT_ARM].parent = &g_Parts[RIGHT_SHOULDER];		// 親をセット
	g_Parts[RIGHT_ARM].pos = XMFLOAT3(-0.5f, -2.0f, 0.0f);
	g_Parts[RIGHT_ARM].tbl_adr = NULL;							// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_ARM].tbl_size = NULL;							// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_ARM].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTARM, &g_Parts[RIGHT_ARM].model);

	g_Parts[LEFT_ELBOW].use = TRUE;
	g_Parts[LEFT_ELBOW].parent = &g_Parts[LEFT_ARM];										// 親をセット
	g_Parts[LEFT_ELBOW].pos = XMFLOAT3(0.3f, -2.5f, 0.0f);
	g_Parts[LEFT_ELBOW].tbl_adr = move_tbl_handL;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_ELBOW].tbl_size = sizeof(move_tbl_handL) / sizeof(INTERPOLATION_DATA);;	// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_ELBOW].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW, &g_Parts[LEFT_ELBOW].model);

	g_Parts[RIGHT_ELBOW].use = TRUE;
	g_Parts[RIGHT_ELBOW].parent = &g_Parts[RIGHT_ARM];										// 親をセット
	g_Parts[RIGHT_ELBOW].pos = XMFLOAT3(-0.3f, -2.5f, 0.0f);
	g_Parts[RIGHT_ELBOW].tbl_adr = move_tbl_handR;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_ELBOW].tbl_size = sizeof(move_tbl_handR) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_ELBOW].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW, &g_Parts[RIGHT_ELBOW].model);

	g_Parts[LEFT_HAND].use = TRUE;
	g_Parts[LEFT_HAND].parent = &g_Parts[LEFT_ELBOW];		// 親をセット
	g_Parts[LEFT_HAND].pos = XMFLOAT3(0.0f, -3.0f, 0.0f);
	g_Parts[LEFT_HAND].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_HAND].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_HAND].load = 1;
	LoadModel(MODEL_PLAYER_LEFTHAND, &g_Parts[LEFT_HAND].model);

	g_Parts[RIGHT_HAND].use = TRUE;
	g_Parts[RIGHT_HAND].parent = &g_Parts[RIGHT_ELBOW];		// 親をセット
	g_Parts[RIGHT_HAND].pos = XMFLOAT3(0.0f, -3.0f, 0.0f);
	g_Parts[RIGHT_HAND].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_HAND].tbl_size = NULL;					// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_HAND].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTHAND, &g_Parts[RIGHT_HAND].model);

	g_Parts[LEFT_THIGH].use = TRUE;
	g_Parts[LEFT_THIGH].parent = &g_Parts[BODY];										// 親をセット
	g_Parts[LEFT_THIGH].pos = XMFLOAT3(1.5f, -4.5f, 0.0f);								// 親をセット
	g_Parts[LEFT_THIGH].tbl_adr = move_tbl_legL;										// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_THIGH].tbl_size = sizeof(move_tbl_legL) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_THIGH].load = 1;
	LoadModel(MODEL_PLAYER_THIGH, &g_Parts[LEFT_THIGH].model);

	g_Parts[RIGHT_THIGH].use = TRUE;
	g_Parts[RIGHT_THIGH].parent = &g_Parts[BODY];											// 親をセット
	g_Parts[RIGHT_THIGH].pos = XMFLOAT3(-1.5f, -4.5f, 0.0f);								// 親をセット
	g_Parts[RIGHT_THIGH].tbl_adr = move_tbl_legR;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_THIGH].tbl_size = sizeof(move_tbl_legR) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_THIGH].load = 1;
	LoadModel(MODEL_PLAYER_THIGH, &g_Parts[RIGHT_THIGH].model);

	g_Parts[LEFT_LEG].use = TRUE;
	g_Parts[LEFT_LEG].parent = &g_Parts[LEFT_THIGH];			// 親をセット
	g_Parts[LEFT_LEG].pos = XMFLOAT3(0.5f, -3.0f, 0.0f);		// 親をセット
	g_Parts[LEFT_LEG].tbl_adr = NULL;							// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_LEG].tbl_size = NULL;							// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_LEG].load = 1;
	LoadModel(MODEL_PLAYER_LEFTLEG, &g_Parts[LEFT_LEG].model);

	g_Parts[RIGHT_LEG].use = TRUE;
	g_Parts[RIGHT_LEG].parent = &g_Parts[RIGHT_THIGH];			// 親をセット
	g_Parts[RIGHT_LEG].pos = XMFLOAT3(-0.5f, -3.0f, 0.0f);		// 親をセット
	g_Parts[RIGHT_LEG].tbl_adr = NULL;							// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_LEG].tbl_size = NULL;							// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_LEG].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTLEG, &g_Parts[RIGHT_LEG].model);

	g_Parts[LEFT_KNEE].use = TRUE;
	g_Parts[LEFT_KNEE].parent = &g_Parts[LEFT_LEG];										// 親をセット
	g_Parts[LEFT_KNEE].pos = XMFLOAT3(-0.5f, -3.5f, 0.5f);								// 親をセット
	g_Parts[LEFT_KNEE].tbl_adr = move_tbl_footL;										// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_KNEE].tbl_size = sizeof(move_tbl_footL) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_KNEE].load = 1;
	LoadModel(MODEL_PLAYER_KNEE, &g_Parts[LEFT_KNEE].model);

	g_Parts[RIGHT_KNEE].use = TRUE;
	g_Parts[RIGHT_KNEE].parent = &g_Parts[RIGHT_LEG];										// 親をセット
	g_Parts[RIGHT_KNEE].pos = XMFLOAT3(0.5f, -3.5f, 0.5f);									// 親をセット
	g_Parts[RIGHT_KNEE].tbl_adr = move_tbl_footR;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_KNEE].tbl_size = sizeof(move_tbl_footR) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_KNEE].load = 1;
	LoadModel(MODEL_PLAYER_KNEE, &g_Parts[RIGHT_KNEE].model);

	g_Parts[LEFT_FOOT].use = TRUE;
	g_Parts[LEFT_FOOT].parent = &g_Parts[LEFT_KNEE];		// 親をセット
	g_Parts[LEFT_FOOT].pos = XMFLOAT3(0.0f, -3.5f, 0.0f);	// 親をセット
	g_Parts[LEFT_FOOT].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[LEFT_FOOT].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[LEFT_FOOT].load = 1;
	LoadModel(MODEL_PLAYER_LEFTFOOT, &g_Parts[LEFT_FOOT].model);

	g_Parts[RIGHT_FOOT].use = TRUE;
	g_Parts[RIGHT_FOOT].parent = &g_Parts[RIGHT_KNEE];		// 親をセット
	g_Parts[RIGHT_FOOT].pos = XMFLOAT3(0.0f, -3.5f, 0.0f);	// 親をセット
	g_Parts[RIGHT_FOOT].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[RIGHT_FOOT].tbl_size = NULL;					// 再生するアニメデータのレコード数をセット
	g_Parts[RIGHT_FOOT].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTFOOT, &g_Parts[RIGHT_FOOT].model);

	g_Parts[SWORD].use = TRUE;
	g_Parts[SWORD].parent = &g_Parts[BODY];												// 親をセット
	g_Parts[SWORD].pos = XMFLOAT3(0.0f, -1.0f, -10.0f);
	g_Parts[SWORD].rot = XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f);
	g_Parts[SWORD].tbl_adr = idle_tbl_sword;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[SWORD].tbl_size = sizeof(idle_tbl_sword) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[SWORD].load = 1;
	LoadModel(MODEL_PLAYER_SWORD, &g_Parts[SWORD].model);

	g_Parts[GUN].use = TRUE;
	g_Parts[GUN].parent = &g_Parts[BODY];		// 親をセット
	g_Parts[GUN].pos = XMFLOAT3(0.0f, -1.0f, -2.0f);
	g_Parts[GUN].tbl_adr = NULL;				// 再生するアニメデータの先頭アドレスをセット
	g_Parts[GUN].tbl_size = NULL;				// 再生するアニメデータのレコード数をセット
	g_Parts[GUN].load = 1;
	LoadModel(MODEL_PLAYER_GUN, &g_Parts[GUN].model);

	g_Parts[GUN_RIGHT].use = FALSE;
	g_Parts[GUN_RIGHT].parent = &g_Parts[RIGHT_HAND];		// 親をセット
	g_Parts[GUN_RIGHT].pos = XMFLOAT3(0.0f, -3.0f, 1.0f);
	g_Parts[GUN_RIGHT].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
	g_Parts[GUN_RIGHT].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[GUN_RIGHT].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[GUN_RIGHT].load = 1;
	LoadModel(MODEL_PLAYER_GUNR, &g_Parts[GUN_RIGHT].model);

	g_Parts[GUN_LEFT].use = FALSE;
	g_Parts[GUN_LEFT].parent = &g_Parts[LEFT_HAND];			// 親をセット
	g_Parts[GUN_LEFT].pos = XMFLOAT3(0.0f, -3.0f, 1.0f);
	g_Parts[GUN_LEFT].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
	g_Parts[GUN_LEFT].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
	g_Parts[GUN_LEFT].tbl_size = NULL;						// 再生するアニメデータのレコード数をセット
	g_Parts[GUN_LEFT].load = 1;
	LoadModel(MODEL_PLAYER_GUNL, &g_Parts[GUN_LEFT].model);

	g_Parts[SWORD_ATTACK].use = FALSE;
	g_Parts[SWORD_ATTACK].parent = &g_Parts[RIGHT_SHOULDER];		// 親をセット
	g_Parts[SWORD_ATTACK].pos = XMFLOAT3(2.5f, 1.0f, 17.0f);
	g_Parts[SWORD_ATTACK].rot = XMFLOAT3(XM_PI / 3, 0.0f, 0.0f);
	g_Parts[SWORD_ATTACK].tbl_adr = NULL;							// 再生するアニメデータの先頭アドレスをセット
	g_Parts[SWORD_ATTACK].tbl_size = NULL;							// 再生するアニメデータのレコード数をセット
	g_Parts[SWORD_ATTACK].load = 1;
	LoadModel(MODEL_PLAYER_SWORDATTACK, &g_Parts[SWORD_ATTACK].model);

}

// フィールドに戻る処理
void BackToField(void)
{
	if (g_Player.pos.x < -(FIELD_BORDER_X))
	{
		g_Player.pos.x += 1000.0f;
	}
	else if (g_Player.pos.x > (FIELD_BORDER_X))
	{
		g_Player.pos.x -= 1000.0f;
	}
	else if (g_Player.pos.z > (FIELD_BORDER_Z))
	{
		g_Player.pos.z -= 1000.0f;
	}
	else
	{
		g_Player.pos.z += 1000.0f;
	}

	g_Player.canMove = TRUE;
}

//=============================================================================
// プレイヤー情報やパーツ情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

PLAYER *GetPlayerParts(void)
{
	return &g_Parts[SWORD_ATTACK];
}


