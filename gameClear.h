//=============================================================================
//
// �Q�[���N���A��ʏ��� [GameClear.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct GAMECLEAR
{
	XMFLOAT3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameClear(void);
void UninitGameClear(void);
void UpdateGameClear(void);
void DrawGameClear(void);

