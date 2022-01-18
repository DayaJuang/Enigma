//=============================================================================
//
// �X�R�A���� [TextBox.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "textBox.h"
#include "input.h"
#include "sprite.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(500.0f)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(300.0f)	// 
#define TEXTURE_MAX					(5)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/frame.png",
	"data/TEXTURE/cursor.png",
	"data/TEXTURE/title_back.png",
	"data/TEXTURE/yes_text.png",
	"data/TEXTURE/no_text.png",
};

static BOOL						g_Load = FALSE;

static TEXTBOX					g_TextBox[TEXTURE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTextBox(void)
{
	ID3D11Device *pDevice = GetDevice();

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

	// ������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_TextBox[i].use = FALSE;
		g_TextBox[i].width = TEXTURE_WIDTH;
		g_TextBox[i].height = TEXTURE_HEIGHT;
		g_TextBox[i].position = { 0.0f, 0.0f, 0.0f };
		g_TextBox[i].cursoPos = 0;
		g_TextBox[i].texNo = i;
	}

	g_TextBox[FRAME_BOX].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f };

	g_TextBox[TITLE_TEXT].width = 400.0f;
	g_TextBox[TITLE_TEXT].height = 80.0f;
	g_TextBox[TITLE_TEXT].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y - 50.0f, 0.0f };

	g_TextBox[YES_TEXT].width = 300.0f;
	g_TextBox[YES_TEXT].height = 80.0f;
	g_TextBox[YES_TEXT].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f };

	g_TextBox[NO_TEXT].width = 300.0f;
	g_TextBox[NO_TEXT].height = 80.0f;
	g_TextBox[NO_TEXT].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y + 50.0f, 0.0f };

	g_TextBox[CURSOR].width = 100.0f;
	g_TextBox[CURSOR].height = 50.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTextBox(void)
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
void UpdateTextBox(void)
{

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTextBox(void)
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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_TextBox[i].use)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TextBox[i].texNo]);

			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_TextBox[i].position.x;
			float py = g_TextBox[i].position.y;
			float pw = g_TextBox[i].width;
			float ph = g_TextBox[i].height;

			float tw = 1.0f;		// �e�N�X�`���̕�
			float th = 1.0f;		// �e�N�X�`���̍���
			float tx = 0.0f;		// �e�N�X�`���̍���X���W
			float ty = 0.0f;		// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void ShowTextBox(BOOL flag)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_TextBox[i].use = flag;
	}
}

TEXTBOX * GetTextBox(void)
{
	return &g_TextBox[0];
}
