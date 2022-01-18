//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/main_menu.png",
	"data/TEXTURE/title_logo.png",
	"data/TEXTURE/press_any_button.png",
	"data/TEXTURE/keyboard_tutorial.png",
	"data/TEXTURE/gamepad_tutorial.png",
	"data/TEXTURE/tutorial.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;					// �|���S���̍��W
static XMFLOAT3					g_PosLogo;					// �|���S���̍��W
static XMFLOAT3					g_PosPressStart;			// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

float	alpha;
BOOL	flag_alpha;

static BOOL						g_Load = FALSE;
static bool						isTutorial = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();
	isTutorial = false;

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos             = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_PosLogo         = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_PosPressStart   = XMFLOAT3(g_w/2, g_h/2 + 75.0f, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_title);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	if (isTutorial)
	{
		if (g_TexNo == 5)
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter��������A�X�e�[�W��؂�ւ���
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_click);
			}
			// �Q�[���p�b�h�œ��͏���
			else if (IsButtonTriggered(0, BUTTON_START))
			{
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_click);
			}
			else if (IsButtonTriggered(0, BUTTON_B))
			{
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_click);
			}

			return;

		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo++;
		}
		// �Q�[���p�b�h�œ��͏���
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo++;
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo++;
		}

	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo = 3;
			isTutorial = true;
		}
		// �Q�[���p�b�h�œ��͏���
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo = 3;
			isTutorial = true;
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo = 3;
			isTutorial = true;
		}
	}

	if (flag_alpha == TRUE)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			flag_alpha = FALSE;
		}
	}
	else
	{
		alpha += 0.02f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			flag_alpha = TRUE;
		}
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	if (!isTutorial)
	{
		// �^�C�g���̃��S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, g_PosLogo.x, g_PosLogo.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// �^�C�g���̃��S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, g_PosPressStart.x, g_PosPressStart.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO * 0.75f, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}




