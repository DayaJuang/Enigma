//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
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
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/enemy.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(150.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// �G�l�~�[

static BOOL				g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = TRUE;

		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(7.0f, 7.0f, 7.0f);

		g_Enemy[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy[i].tbl_size = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[i].hp = 1250;
		g_Enemy[i].dmg = 100;
		g_Enemy[i].mode = ENEMY_IDLE;

		g_Enemy[i].use = TRUE;			// TRUE:�����Ă�

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
// �I������
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
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	ENEMYHP *enemyHP = GetEnemyHP();
	ENEMYPOINT *ePoint = GetEnemyPoint();

	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
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

		if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			// ���C�L���X�g���đ����̍��������߂�
			XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// �Ԃ������|���S���̖@���x�N�g���i�����j
			XMFLOAT3 hitPosition;

			hitPosition.y = g_Enemy[i].pos.y - ENEMY_OFFSET_Y;	// �O�ꂽ���p�ɏ��������Ă���
			bool ans = RayHitField(g_Enemy[i].pos, &hitPosition, &normal);
			g_Enemy[i].pos.y = hitPosition.y + ENEMY_OFFSET_Y;

			// �e���v���C���[�̈ʒu�ɍ��킹��
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
				float g_fWidthBase = 50.0f;				// ��̕�
				float g_fHeightBase = 100.0f;			// ��̍���


				pos = { g_Enemy[i].pos.x,g_Enemy[i].pos.y - 50.0f, g_Enemy[i].pos.z };

				fAngle = (float)(rand() % 628 - 314) / 100.0f;
				fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
				move.x = sinf(fAngle) * fLength;
				move.y = 0.0f;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 100 + 150;

				fSize = (float)(rand() % 30 + 20);

				// �r���{�[�h�̐ݒ�
				SetEnemyBullet(pos, move, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife);

			}

			//////////////////////////////////////////////////////////////////////
			// �p������
			//////////////////////////////////////////////////////////////////////

			XMVECTOR vx, nvx, up;
			XMVECTOR quat;
			float len, angle;

			// �Q�̃x�N�g���̊O�ς�����ĔC�ӂ̉�]�������߂�
			g_Enemy[i].upVector = normal;
			up = { 0.0f, 1.0f, 0.0f, 0.0f };
			vx = XMVector3Cross(up, XMLoadFloat3(&g_Enemy[i].upVector));

			// ���߂���]������N�H�[�^�j�I�������o��
			nvx = XMVector3Length(vx);
			XMStoreFloat(&len, nvx);
			nvx = XMVector3Normalize(vx);
			angle = asinf(len);
			quat = XMQuaternionRotationNormal(nvx, angle);

			// �O��̃N�H�[�^�j�I�����獡��̃N�H�[�^�j�I���܂ł̉�]�����炩�ɂ���
			quat = XMQuaternionSlerp(XMLoadFloat4(&g_Enemy[i].quaternion), quat, 0.05f);

			// ����̃N�H�[�^�j�I���̌��ʂ�ۑ�����
			XMStoreFloat4(&g_Enemy[i].quaternion, quat);

		}
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("\nEnemy:X:%f Y:%f HP:%d\n", g_Enemy[2].pos.x, g_Enemy[2].pos.y, g_Enemy[0].hp);
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_BACK);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		if (g_Enemy[i].mode == ENEMY_BATTLE)
		{
			SetFuchi(1);
		}

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �N�H�[�^�j�I���𔽉f
		XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Enemy[i].quaternion));
		mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Enemy[i].model);

		SetFuchi(0);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
