//=============================================================================
//
// �X�R�A���� [minimap.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "minimap.h"
#include "input.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(200.0f)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(200.0f)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/MINIMAP/minimap.png",
};


static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;
bool							isMiniMapMaximize = false;

MINIMAP							g_MiniMap;					// �X�R�A

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMiniMap(void)
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

	// �~�j�}�b�v�̏�����
	g_MiniMap.use = TRUE;
	g_MiniMap.width = TEXTURE_WIDTH;
	g_MiniMap.height = TEXTURE_HEIGHT;
	g_MiniMap.position   = { SCREEN_WIDTH - (TEXTURE_WIDTH*0.6),SCREEN_HEIGHT - (TEXTURE_HEIGHT*0.6), 0.0f };
	g_MiniMap.scale = { 1.0f,1.0f };
	g_TexNo = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMiniMap(void)
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
void UpdateMiniMap(void)
{
	if (GetKeyboardTrigger(DIK_B))
	{	
		if (isMiniMapMaximize)
		{
			isMiniMapMaximize = false;
			g_MiniMap.scale = { 1.0f,1.0f };
			g_MiniMap.position = { SCREEN_WIDTH - (TEXTURE_WIDTH * 0.7),SCREEN_HEIGHT - (TEXTURE_HEIGHT * 0.7), 0.0f };
		}
		else
		{
			isMiniMapMaximize = true;
			g_MiniMap.scale = { 3.5f,3.5f };
			g_MiniMap.position = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f, 0.0f };
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
void DrawMiniMap(void)
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

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = g_MiniMap.position.x;					// �~�j�}�b�v�̕\���ʒuX
	float py = g_MiniMap.position.y;					// �~�j�}�b�v�̕\���ʒuY
	float pw = g_MiniMap.width * g_MiniMap.scale.x;		// �~�j�}�b�v�̕\����
	float ph = g_MiniMap.height * g_MiniMap.scale.y;	// �~�j�}�b�v�̕\������

	float tw = 1.0f;			// �e�N�X�`���̕�
	float th = 1.0f;			// �e�N�X�`���̍���
	float tx = 0.0f;			// �e�N�X�`���̍���X���W
	float ty = 0.0f;			// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}

MINIMAP * GetMiniMap(void)
{
	return &g_MiniMap;
}
