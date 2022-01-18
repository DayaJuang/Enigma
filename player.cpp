//=============================================================================
//
// ���f������ [player.cpp]
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
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER_BODY				"data/MODEL/body.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_SWORD				"data/MODEL/sword.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_SWORDATTACK		"data/MODEL/swordAttack.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GUN				"data/MODEL/gunBack.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GUNR				"data/MODEL/gun.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GUNL				"data/MODEL/gun.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_NECK				"data/MODEL/neck.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_SHOULDER			"data/MODEL/rigthShoulder.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_HEAD				"data/MODEL/head.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_FRONTHAIR			"data/MODEL/frontHair.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BACKHAIR			"data/MODEL/backHair.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LEFTARM			"data/MODEL/leftArm.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RIGHTARM			"data/MODEL/rightArm.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_ELBOW				"data/MODEL/elbow.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LEFTHAND			"data/MODEL/leftHand.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RIGHTHAND			"data/MODEL/rightHand.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_THIGH				"data/MODEL/thigh.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LEFTLEG			"data/MODEL/leftLeg.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RIGHTLEG			"data/MODEL/rightLeg.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_KNEE				"data/MODEL/knee.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LEFTFOOT			"data/MODEL/leftFoot.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RIGHTFOOT			"data/MODEL/rightFoot.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(10.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(36.5f)							// �v���C���[�̑��������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static PLAYER		g_Parts[MAX_PARTS];		// �v���C���[�̃p�[�c�p

static SKILL		g_Skills[MAX_SKILL];

static BOOL			g_Load = FALSE;

#pragma region INTERPOLATION DATA

//=============================================================================
// �A�C�h���A�j���[�V����
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
// �����A�j���[�V����
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
// ����A�j���[�V����
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
// �_�b�V���A�j���[�V����
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
// �U�� 1 �A�j���[�V����
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
// �e���� �A�j���[�V����
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
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;
	LoadModel(MODEL_PLAYER_BODY, &g_Player.model);

	// �v���C���[�̏�����
	g_Player.pos = { 38500.0f, 11450.0f, -89800.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 2.0f, 2.0f, 2.0f };

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	g_Player.maxHp = g_Player.hp = 500;
	g_Player.maxMp = g_Player.mp = 450;
	g_Player.bulletDmg = 20;
	g_Player.slashDmg = 50;
	g_Player.waitTime = 0;
	g_Player.isAttack = FALSE;

	g_Player.use = TRUE;

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �K�w�A�j���[�V�����p�̏���������
	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������
	g_Player.canMove = TRUE;

	// �p�[�c�̏�����
	InitParts();

	// �X�L��������
	// �_�b�V��
	g_Skills[SKILL_DASH].cooldown = 0.0f;
	g_Skills[SKILL_DASH].actionCooldown = 0.0f;
	g_Skills[SKILL_DASH].mpNeeded = 50;

	// �X���b�V��
	g_Skills[SKILL_SLASH].cooldown = 10.0f;
	g_Skills[SKILL_SLASH].actionCooldown = 0.0f;
	g_Skills[SKILL_SLASH].mpNeeded = 200;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		
		g_Player.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	TEXTBOX *textBox = GetTextBox();

	int oldAnim = g_Player.action;	// �A�j���[�V�������i�[

	// �X�L���N�[���_�E������
	for (int i = 0; i < MAX_SKILL; i++)
	{
		if (g_Skills[i].actionCooldown > 0.0f)
		{
			g_Skills[i].actionCooldown -= 0.1f;

			if (g_Skills[i].actionCooldown < 0.0f)
				g_Skills[i].actionCooldown = 0.0f;
		}
	}

	// �L�[�{�[�h�̃C���v�b�g����
	if ((GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT)) && g_Player.canMove)
	{	// ���ֈړ�
		g_Player.dir = XM_PI / 2;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}
	if ((GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT)) && g_Player.canMove)
	{	// �E�ֈړ�
		g_Player.dir = -XM_PI / 2;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}

	if ((GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP)) && g_Player.canMove)
	{	// ��ֈړ�
		g_Player.dir = XM_PI;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}
	if ((GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN)) && g_Player.canMove)
	{	// ���ֈړ�
		g_Player.dir = 0.0f;

		if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_A))
			PlayerRun();
		else
			PlayerWalk();
	}

	// �_�b�V������
	if ((GetKeyboardTrigger(DIK_D) || IsButtonPressed(0, BUTTON_B)) && g_Player.mp >= g_Skills[SKILL_DASH].mpNeeded && !textBox[FRAME_BOX].use)
	{
		g_Player.canMove = FALSE;
		g_Player.mp -= g_Skills[SKILL_DASH].mpNeeded;
		g_Player.action = DASH;
	}

	// �X���b�V������
	if ((GetKeyboardTrigger(DIK_X) || IsButtonPressed(0, BUTTON_Y)) && g_Player.mp >= g_Skills[SKILL_SLASH].mpNeeded && g_Skills[SKILL_SLASH].actionCooldown <= 0.0f && !textBox[FRAME_BOX].use)
	{
		PlaySound(SOUND_LABEL_SE_swing);

		g_Player.canMove = FALSE;
		g_Player.mp -= g_Skills[SKILL_SLASH].mpNeeded;
		g_Player.action = ATTACK;

		g_Skills[SKILL_SLASH].actionCooldown = 10.0f;
	}

	// �A�j���[�V������؂�ւ��
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

	//	// Key���͂���������ړ���������
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// ���͂̂����������փv���C���[���������Ĉړ�������
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	// �e���ˏ���
	if ((GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_X)) && !textBox[FRAME_BOX].use)
	{
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 8.0f;
		g_Player.canMove = FALSE;
		g_Player.action = SHOOT;
		SetBullet(pos, g_Player.rot);
	}

	// �t�B�[���h���痣��邩�ǂ����m�F
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

	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// �Ԃ������|���S���̖@���x�N�g���i�����j
	XMFLOAT3 hitPosition;

	hitPosition.y = g_Player.pos.y - PLAYER_OFFSET_Y;	// �O�ꂽ���p�ɏ��������Ă���
	bool ans = RayHitField(g_Player.pos, &hitPosition, &normal);
	g_Player.pos.y = hitPosition.y + PLAYER_OFFSET_Y;

	g_Player.spd *= 0.3f;

	if (g_Player.spd <= 0.1f && g_Player.canMove)
	{
		g_Player.action = IDLE;
		g_Player.canMove = TRUE;
	}

	// �v���C���[�A�N�V�������ς�����ꍇ�A�A�j���[�V�����t���[�������Z�b�g
	if (g_Player.action != oldAnim)
	{
		for (int i = 0; i < MAX_PARTS; i++)
		{
			g_Parts[i].move_time = 0.0f;
		}
	}

	// �K�w�A�j���[�V����
	for (int i = 0; i < MAX_PARTS; i++)
	{
		// �g���Ă���Ȃ珈������
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tbl_adr != NULL))
		{
			// �ړ�����
			int		index = (int)g_Parts[i].move_time;
			float	time = g_Parts[i].move_time - index;
			int		size = g_Parts[i].tbl_size;

			float dt = 1.0f / g_Parts[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
			g_Parts[i].move_time += dt;					// �A�j���[�V�����̍��v���Ԃɑ���

			// �_�b�V���p�i�t���[��3��4�̂Ƃ��ɓ����j
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

			if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
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

			// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
			XMVECTOR p1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
			XMVECTOR p0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
			XMVECTOR vec = p1 - p0;
			XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

			// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
			XMVECTOR r1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].rot);	// ���̊p�x
			XMVECTOR r0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
			XMVECTOR rot = r1 - r0;
			XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

			// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
			XMVECTOR s1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].scl);	// ����Scale
			XMVECTOR s0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].scl);	// ���݂�Scale
			XMVECTOR scl = s1 - s0;
			XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);

		}
	}

	// �v���C���[��HP��MP�o�[�̍X�V
	UI *playerUI = GetUI();

	// HP�o�[�̍X�V����
	playerUI[HP_BAR].width = (float) g_Player.hp;

	// �v���C���[��HP���}�C�i�X�ɂȂ�Ȃ��悤��
	if (playerUI[HP_BAR].width < 0.0f)
	{
		playerUI[HP_BAR].width = 0.0f;

		g_Player.use = FALSE;

		SetFade(FADE_OUT, MODE_RESULT);

	}
	else if (playerUI[HP_BAR].width > g_Player.maxHp)
	{
		// �v���C���[��HP���ő�̒l���I�[�o�[���Ȃ��悤��
		playerUI[HP_BAR].width = (float)g_Player.maxHp;
	}

	// MP�o�[�̍X�V����
	playerUI[MP_BAR].width = (float) g_Player.mp;

	// �v���C���[��MP���}�C�i�X�ɂȂ�Ȃ��悤��
	if (playerUI[MP_BAR].width < 0.0f)
	{
		playerUI[MP_BAR].width = 0.0f;
	}
	else if (playerUI[MP_BAR].width > g_Player.maxMp)
	{
		// �v���C���[��MP���ő�̒l���I�[�o�[���Ȃ��悤��
		playerUI[MP_BAR].width = (float)g_Player.maxMp;
	}

	// �v���C���[��MP���񕜏���
	if (g_Player.action != ATTACK && g_Player.action != RUN && g_Player.action != SHOOT && g_Player.action != DASH)
	{
		g_Player.waitTime += 1;

		// mp���񕜂���
		if (g_Player.waitTime >= 120 && g_Player.mp < g_Player.maxMp)
		{
			g_Player.mp += 2;
		}
	}
	else
	{
		g_Player.waitTime = 0;
	}

	//{	// �|�C���g���C�g�̃e�X�g
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
	// �p������
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;

	// �Q�̃x�N�g���̊O�ς�����ĔC�ӂ̉�]�������߂�
	g_Player.upVector = normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));

	// ���߂���]������N�H�[�^�j�I�������o��
	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	angle = asinf(len);
	quat = XMQuaternionRotationNormal(nvx, angle);

	// �O��̃N�H�[�^�j�I�����獡��̃N�H�[�^�j�I���܂ł̉�]�����炩�ɂ���
	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.05f);

	// ����̃N�H�[�^�j�I���̌��ʂ�ۑ�����
	XMStoreFloat4(&g_Player.quaternion, quat);

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	XMFLOAT3 rot = g_Player.rot;
	pos.y -= (PLAYER_OFFSET_Y - 0.05f);
	SetPositionShadow(g_Player.shadowIdx, pos);

