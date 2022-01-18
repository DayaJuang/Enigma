//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,	// BGM0
	SOUND_LABEL_BGM_field,	// BGM1
	SOUND_LABEL_BGM_gameover,	// BGM2
	SOUND_LABEL_BGM_clear,	// BGM2
	SOUND_LABEL_SE_click,		// ������
	SOUND_LABEL_SE_hit,		// ������
	SOUND_LABEL_SE_hurt,	// 
	SOUND_LABEL_SE_swing,	// 
	SOUND_LABEL_SE_shot000,

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

