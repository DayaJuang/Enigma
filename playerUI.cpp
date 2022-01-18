//=============================================================================
//
// �X�R�A���� [playerUI.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "playerUI.h"
#include "input.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FRAME_WIDTH				(510.0f)	// �L�����T�C�Y
#define FRAME_HEIGHT			(50.0f)	// 
#define BAR_WIDTH				(450.0f)	// �L�����T�C�Y
#define BAR_HEIGHT				(10.0f)	// 
#define TEXTURE_MAX					(3)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/healthFrame.png",
	"data/TEXTURE/playerHP.png",
	"data/TEXTURE/playerMP.png"
};

static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

UI							g_UI[TEXTURE_MAX];					// UI

//=============================================================================
// ����������
//=============================================================================
HRESULT InitUI(void)
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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// �v���C���[UI�̏�����
		g_UI[i].use = TRUE;
		g_UI[i].width = 0.0f;
		g_UI[i].height = 0.0f;
		g_UI[i].position = { 250.0f, 50.0f, 0.0f };
	}

	g_UI[FRAME].width = FRAME_WIDTH;
	g_UI[FRAME].height = FRAME_HEIGHT;
	g_UI[FRAME].position = { 30.0f, SCREEN_HEIGHT - 70.0f, 0.0f };

	g_UI[HP_BAR].width = 500.0f;
	g_UI[HP_BAR].height = BAR_HEIGHT;
	g_UI[HP_BAR].position = { 35.0f, SCREEN_HEIGHT - 68.0f, 0.0f };

	g_UI[MP_BAR].width = BAR_WIDTH;
	g_UI[MP_BAR].height = BAR_HEIGHT;
	g_UI[MP_BAR].position = { 45.0f, SCREEN_HEIGHT - 55.0f, 0.0f };

	g_TexNo = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUI(void)
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
void UpdateUI(void)
{

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUI(void)
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
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		// �v���C���[UI�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_UI[i].position.x;			// �v���C���[UI�̕\���ʒuX
		float py = g_UI[i].position.y;			// �v���C���[UI�̕\���ʒuY
		float pw = g_UI[i].width;				// �v���C���[UI�̕\����
		float ph = g_UI[i].height;				// �v���C���[UI�̕\������

		float tw = 1.0f;			// �e�N�X�`���̕�
		float th = 1.0f;			// �e�N�X�`���̍���
		float tx = 0.0f;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}

UI * GetUI(void)
{
	return &g_UI[0];
}
