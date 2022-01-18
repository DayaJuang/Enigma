//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "meshfield.h"
#include "renderer.h"
#include "collision.h"
#include "debugproc.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX		(4)				// �e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11Buffer					*g_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@
static ID3D11DeviceContext			*g_ImmediateContext = NULL;
static ID3D11VertexShader			*g_VertexShader = NULL;
static ID3D11PixelShader			*g_PixelShader = NULL;
static ID3D11InputLayout			*g_VertexLayout = NULL;



static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;				// �e�N�X�`���ԍ�

static XMFLOAT3		g_posField;								// �|���S���\���ʒu�̒��S���W
static XMFLOAT3		g_rotField;								// �|���S���̉�]�p
static XMFLOAT3		g_sclField;

static int			nNumBlockXField, nNumBlockZField;										// �u���b�N��
static int			nNumVertexField;														// �����_��	
static int			g_nNumVertexIndexField;													// ���C���f�b�N�X��
static float		g_fBlockSizeXField, g_fBlockSizeZField, g_FieldWidth, g_FieldHeight;	// �u���b�N�T�C�Y

static char* g_TextureName[] = {
	"data/TEXTURE/snow_field.jpg",
	"data/TEXTURE/rock.jpg",
	"data/TEXTURE/snow_normal.jpg",
	"data/TEXTURE/rock_normal.jpg",
};

static VERTEX_3D	*g_Vertex = NULL;

static BOOL			g_Load = FALSE;

static HEIGHTMAPINFO hmInfo;

