//=============================================================================
//
// �X�R�A���� [TextBox.h]
// Author : 
//
//=============================================================================
#pragma once

#define CURSOR_MAX_POSITION			(2)		// �e�N�X�`���̐�

//*****************************************************************************
// �}�N����`
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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTextBox(void);
void UninitTextBox(void);
void UpdateTextBox(void);
void DrawTextBox(void);
void ShowTextBox(BOOL flag);

TEXTBOX *GetTextBox(void);

