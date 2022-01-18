//=============================================================================
//
// �X�R�A���� [enemyHP.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "enemyHP.h"
#include "input.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BAR_WIDTH				(750.0f)	// �L�����T�C�Y
#define BAR_HEIGHT				(10.0f)	// 
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
	"data/TEXTURE/enemyHB.png",
};

static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

ENEMYHP							g_EnemyHP;					// �X�R�A

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemyHP(void)
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

	g_EnemyHP.use = FALSE;
	g_EnemyHP.width = BAR_WIDTH;
	g_EnemyHP.height = BAR_HEIGHT;
	g_EnemyHP.position = { SCREEN_CENTER_X, 10.0f, 0.0f };
	g_TexNo = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemyHP(void)
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
void UpdateEnemyHP(void)
{
	if (g_EnemyHP.width <= 0)
	{
		g_EnemyHP.use = FALSE;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyHP(void)
{
	if (g_EnemyHP.use)
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
		float px = g_EnemyHP.position.x;	// �G�l�~�[HP�̕\���ʒuX
		float py = g_EnemyHP.position.y;	// �G�l�~�[HP�̕\���ʒuY
		float pw = g_EnemyHP.width;			// �G�l�~�[HP�̕\����
		float ph = g_EnemyHP.height;		// �G�l�~�[HP�̕\������

		float tw = 1.0f;		// �e�N�X�`���̕�
		float th = 1.0f;		// �e�N�X�`���̍���
		float tx = 0.0f;		// �e�N�X�`���̍���X���W
		float ty = 0.0f;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}

}

// �G�l�~�[HP�̐ݒ肷��
void SetEnemyHP(int enemyHP)
{
	if (g_EnemyHP.use) return;

	g_EnemyHP.use = TRUE;
	g_EnemyHP.width = (float)enemyHP;
}

ENEMYHP * GetEnemyHP(void)
{
	return &g_EnemyHP;
}
