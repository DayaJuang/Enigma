//=============================================================================
//
// スコア処理 [TextBox.h]
// Author : 
//
//=============================================================================
#pragma once

#define CURSOR_MAX_POSITION			(2)		// テクスチャの数

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct TEXTBOX
{
	XMFLOAT3	position;

	float		width;
	float		height;
	int			cursoPos;
	int			texNo;

	bool		use;
};

enum
{
	FRAME_BOX,
	CURSOR,
	TITLE_TEXT,
	YES_TEXT,
	NO_TEXT,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTextBox(void);
void UninitTextBox(void);
void UpdateTextBox(void);
void DrawTextBox(void);
void ShowTextBox(BOOL flag);

TEXTBOX *GetTextBox(void);

