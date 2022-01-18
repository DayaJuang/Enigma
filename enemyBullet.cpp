//=============================================================================
//
// �G�l�~�[�o���b�g���� [EnemyBullet.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "enemyBullet.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)				// �e�N�X�`���̐�

#define	EnemyBullet_SIZE_X		(75.0f)	// ���_�T�C�Y
#define	EnemyBullet_SIZE_Y		(75.0f)	// ���_�T�C�Y
#define	VALUE_MOVE_EnemyBullet	(5.0f)		// �ړ����x

#define	DISP_SHADOW							// �e�̕\��
//#undef DISP_SHADOW


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemyBullet(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;		// ���_�o�b�t�@

static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;					// �e�N�X�`���ԍ�

static ENEMYBULLET				g_EBullet[MAX_EBULLET];		// �G�l�~�[�o���b�g���[�N
static XMFLOAT3					playerPos;						// �r���{�[�h�����ʒu
static float					g_roty = 0.0f;					// �ړ�����
static float					g_spd = 0.0f;					// �ړ��X�s�[�h

static char *g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/Circ_Deg13.png",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemyBullet(void)
{
	// ���_���̍쐬
	MakeVertexEnemyBullet();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// �G�l�~�[�o���b�g���[�N�̏�����
	for(int nCntEnemyBullet = 0; nCntEnemyBullet < MAX_EBULLET; nCntEnemyBullet++)
	{
		g_EBullet[nCntEnemyBullet].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_EBullet[nCntEnemyBullet].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_EBullet[nCntEnemyBullet].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_EBullet[nCntEnemyBullet].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_EBullet[nCntEnemyBullet].material, sizeof(g_EBullet[nCntEnemyBullet].material));
		g_EBullet[nCntEnemyBullet].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_EBullet[nCntEnemyBullet].fSizeX = EnemyBullet_SIZE_X;
		g_EBullet[nCntEnemyBullet].fSizeY = EnemyBullet_SIZE_Y;
		g_EBullet[nCntEnemyBullet].nIdxShadow = -1;
		g_EBullet[nCntEnemyBullet].nLife = 0;
		g_EBullet[nCntEnemyBullet].bUse = FALSE;
	}

	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemyBullet(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemyBullet(void)
{
	{
		for(int nCntEnemyBullet = 0; nCntEnemyBullet < MAX_EBULLET; nCntEnemyBullet++)
		{
			if(g_EBullet[nCntEnemyBullet].bUse)
			{// �g�p��
				g_EBullet[nCntEnemyBullet].pos.x += g_EBullet[nCntEnemyBullet].move.x;
				g_EBullet[nCntEnemyBullet].pos.z += g_EBullet[nCntEnemyBullet].move.z;

				g_EBullet[nCntEnemyBullet].move.x += (0.0f - g_EBullet[nCntEnemyBullet].move.x) * 0.015f;
				g_EBullet[nCntEnemyBullet].move.z += (0.0f - g_EBullet[nCntEnemyBullet].move.z) * 0.015f;

#ifdef DISP_SHADOW
				if(g_EBullet[nCntEnemyBullet].nIdxShadow != -1)
				{// �e�g�p��
					float colA;

					// �e�̈ʒu�ݒ�
					SetPositionShadow(g_EBullet[nCntEnemyBullet].nIdxShadow, XMFLOAT3(g_EBullet[nCntEnemyBullet].pos.x, 0.1f, g_EBullet[nCntEnemyBullet].pos.z));

					// �e�̐F�̐ݒ�
					colA = g_EBullet[nCntEnemyBullet].material.Diffuse.w;
					SetColorShadow(g_EBullet[nCntEnemyBullet].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
				}
#endif

				g_EBullet[nCntEnemyBullet].nLife--;
				if(g_EBullet[nCntEnemyBullet].nLife <= 0)
				{
					g_EBullet[nCntEnemyBullet].bUse = FALSE;
					ReleaseShadow(g_EBullet[nCntEnemyBullet].nIdxShadow);
					g_EBullet[nCntEnemyBullet].nIdxShadow = -1;
				}
			}
		}

#ifdef _DEBUG	// �f�o�b�O����\������
		PrintDebugProc("\nEnemyBullet:X:%f Y:%f Z:%f\n", g_EBullet[0].pos.x, g_EBullet[0].pos.y, g_EBullet[0].pos.z);
#endif

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyBullet(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// Z��r����
	SetDepthEnable(FALSE);

	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for(int nCntEnemyBullet = 0; nCntEnemyBullet < MAX_EBULLET; nCntEnemyBullet++)
	{
		if(g_EBullet[nCntEnemyBullet].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			mtxWorld = XMMatrixInverse(nullptr, mtxView);
			mtxWorld.r[3].m128_f32[0] = 0.0f;
			mtxWorld.r[3].m128_f32[1] = 0.0f;
			mtxWorld.r[3].m128_f32[2] = 0.0f;

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_EBullet[nCntEnemyBullet].scale.x, g_EBullet[nCntEnemyBullet].scale.y, g_EBullet[nCntEnemyBullet].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_EBullet[nCntEnemyBullet].pos.x, g_EBullet[nCntEnemyBullet].pos.y, g_EBullet[nCntEnemyBullet].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_EBullet[nCntEnemyBullet].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r�L��
	SetDepthEnable(TRUE);

	// �t�H�O�L��
	SetFogEnable(TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEnemyBullet(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-EnemyBullet_SIZE_X / 2, EnemyBullet_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(EnemyBullet_SIZE_X / 2, EnemyBullet_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-EnemyBullet_SIZE_X / 2, -EnemyBullet_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(EnemyBullet_SIZE_X / 2, -EnemyBullet_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorEnemyBullet(int nIdxEnemyBullet, XMFLOAT4 col)
{
	g_EBullet[nIdxEnemyBullet].material.Diffuse = col;
}

//=============================================================================
// �G�l�~�[�o���b�g�̔�������
//=============================================================================
int SetEnemyBullet(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxEnemyBullet = -1;

	for(int nCntEnemyBullet = 0; nCntEnemyBullet < MAX_EBULLET; nCntEnemyBullet++)
	{
		if(!g_EBullet[nCntEnemyBullet].bUse)
		{
			g_EBullet[nCntEnemyBullet].pos = pos;
			g_EBullet[nCntEnemyBullet].rot   = { 0.0f, 0.0f, 0.0f };
			g_EBullet[nCntEnemyBullet].scale = { 2.0f, 2.0f, 2.0f };
			g_EBullet[nCntEnemyBullet].move = move;
			g_EBullet[nCntEnemyBullet].material.Diffuse = col;
			g_EBullet[nCntEnemyBullet].fSizeX = fSizeX;
			g_EBullet[nCntEnemyBullet].fSizeY = fSizeY;
			g_EBullet[nCntEnemyBullet].nLife = nLife;
			g_EBullet[nCntEnemyBullet].bUse = TRUE;

			nIdxEnemyBullet = nCntEnemyBullet;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_EBullet[nCntEnemyBullet].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
			if(g_EBullet[nCntEnemyBullet].nIdxShadow != -1)
			{
				SetColorShadow(g_EBullet[nCntEnemyBullet].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxEnemyBullet;
}

ENEMYBULLET *GetEnemyBullet(void)
{
	return &g_EBullet[0];
}
