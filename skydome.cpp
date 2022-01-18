//=============================================================================
//
// ���f������ [Skydome.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "skydome.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "meshfield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_SKYDOME		"data/MODEL/skydome.obj"			// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static SKYDOME		skydome;						// �v���C���[

static BOOL			g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkydome(void)
{
	LoadModel(MODEL_SKYDOME, &skydome.model);
	skydome.load = TRUE;
	
	skydome.pos = { 0.0f, 0.0f, 0.0f };
	skydome.rot = { 0.0f, 0.0f, 0.0f };
	skydome.scl = { 800.0f, 800.0f, 800.0f };
	
	skydome.use = TRUE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSkydome(void)
{
	if (g_Load == FALSE) return;

	// ���f���̉������
	if (skydome.load)
	{
		UnloadModel(&skydome.model);
		skydome.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSkydome(void)
{
	skydome.rot.y += 0.0001f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkydome(void)
{
	
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(skydome.scl.x, skydome.scl.y, skydome.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(skydome.rot.x, skydome.rot.y, skydome.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(skydome.pos.x, skydome.pos.y, skydome.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&skydome.mtxWorld, mtxWorld);


	// ���f���`��
	DrawModel(&skydome.model);

	SetLightEnable(TRUE);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
SKYDOME *GetSkydome(void)
{
	return &skydome;
}