#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	PrintDebugProc("\nPlayer HP:%d", g_Player.hp);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	if (g_Player.use)
	{
		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �N�H�[�^�j�I���𔽉f
		XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.quaternion));
		mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

		// ���f���`��
		//DrawModel(&g_Player.model);

		// �p�[�c�̊K�w�A�j���[�V����
		for (int i = 0; i < MAX_PARTS; i++)
		{
			
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
			{
				mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
				// ��
				// g_Player.mtxWorld���w���Ă���
			}

			XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

			// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
			if (g_Parts[i].use == FALSE) continue;

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// ���f���`��
			DrawModel(&g_Parts[i].model);

		}
	}
}

// �A�j���[�V������؂�ւ��
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

// ���`�ۊǂ�ݒ肷��
void SetTableAddress(int index, INTERPOLATION_DATA *animName)
{
	g_Parts[index].tbl_adr = animName;
}

// �v���C���[�ړ�����
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

// �p�[�c�̏�����
void InitParts(void)
{
	// �p�[�c�̏�����
	for (int i = 0; i < MAX_PARTS; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i].move_time = 0.0f;	// ���s���Ԃ��N���A
		g_Parts[i].tbl_size = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = 0;
	}

	g_Parts[BODY].use = TRUE;
	g_Parts[BODY].parent = &g_Player;												// �e���Z�b�g
	g_Parts[BODY].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Parts[BODY].tbl_adr = walk_tbl_body;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[BODY].tbl_size = sizeof(walk_tbl_body) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[BODY].load = 1;
	LoadModel(MODEL_PLAYER_BODY, &g_Parts[BODY].model);

	g_Parts[NECK].use = TRUE;
	g_Parts[NECK].parent = &g_Parts[BODY];											// �e���Z�b�g
	g_Parts[NECK].pos = XMFLOAT3(0.0f, 5.0f, 0.0f);
	g_Parts[NECK].tbl_adr = move_tbl_neck;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[NECK].tbl_size = sizeof(move_tbl_neck) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[NECK].load = 1;
	LoadModel(MODEL_PLAYER_NECK, &g_Parts[NECK].model);

	g_Parts[LEFT_SHOULDER].use = TRUE;
	g_Parts[LEFT_SHOULDER].parent = &g_Parts[BODY];											// �e���Z�b�g
	g_Parts[LEFT_SHOULDER].pos = XMFLOAT3(2.5f, 3.5f, -1.0f);
	g_Parts[LEFT_SHOULDER].tbl_adr = move_tbl_armL;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_SHOULDER].tbl_size = sizeof(move_tbl_armL) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_SHOULDER].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER, &g_Parts[LEFT_SHOULDER].model);

	g_Parts[RIGHT_SHOULDER].use = TRUE;
	g_Parts[RIGHT_SHOULDER].parent = &g_Parts[BODY];											// �e���Z�b�g
	g_Parts[RIGHT_SHOULDER].pos = XMFLOAT3(-2.5f, 3.5f, -1.0f);
	g_Parts[RIGHT_SHOULDER].tbl_adr = move_tbl_armR;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_SHOULDER].tbl_size = sizeof(move_tbl_armR) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_SHOULDER].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER, &g_Parts[RIGHT_SHOULDER].model);

	g_Parts[HEAD].use = TRUE;
	g_Parts[HEAD].parent = &g_Parts[NECK];				// �e���Z�b�g
	g_Parts[HEAD].pos = XMFLOAT3(0.0f, 5.3f, -2.0f);
	g_Parts[HEAD].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[HEAD].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[HEAD].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &g_Parts[HEAD].model);

	g_Parts[FRONT_HAIR].use = TRUE;
	g_Parts[FRONT_HAIR].parent = &g_Parts[HEAD];				// �e���Z�b�g
	g_Parts[FRONT_HAIR].pos = XMFLOAT3(0.0f, -3.5f, 2.0f);
	g_Parts[FRONT_HAIR].tbl_adr = NULL;							// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[FRONT_HAIR].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[FRONT_HAIR].load = 1;
	LoadModel(MODEL_PLAYER_FRONTHAIR, &g_Parts[FRONT_HAIR].model);

	g_Parts[BACK_HAIR].use = TRUE;
	g_Parts[BACK_HAIR].parent = &g_Parts[HEAD];												// �e���Z�b�g
	g_Parts[BACK_HAIR].pos = XMFLOAT3(0.0f, -7.0f, 0.3f);
	g_Parts[BACK_HAIR].tbl_adr = move_tbl_backHair;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[BACK_HAIR].tbl_size = sizeof(move_tbl_backHair) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[BACK_HAIR].load = 1;
	LoadModel(MODEL_PLAYER_BACKHAIR, &g_Parts[BACK_HAIR].model);

	g_Parts[LEFT_ARM].use = TRUE;
	g_Parts[LEFT_ARM].parent = &g_Parts[LEFT_SHOULDER];		// �e���Z�b�g
	g_Parts[LEFT_ARM].pos = XMFLOAT3(0.5f, -2.0f, 0.0f);
	g_Parts[LEFT_ARM].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_ARM].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_ARM].load = 1;
	LoadModel(MODEL_PLAYER_LEFTARM, &g_Parts[LEFT_ARM].model);

	g_Parts[RIGHT_ARM].use = TRUE;
	g_Parts[RIGHT_ARM].parent = &g_Parts[RIGHT_SHOULDER];		// �e���Z�b�g
	g_Parts[RIGHT_ARM].pos = XMFLOAT3(-0.5f, -2.0f, 0.0f);
	g_Parts[RIGHT_ARM].tbl_adr = NULL;							// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_ARM].tbl_size = NULL;							// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_ARM].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTARM, &g_Parts[RIGHT_ARM].model);

	g_Parts[LEFT_ELBOW].use = TRUE;
	g_Parts[LEFT_ELBOW].parent = &g_Parts[LEFT_ARM];										// �e���Z�b�g
	g_Parts[LEFT_ELBOW].pos = XMFLOAT3(0.3f, -2.5f, 0.0f);
	g_Parts[LEFT_ELBOW].tbl_adr = move_tbl_handL;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_ELBOW].tbl_size = sizeof(move_tbl_handL) / sizeof(INTERPOLATION_DATA);;	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_ELBOW].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW, &g_Parts[LEFT_ELBOW].model);

	g_Parts[RIGHT_ELBOW].use = TRUE;
	g_Parts[RIGHT_ELBOW].parent = &g_Parts[RIGHT_ARM];										// �e���Z�b�g
	g_Parts[RIGHT_ELBOW].pos = XMFLOAT3(-0.3f, -2.5f, 0.0f);
	g_Parts[RIGHT_ELBOW].tbl_adr = move_tbl_handR;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_ELBOW].tbl_size = sizeof(move_tbl_handR) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_ELBOW].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW, &g_Parts[RIGHT_ELBOW].model);

	g_Parts[LEFT_HAND].use = TRUE;
	g_Parts[LEFT_HAND].parent = &g_Parts[LEFT_ELBOW];		// �e���Z�b�g
	g_Parts[LEFT_HAND].pos = XMFLOAT3(0.0f, -3.0f, 0.0f);
	g_Parts[LEFT_HAND].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_HAND].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_HAND].load = 1;
	LoadModel(MODEL_PLAYER_LEFTHAND, &g_Parts[LEFT_HAND].model);

	g_Parts[RIGHT_HAND].use = TRUE;
	g_Parts[RIGHT_HAND].parent = &g_Parts[RIGHT_ELBOW];		// �e���Z�b�g
	g_Parts[RIGHT_HAND].pos = XMFLOAT3(0.0f, -3.0f, 0.0f);
	g_Parts[RIGHT_HAND].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_HAND].tbl_size = NULL;					// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_HAND].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTHAND, &g_Parts[RIGHT_HAND].model);

	g_Parts[LEFT_THIGH].use = TRUE;
	g_Parts[LEFT_THIGH].parent = &g_Parts[BODY];										// �e���Z�b�g
	g_Parts[LEFT_THIGH].pos = XMFLOAT3(1.5f, -4.5f, 0.0f);								// �e���Z�b�g
	g_Parts[LEFT_THIGH].tbl_adr = move_tbl_legL;										// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_THIGH].tbl_size = sizeof(move_tbl_legL) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_THIGH].load = 1;
	LoadModel(MODEL_PLAYER_THIGH, &g_Parts[LEFT_THIGH].model);

	g_Parts[RIGHT_THIGH].use = TRUE;
	g_Parts[RIGHT_THIGH].parent = &g_Parts[BODY];											// �e���Z�b�g
	g_Parts[RIGHT_THIGH].pos = XMFLOAT3(-1.5f, -4.5f, 0.0f);								// �e���Z�b�g
	g_Parts[RIGHT_THIGH].tbl_adr = move_tbl_legR;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_THIGH].tbl_size = sizeof(move_tbl_legR) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_THIGH].load = 1;
	LoadModel(MODEL_PLAYER_THIGH, &g_Parts[RIGHT_THIGH].model);

	g_Parts[LEFT_LEG].use = TRUE;
	g_Parts[LEFT_LEG].parent = &g_Parts[LEFT_THIGH];			// �e���Z�b�g
	g_Parts[LEFT_LEG].pos = XMFLOAT3(0.5f, -3.0f, 0.0f);		// �e���Z�b�g
	g_Parts[LEFT_LEG].tbl_adr = NULL;							// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_LEG].tbl_size = NULL;							// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_LEG].load = 1;
	LoadModel(MODEL_PLAYER_LEFTLEG, &g_Parts[LEFT_LEG].model);

	g_Parts[RIGHT_LEG].use = TRUE;
	g_Parts[RIGHT_LEG].parent = &g_Parts[RIGHT_THIGH];			// �e���Z�b�g
	g_Parts[RIGHT_LEG].pos = XMFLOAT3(-0.5f, -3.0f, 0.0f);		// �e���Z�b�g
	g_Parts[RIGHT_LEG].tbl_adr = NULL;							// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_LEG].tbl_size = NULL;							// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_LEG].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTLEG, &g_Parts[RIGHT_LEG].model);

	g_Parts[LEFT_KNEE].use = TRUE;
	g_Parts[LEFT_KNEE].parent = &g_Parts[LEFT_LEG];										// �e���Z�b�g
	g_Parts[LEFT_KNEE].pos = XMFLOAT3(-0.5f, -3.5f, 0.5f);								// �e���Z�b�g
	g_Parts[LEFT_KNEE].tbl_adr = move_tbl_footL;										// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_KNEE].tbl_size = sizeof(move_tbl_footL) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_KNEE].load = 1;
	LoadModel(MODEL_PLAYER_KNEE, &g_Parts[LEFT_KNEE].model);

	g_Parts[RIGHT_KNEE].use = TRUE;
	g_Parts[RIGHT_KNEE].parent = &g_Parts[RIGHT_LEG];										// �e���Z�b�g
	g_Parts[RIGHT_KNEE].pos = XMFLOAT3(0.5f, -3.5f, 0.5f);									// �e���Z�b�g
	g_Parts[RIGHT_KNEE].tbl_adr = move_tbl_footR;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_KNEE].tbl_size = sizeof(move_tbl_footR) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_KNEE].load = 1;
	LoadModel(MODEL_PLAYER_KNEE, &g_Parts[RIGHT_KNEE].model);

	g_Parts[LEFT_FOOT].use = TRUE;
	g_Parts[LEFT_FOOT].parent = &g_Parts[LEFT_KNEE];		// �e���Z�b�g
	g_Parts[LEFT_FOOT].pos = XMFLOAT3(0.0f, -3.5f, 0.0f);	// �e���Z�b�g
	g_Parts[LEFT_FOOT].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[LEFT_FOOT].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[LEFT_FOOT].load = 1;
	LoadModel(MODEL_PLAYER_LEFTFOOT, &g_Parts[LEFT_FOOT].model);

	g_Parts[RIGHT_FOOT].use = TRUE;
	g_Parts[RIGHT_FOOT].parent = &g_Parts[RIGHT_KNEE];		// �e���Z�b�g
	g_Parts[RIGHT_FOOT].pos = XMFLOAT3(0.0f, -3.5f, 0.0f);	// �e���Z�b�g
	g_Parts[RIGHT_FOOT].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[RIGHT_FOOT].tbl_size = NULL;					// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[RIGHT_FOOT].load = 1;
	LoadModel(MODEL_PLAYER_RIGHTFOOT, &g_Parts[RIGHT_FOOT].model);

	g_Parts[SWORD].use = TRUE;
	g_Parts[SWORD].parent = &g_Parts[BODY];												// �e���Z�b�g
	g_Parts[SWORD].pos = XMFLOAT3(0.0f, -1.0f, -10.0f);
	g_Parts[SWORD].rot = XMFLOAT3(0.0f, 0.0f, XM_PI / 8.0f);
	g_Parts[SWORD].tbl_adr = idle_tbl_sword;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[SWORD].tbl_size = sizeof(idle_tbl_sword) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[SWORD].load = 1;
	LoadModel(MODEL_PLAYER_SWORD, &g_Parts[SWORD].model);

	g_Parts[GUN].use = TRUE;
	g_Parts[GUN].parent = &g_Parts[BODY];		// �e���Z�b�g
	g_Parts[GUN].pos = XMFLOAT3(0.0f, -1.0f, -2.0f);
	g_Parts[GUN].tbl_adr = NULL;				// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[GUN].tbl_size = NULL;				// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[GUN].load = 1;
	LoadModel(MODEL_PLAYER_GUN, &g_Parts[GUN].model);

	g_Parts[GUN_RIGHT].use = FALSE;
	g_Parts[GUN_RIGHT].parent = &g_Parts[RIGHT_HAND];		// �e���Z�b�g
	g_Parts[GUN_RIGHT].pos = XMFLOAT3(0.0f, -3.0f, 1.0f);
	g_Parts[GUN_RIGHT].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
	g_Parts[GUN_RIGHT].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[GUN_RIGHT].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[GUN_RIGHT].load = 1;
	LoadModel(MODEL_PLAYER_GUNR, &g_Parts[GUN_RIGHT].model);

	g_Parts[GUN_LEFT].use = FALSE;
	g_Parts[GUN_LEFT].parent = &g_Parts[LEFT_HAND];			// �e���Z�b�g
	g_Parts[GUN_LEFT].pos = XMFLOAT3(0.0f, -3.0f, 1.0f);
	g_Parts[GUN_LEFT].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
	g_Parts[GUN_LEFT].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[GUN_LEFT].tbl_size = NULL;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[GUN_LEFT].load = 1;
	LoadModel(MODEL_PLAYER_GUNL, &g_Parts[GUN_LEFT].model);

	g_Parts[SWORD_ATTACK].use = FALSE;
	g_Parts[SWORD_ATTACK].parent = &g_Parts[RIGHT_SHOULDER];		// �e���Z�b�g
	g_Parts[SWORD_ATTACK].pos = XMFLOAT3(2.5f, 1.0f, 17.0f);
	g_Parts[SWORD_ATTACK].rot = XMFLOAT3(XM_PI / 3, 0.0f, 0.0f);
	g_Parts[SWORD_ATTACK].tbl_adr = NULL;							// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[SWORD_ATTACK].tbl_size = NULL;							// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[SWORD_ATTACK].load = 1;
	LoadModel(MODEL_PLAYER_SWORDATTACK, &g_Parts[SWORD_ATTACK].model);

}

// �t�B�[���h�ɖ߂鏈��
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
// �v���C���[����p�[�c�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

PLAYER *GetPlayerParts(void)
{
	return &g_Parts[SWORD_ATTACK];
}


