//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
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
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 20.0f, 2.0f, 10.0f, 10.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// ��̏�����
	InitSkydome();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �e�̏�����
	InitBullet();
	InitEnemyBullet();

	// �~�j�}�b�v�p�̏�����
	InitMiniMap();

	InitPoint();

	InitEnemyPoint();

	// �v���C���[UI�̏�����
	InitUI();

	// �G�l�~�[��HPUI�̏�����
	InitEnemyHP();

	// �p�[�e�B�N���̏�����
	InitParticle();

	InitTextBox();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_field);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �~�j�}�b�v�̏I������
	UninitMiniMap();

	UninitPoint();

	UninitEnemyPoint();

	// �v���C���[UI�̏I������
	UninitUI();

	// �e�̏I������
	UninitBullet();
	UninitEnemyBullet();

	// �n�ʂ̏I������
	UninitMeshField();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �G�l�~�[HP�̏I������
	UninitEnemyHP();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

	// ��̏I������
	UninitSkydome();

	UninitTextBox();

}

//=============================================================================
// �X�V����
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

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// ��̍X�V
	UpdateSkydome();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// �G�l�~�[HP�̍X�V����
	UpdateEnemyHP();

	// �e�̍X�V����
	UpdateBullet();
	UpdateEnemyBullet();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if(enemy[i].use)
			UpdateEnemyPoint(enemy[i].pos.x, enemy[i].pos.z,i);
	}

	// �~�j�}�b�v�̍X�V
	UpdatePoint(player->pos.x,player->pos.z);

	UpdateMiniMap();

	// �v���C���[UI�̍X�V
	UpdateUI();

	UpdateTextBox();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��
	DrawMeshField();

	DrawSkydome();

	// �e�̕`�揈��
	DrawShadow();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawBullet();
	DrawEnemyBullet();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �~�j�}�b�v�̕`��
	DrawMiniMap();

	DrawPoint();

	DrawEnemyPoint();

	// �v���C���[UI�̕`��
	DrawUI();

	DrawTextBox();

	// �G�l�~�[HP�̕`�揈��
	DrawEnemyHP();

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayer()->pos;
	//pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
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

		// �G�l�~�[���_
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

		// �G�l�~�[���_
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
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer();	// �v���C���[�̃|�C���^�[��������
	BULLET *bullet = GetBullet();	// �e�̃|�C���^�[��������

	ENEMYHP *enemyHP = GetEnemyHP();
	ENEMYBULLET *enemyBullet = GetEnemyBullet();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (enemy[i].use == FALSE)
			continue;

		//BC�̓����蔻��
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size) && player->isAttack)
		{
			if (enemy[i].mode == ENEMY_IDLE)
			{
				enemy[i].mode = ENEMY_BATTLE;
			}

			int dmg = (player->slashDmg - 6) + rand() % ((player->slashDmg + 6) - (player->slashDmg - 6));

			// �G��HP�����炷
			enemy[i].hp -= dmg;

			enemyHP->width = (float)enemy[i].hp;

			PlaySound(SOUND_LABEL_SE_hit);
		}
	}

	// �v���C���[�̒e�ƓG
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bullet[i].use == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].fWidth, enemy[j].size))
			{
				if (enemy[j].mode == ENEMY_IDLE)
				{
					enemy[j].mode = ENEMY_BATTLE;
				}

				int dmg = (player->bulletDmg - 2) + rand() % ((player->bulletDmg + 2) - (player->bulletDmg - 2));

				// �����������疢�g�p�ɖ߂�
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// �G��HP�����炷
				enemy[j].hp -= dmg;

				enemyHP->width = (float)enemy[j].hp;

				PlaySound(SOUND_LABEL_SE_hit);

			}
		}

	}

	// �G�l�~�[�̒e�ƃv���C���[
	for (int i = 0; i < MAX_EBULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (enemyBullet[i].bUse == FALSE)
			continue;

		//BC�̓����蔻��
		if (CollisionBC(enemyBullet[i].pos, player->pos, enemyBullet[i].fSizeX, player->size))
		{
			int dmg = enemy[0].dmg;

			// �����������疢�g�p�ɖ߂�
			enemyBullet[i].bUse = FALSE;
			ReleaseShadow(bullet[i].shadowIdx);

			// �v���C���[��HP�����炷
			player->hp -= dmg;

			PlaySound(SOUND_LABEL_SE_hurt);
		}

	}



	// �G�l�~�[���S�����S�������ԑJ��
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].hp <= 0) continue;
		enemy_count++;
	}

	// �G�l�~�[���O�C�H
	if (enemy_count == 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

}