static TEMP_VERTEX temp1;
static TEMP_VERTEX temp2;
static TEMP_VERTEX temp3;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(XMFLOAT3 pos, XMFLOAT3 rot, float sclXZ, float sclY, float fBlockSizeX, float fBlockSizeZ)
{
	// �t�B�[���h�f�[�^��ǂݍ���
	LoadRawHeightMap("heightmap.r16", hmInfo);

	// �|���S���\���ʒu�̒��S���W��ݒ�
	g_posField = pos;

	g_rotField = rot;

	g_sclField.y = sclY;

	g_sclField.x = g_sclField.z = sclXZ;

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// �u���b�N���̐ݒ�
	nNumBlockXField = hmInfo.terrainWidth - 1;
	nNumBlockZField = hmInfo.terrainHeight - 1;

	g_FieldHeight = (float)hmInfo.terrainHeight;
	g_FieldWidth = (float)hmInfo.terrainWidth;

	// ���_���̐ݒ�
	nNumVertexField = (hmInfo.terrainWidth) * (hmInfo.terrainHeight);

	// �C���f�b�N�X���̐ݒ�
	g_nNumVertexIndexField = (nNumBlockXField + 1) * 2 * nNumBlockZField + (nNumBlockZField - 1) * 2;

	// �u���b�N�T�C�Y�̐ݒ�
	g_fBlockSizeXField = fBlockSizeX * sclXZ;
	g_fBlockSizeZField = fBlockSizeZ * sclXZ;

	// ���_�����������ɍ���Ă���
	g_Vertex = new VERTEX_3D[nNumVertexField];

	for (int z = 0; z < (nNumBlockZField + 1); z++)
	{
		for (int x = 0; x < (nNumBlockXField + 1); x++)
		{
			// ���_���W�̐ݒ�
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.x = -(nNumBlockXField / 2.0f) * g_fBlockSizeXField + x * g_fBlockSizeXField;
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.z = (nNumBlockZField / 2.0f) * g_fBlockSizeZField - z * g_fBlockSizeZField;
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.y = hmInfo.heightMap[z * (nNumBlockXField + 1) + x].y * sclY;

			// ���ˌ��̐ݒ�
			g_Vertex[z * (nNumBlockXField + 1) + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			float texSizeX = 1.0f;
			float texSizeZ = 1.0f;
			g_Vertex[z * (nNumBlockXField + 1) + x].TexCoord.x = texSizeX * x;
			g_Vertex[z * (nNumBlockXField + 1) + x].TexCoord.y = texSizeZ * z;

			hmInfo.heightMap[z * (nNumBlockXField + 1) + x].x = -(nNumBlockXField / 2.0f) * g_fBlockSizeXField + x * g_fBlockSizeXField;
			hmInfo.heightMap[z * (nNumBlockXField + 1) + x].z = (nNumBlockZField / 2.0f) * g_fBlockSizeZField - z * g_fBlockSizeZField;

		}

	}

	// �t�B�[���h�̖@�����v�Z���܂�
	CalculateFieldNormal(hmInfo);

	//CalculateTerrainVector();

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * nNumVertexField;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned long) * g_nNumVertexIndexField;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*nNumVertexField);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned long *pIdx = (unsigned long*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (nNumBlockXField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (nNumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (nNumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (nNumBlockXField + 1) + nNumBlockXField;
				nCntIdx++;
			}

		}

		GetDeviceContext()->Unmap(g_IndexBuffer, 0);
	}

	delete[] hmInfo.heightMap;
	hmInfo.heightMap = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if (g_Load == FALSE) return;

	// �C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	if (g_Vertex)
	{
		delete[] g_Vertex;
		g_Vertex = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("bitmap Color: R:%f G:%f B:%f\n", g_Vertex[1].Binormal.y, g_Vertex[100].Tangent.x, g_Vertex[10].Tangent.y);
#endif

	return;	// �������X�L�b�v�I

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	// �S���_���𖈉�㏑�����Ă���̂�DX11�ł͂��̕�����������ł�
	memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*nNumVertexField);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{

	SetNormalMapping(1);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//SetTerrainShader();

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
	GetDeviceContext()->PSSetShaderResources(1, 1, &g_Texture[1]);
	GetDeviceContext()->PSSetShaderResources(2, 1, &g_Texture[2]);
	GetDeviceContext()->PSSetShaderResources(3, 1, &g_Texture[3]);

	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_rotField.x, g_rotField.y, g_rotField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_posField.x, g_posField.y, g_posField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	// �|���S���̕`��
	GetDeviceContext()->DrawIndexed(g_nNumVertexIndexField, 0, 0);

	SetNormalMapping(0);
}

BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3 *HitPosition, XMFLOAT3 *Normal)
{
	XMFLOAT3 start = pos;
	XMFLOAT3 end = pos;
	XMFLOAT3 org = *HitPosition;

	// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
	start.y += 1000.0f;
	end.y -= 1000.0f;

	// ����������������ׂɑS�����ł͂Ȃ��āA���W����|���S��������o����
	float fz = (nNumBlockXField / 2.0f) * g_fBlockSizeXField;
	float fx = (nNumBlockZField / 2.0f) * g_fBlockSizeZField;
	int sz = (int)((-start.z+fz) / g_fBlockSizeZField);
	int sx = (int)(( start.x+fx) / g_fBlockSizeXField);
	int ez = sz + 1;
	int ex = sx + 1;

	if ((sz < 0) || (sz > nNumBlockZField - 1) ||
		(sx < 0) || (sx > nNumBlockXField - 1))
	{
		*Normal = {0.0f, 1.0f, 0.0f};
		return FALSE;
	}

	// �K�v�����������J��Ԃ�
	for (int z = sz; z < ez; z++)
	{
		for (int x = sx; x < ex; x++)
		{
			XMFLOAT3 p0 = g_Vertex[z * (nNumBlockXField + 1) + x].Position;
			XMFLOAT3 p1 = g_Vertex[z * (nNumBlockXField + 1) + (x + 1)].Position;
			XMFLOAT3 p2 = g_Vertex[(z + 1) * (nNumBlockXField + 1) + x].Position;
			XMFLOAT3 p3 = g_Vertex[(z + 1) * (nNumBlockXField + 1) + (x + 1)].Position;

			// �O�p�|���S��������Q�����̓����蔻��
			BOOL ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
			if (ans)
			{
				return TRUE;
			}

			ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
			if (ans)
			{
				return TRUE;
			}
		}
	}

	// �����ɂ��������Ă��Ȃ�������
	*HitPosition = org;
	return FALSE;
}

