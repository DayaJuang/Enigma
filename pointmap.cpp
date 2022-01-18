//=============================================================================
//
// �X�R�A���� [pointMap.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "minimap.h"
#include "pointmap.h"
#include "meshfield.h"
#include "sprite.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(3.0f)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(3.0f)	// 
#define BORDER_SIZE					(10.0f)	// �L�����T�C�Y
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
	"data/MINIMAP/point.jpg",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPoint(void)
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


	// �|�C���g�̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { 0.0f , 0.0f , 0.0f };
	g_TexNo = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPoint(void)
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
void UpdatePoint(float posX, float posY)
{
	MINIMAP *miniMap = GetMiniMap();

	// �~�j�}�b�v�̋��E��ݒ肷��
	float xMinBound = miniMap->position.x - (miniMap->width * miniMap->scale.x - BORDER_SIZE) * 0.5f;
	float xMaxBound = miniMap->position.x + (miniMap->width * miniMap->scale.x - BORDER_SIZE) * 0.5f;

	float yMinBound = miniMap->position.y - (miniMap->height* miniMap->scale.y  - BORDER_SIZE) * 0.5f;
	float yMaxBound = miniMap->position.y + (miniMap->height* miniMap->scale.y  - BORDER_SIZE) * 0.5f;

	float percentX = posX / GetMapWidth() * 0.1f * miniMap->scale.x;
	float percentY = -posY / GetMapHeight() * 0.1f * miniMap->scale.y;

	g_Pos.x = miniMap->position.x + miniMap->width * percentX;
	g_Pos.y = miniMap->position.y + miniMap->height * percentY;

	// �|�C���g���~�j�}�b�v���痣��Ȃ��悤��
	if (g_Pos.x < xMinBound)
	{
		g_Pos.x = xMinBound;
	}

	if (g_Pos.x > xMaxBound)
	{
		g_Pos.x = xMaxBound;
	}

	if (g_Pos.y < yMinBound)
	{
		g_Pos.y = yMinBound;
	}

	if (g_Pos.y > yMaxBound)
	{
		g_Pos.y = yMaxBound;
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("point Position: X:%f Y:%f \n", percentX,percentY);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPoint(void)
{
	MINIMAP *miniMap = GetMiniMap();

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

	float px = g_Pos.x;								// �|�C���g�̕\���ʒuX
	float py = g_Pos.y;								// �|�C���g�̕\���ʒuY
	float pw = g_w * miniMap->scale.x;				// �|�C���g�̕\����
	float ph = g_h * miniMap->scale.y;				// �|�C���g�̕\������

	float tw = 1.0f;			// �e�N�X�`���̕�
	float th = 1.0f;			// �e�N�X�`���̍���
	float tx = 0.0f;			// �e�N�X�`���̍���X���W
	float ty = 0.0f;			// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}