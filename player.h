//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�
#define MAX_SKILL		(2)					// �v���C���[�̐�

#define	PLAYER_SIZE		(100.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adr;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X

	XMFLOAT4X4			mtxWorld;	// ���[���h�}�g���b�N�X
	
	XMFLOAT4			quaternion;	// �N�H�[�^�j�I��
	XMFLOAT3			upVector;	// �����������Ă��鏊

	XMFLOAT3			pos;		// �|���S���̈ʒu
	XMFLOAT3			rot;		// �|���S���̌���(��])
	XMFLOAT3			scl;		// �|���S���̑傫��(�X�P�[��)

	DX11_MODEL			model;		// ���f�����

	float				spd;		// �ړ��X�s�[�h
	float				dir;		// ����
	float				size;		// �����蔻��̑傫��
	float				move_time;	// ���s����

	int					shadowIdx;	// �e��Index
	int					tbl_size;	// �o�^�����e�[�u���̃��R�[�h����
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

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X
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
// �v���g�^�C�v�錾
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