// �t�B�[���h�̕��ϖ@���̌v�Z
void CalculateFieldNormal(HEIGHTMAPINFO &hmInfo)
{
	int index1 = 0;
	int index2 = 0; 
	int index3 = 0; 

	int index = 0;

	float vertex1[3] = { 0.0f,0.0f,0.0f };
	float vertex2[3] = { 0.0f,0.0f,0.0f };
	float vertex3[3] = { 0.0f,0.0f,0.0f };

	float vector1[3] = { 0.0f,0.0f,0.0f };
	float vector2[3] = { 0.0f,0.0f,0.0f };

	float len = 0.0f;
	int size = (hmInfo.terrainWidth)*(hmInfo.terrainHeight);

	XMFLOAT3 sum = {0.0f,0.0f,0.0f};
	XMFLOAT3* normals;	// �@�����i�[����

	normals = new XMFLOAT3[size];

	for (int j = 0; j < (hmInfo.terrainHeight - 1); j++)
	{
		for (int i = 0; i < (hmInfo.terrainHeight - 1); i++)
		{
			index1 = ((j + 1) * hmInfo.terrainWidth) + i;      // �������_
			index2 = ((j + 1) * hmInfo.terrainWidth) + (i + 1);  // �E�����_
			index3 = (j * hmInfo.terrainWidth) + i;          // ���㒸�_

			// ���_���K��
			vertex1[0] = hmInfo.heightMap[index1].x;
			vertex1[1] = hmInfo.heightMap[index1].y;
			vertex1[2] = hmInfo.heightMap[index1].z;

			vertex2[0] = hmInfo.heightMap[index2].x;
			vertex2[1] = hmInfo.heightMap[index2].y;
			vertex2[2] = hmInfo.heightMap[index2].z;

			vertex3[0] = hmInfo.heightMap[index3].x;
			vertex3[1] = hmInfo.heightMap[index3].y;
			vertex3[2] = hmInfo.heightMap[index3].z;

			// �x�N�g���̌v�Z
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (hmInfo.terrainWidth - 1)) + i;

			// �x�N�g���̊O�όv�Z���s���iUn-normalize���l���K���j
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

			// �x�N�g���̒���
			len = (float)sqrt((normals[index].x * normals[index].x) + (normals[index].y * normals[index].y) +
				(normals[index].z * normals[index].z));

			// �l���m�[�}���C�Y���܂�
			normals[index].x = (normals[index].x / len);
			normals[index].y = (normals[index].y / len);
			normals[index].z = (normals[index].z / len);

		}
	}

	// �e�ʂ̖@���a���v�Z
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			// ������
			sum.x = 0.0f;
			sum.y = 0.0f;
			sum.z = 0.0f;

			// ������
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (hmInfo.terrainWidth - 1)) + (i - 1);

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// �E����
			if ((i < (hmInfo.terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (hmInfo.terrainWidth - 1)) + i;

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// �����
			if (((i - 1) >= 0) && (j < (hmInfo.terrainHeight - 1)))
			{
				index = (j * (hmInfo.terrainWidth - 1)) + (i - 1);

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// �E���
			if ((i < (hmInfo.terrainWidth - 1)) && (j < (hmInfo.terrainHeight - 1)))
			{
				index = (j * (hmInfo.terrainWidth - 1)) + i;

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// �������v�Z����
			len = (float)sqrt((sum.x * sum.x) + (sum.y * sum.y) + (sum.z * sum.z));

			index = (j * hmInfo.terrainWidth) + i;

			// �@���̐ݒ�
			g_Vertex[index].Normal.x = sum.x / len;
			g_Vertex[index].Normal.y = sum.y / len;
			g_Vertex[index].Normal.z = sum.z / len;

		}
	}

	delete[] normals;
	normals = 0;
}

// Tangent��Binormal���v�Z����֐�
void CalculateTerrainVector(void)
{

	XMFLOAT3 Tangent = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Binormal = { 0.0f,0.0f,0.0f };

	int faceCount = (hmInfo.terrainWidth) * (hmInfo.terrainHeight) / 3;	// �O�p�`

	int index = 0;

	// ���_�f�[�^��Temp�֐��ɃR�s�[
	for (int i = 0; i < faceCount; i++)
	{
		// ���_1�Ԗ�
		temp1.x = g_Vertex[index].Position.x;
		temp1.y = g_Vertex[index].Position.y;
		temp1.z = g_Vertex[index].Position.z;
		temp1.tu = g_Vertex[index].TexCoord.x;
		temp1.tv = g_Vertex[index].TexCoord.y;
		index++;

		// ���_�Q�Ԗ�
		temp2.x = g_Vertex[index].Position.x;
		temp2.y = g_Vertex[index].Position.y;
		temp2.z = g_Vertex[index].Position.z;
		temp2.tu = g_Vertex[index].TexCoord.x;
		temp2.tv = g_Vertex[index].TexCoord.y;
		index++;

		// ���_3�Ԗ�
		temp3.x = g_Vertex[index].Position.x;
		temp3.y = g_Vertex[index].Position.y;
		temp3.z = g_Vertex[index].Position.z;
		temp3.tu = g_Vertex[index].TexCoord.x;
		temp3.tv = g_Vertex[index].TexCoord.y;
		index++;

		/**********************************
		  TANGENT�ANORMAL�ABINORMAL�̌v�Z
		**********************************/

		float vector1[3] = { 0.0f,0.0f,0.0f };
		float vector2[3] = { 0.0f,0.0f,0.0f };

		float tuVector[2] = { 0.0f,0.0f };
		float tvVector[2] = { 0.0f,0.0f };

		float den = 0.0f;
		float length = 0.0f;

		// �x�N�g���̌v�Z
		vector1[0] = temp2.x - temp1.x;
		vector1[1] = temp2.y - temp1.y;
		vector1[2] = temp2.z - temp1.z;

		vector2[0] = temp3.x - temp1.x;
		vector2[1] = temp3.y - temp1.y;
		vector2[2] = temp3.z - temp1.z;

		// tu��tv�̃x�N�g���v�Z
		tuVector[0] = temp2.tu - temp1.tu;
		tvVector[0] = temp2.tv - temp1.tv;

		tuVector[1] = temp3.tu - temp1.tu;
		tvVector[1] = temp3.tv - temp1.tv;

		// denominator�̌v�Z
		den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

		// Tangent��Binormal���K�����邽�߁A�O�όv�Z
		Tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		Tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		Tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		Binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		Binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		Binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Tangent�̒������v�Z
		length = (float)sqrt((Tangent.x * Tangent.x) + (Tangent.y * Tangent.y) + (Tangent.z * Tangent.z));

		// Tangent�̒����̃m�[�}���C�Y
		Tangent.x = Tangent.x / length;
		Tangent.y = Tangent.y / length;
		Tangent.z = Tangent.z / length;

		// Binormal�̒����̌v�Z
		length = (float)sqrt((Binormal.x * Binormal.x) + (Binormal.y * Binormal.y) + (Binormal.z * Binormal.z));

		// Binormal�̒����̃m�[�}���C�Y
		Binormal.x = Binormal.x / length;
		Binormal.y = Binormal.y / length;
		Binormal.z = Binormal.z / length;

		// Tangent�ABinormal�A�@����ݒ肷��
		g_Vertex[index - 1].Tangent.x = Tangent.x;
		g_Vertex[index - 1].Tangent.y = Tangent.y;
		g_Vertex[index - 1].Tangent.z = Tangent.z;
		g_Vertex[index - 1].Binormal.x = Binormal.x;
		g_Vertex[index - 1].Binormal.y = Binormal.y;
		g_Vertex[index - 1].Binormal.z = Binormal.z;

		g_Vertex[index - 2].Tangent.x = Tangent.x;
		g_Vertex[index - 2].Tangent.y = Tangent.y;
		g_Vertex[index - 2].Tangent.z = Tangent.z;
		g_Vertex[index - 2].Binormal.x = Binormal.x;
		g_Vertex[index - 2].Binormal.y = Binormal.y;
		g_Vertex[index - 2].Binormal.z = Binormal.z;

		g_Vertex[index - 3].Tangent.x = Tangent.x;
		g_Vertex[index - 3].Tangent.y = Tangent.y;
		g_Vertex[index - 3].Tangent.z = Tangent.z;
		g_Vertex[index - 3].Binormal.x = Binormal.x;
		g_Vertex[index - 3].Binormal.y = Binormal.y;
		g_Vertex[index - 3].Binormal.z = Binormal.z;

	}
}

float GetMapWidth(void)
{
	return g_FieldWidth * g_sclField.x;
}

float GetMapHeight(void)
{
	return g_FieldHeight * g_sclField.z;
}