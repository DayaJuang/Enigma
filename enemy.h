//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY		(5)			// �G�l�~�[�̐�

#define	ENEMY_SIZE		(200.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
{
	INTERPOLATION_DATA	*tbl_adr;			// �A�j���f�[�^�̃e�[�u���擪�A�h���X

	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X

	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	// �N�H�[�^�j�I��
	XMFLOAT4			quaternion;	// �N�H�[�^�j�I��
	XMFLOAT3			upVector;	// �����������Ă��鏊

	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	DX11_MODEL			model;				// ���f�����

	BOOL				use;
	BOOL				load;

	float				move_time;			// ���s����
	float				spd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�
	int					hp;
	int					dmg;
	int					mode;
	int					tbl_size;			// �o�^�����e�[�u���̃��R�[�h����

};

enum
{
	ENEMY_IDLE,
	ENEMY_BATTLE
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemyHP(int enemyHP);

ENEMY *GetEnemy(void);

