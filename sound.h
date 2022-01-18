//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,	// BGM0
	SOUND_LABEL_BGM_field,	// BGM1
	SOUND_LABEL_BGM_gameover,	// BGM2
	SOUND_LABEL_BGM_clear,	// BGM2
	SOUND_LABEL_SE_click,		// 爆発音
	SOUND_LABEL_SE_hit,		// 爆発音
	SOUND_LABEL_SE_hurt,	// 
	SOUND_LABEL_SE_swing,	// 
	SOUND_LABEL_SE_shot000,

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

