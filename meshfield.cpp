//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX		(4)				// テクスチャの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11Buffer					*g_IndexBuffer = NULL;	// インデックスバッファ
static ID3D11DeviceContext			*g_ImmediateContext = NULL;
static ID3D11VertexShader			*g_VertexShader = NULL;
static ID3D11PixelShader			*g_PixelShader = NULL;
static ID3D11InputLayout			*g_VertexLayout = NULL;



static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;				// テクスチャ番号

static XMFLOAT3		g_posField;								// ポリゴン表示位置の中心座標
static XMFLOAT3		g_rotField;								// ポリゴンの回転角
static XMFLOAT3		g_sclField;

static int			nNumBlockXField, nNumBlockZField;										// ブロック数
static int			nNumVertexField;														// 総頂点数	
static int			g_nNumVertexIndexField;													// 総インデックス数
static float		g_fBlockSizeXField, g_fBlockSizeZField, g_FieldWidth, g_FieldHeight;	// ブロックサイズ

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
// 初期化処理
//=============================================================================
HRESULT InitMeshField(XMFLOAT3 pos, XMFLOAT3 rot, float sclXZ, float sclY, float fBlockSizeX, float fBlockSizeZ)
{
	// フィールドデータを読み込む
	LoadRawHeightMap("heightmap.r16", hmInfo);

	// ポリゴン表示位置の中心座標を設定
	g_posField = pos;

	g_rotField = rot;

	g_sclField.y = sclY;

	g_sclField.x = g_sclField.z = sclXZ;

	// テクスチャ生成
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

	// ブロック数の設定
	nNumBlockXField = hmInfo.terrainWidth - 1;
	nNumBlockZField = hmInfo.terrainHeight - 1;

	g_FieldHeight = (float)hmInfo.terrainHeight;
	g_FieldWidth = (float)hmInfo.terrainWidth;

	// 頂点数の設定
	nNumVertexField = (hmInfo.terrainWidth) * (hmInfo.terrainHeight);

	// インデックス数の設定
	g_nNumVertexIndexField = (nNumBlockXField + 1) * 2 * nNumBlockZField + (nNumBlockZField - 1) * 2;

	// ブロックサイズの設定
	g_fBlockSizeXField = fBlockSizeX * sclXZ;
	g_fBlockSizeZField = fBlockSizeZ * sclXZ;

	// 頂点情報をメモリに作っておく
	g_Vertex = new VERTEX_3D[nNumVertexField];

	for (int z = 0; z < (nNumBlockZField + 1); z++)
	{
		for (int x = 0; x < (nNumBlockXField + 1); x++)
		{
			// 頂点座標の設定
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.x = -(nNumBlockXField / 2.0f) * g_fBlockSizeXField + x * g_fBlockSizeXField;
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.z = (nNumBlockZField / 2.0f) * g_fBlockSizeZField - z * g_fBlockSizeZField;
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.y = hmInfo.heightMap[z * (nNumBlockXField + 1) + x].y * sclY;

			// 反射光の設定
			g_Vertex[z * (nNumBlockXField + 1) + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			float texSizeX = 1.0f;
			float texSizeZ = 1.0f;
			g_Vertex[z * (nNumBlockXField + 1) + x].TexCoord.x = texSizeX * x;
			g_Vertex[z * (nNumBlockXField + 1) + x].TexCoord.y = texSizeZ * z;

			hmInfo.heightMap[z * (nNumBlockXField + 1) + x].x = -(nNumBlockXField / 2.0f) * g_fBlockSizeXField + x * g_fBlockSizeXField;
			hmInfo.heightMap[z * (nNumBlockXField + 1) + x].z = (nNumBlockZField / 2.0f) * g_fBlockSizeZField - z * g_fBlockSizeZField;

		}

	}

	// フィールドの法線を計算します
	CalculateFieldNormal(hmInfo);

	//CalculateTerrainVector();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * nNumVertexField;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned long) * g_nNumVertexIndexField;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*nNumVertexField);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned long *pIdx = (unsigned long*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
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
			{// 縮退ポリゴンのためのダブりの設定
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
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	if (g_Load == FALSE) return;

	// インデックスバッファの解放
	if (g_IndexBuffer) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	// 頂点バッファの解放
	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// テクスチャの解放
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
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{
#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("bitmap Color: R:%f G:%f B:%f\n", g_Vertex[1].Binormal.y, g_Vertex[100].Tangent.x, g_Vertex[10].Tangent.y);
#endif

	return;	// 処理をスキップ！

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	// 全頂点情報を毎回上書きしているのはDX11ではこの方が早いからです
	memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*nNumVertexField);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{

	SetNormalMapping(1);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//SetTerrainShader();

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
	GetDeviceContext()->PSSetShaderResources(1, 1, &g_Texture[1]);
	GetDeviceContext()->PSSetShaderResources(2, 1, &g_Texture[2]);
	GetDeviceContext()->PSSetShaderResources(3, 1, &g_Texture[3]);

	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_rotField.x, g_rotField.y, g_rotField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posField.x, g_posField.y, g_posField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	// ポリゴンの描画
	GetDeviceContext()->DrawIndexed(g_nNumVertexIndexField, 0, 0);

	SetNormalMapping(0);
}

BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3 *HitPosition, XMFLOAT3 *Normal)
{
	XMFLOAT3 start = pos;
	XMFLOAT3 end = pos;
	XMFLOAT3 org = *HitPosition;

	// 少し上から、ズドーンと下へレイを飛ばす
	start.y += 1000.0f;
	end.y -= 1000.0f;

	// 処理を高速化する為に全検索ではなくて、座標からポリゴンを割り出すと
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

	// 必要数分検索を繰り返す
	for (int z = sz; z < ez; z++)
	{
		for (int x = sx; x < ex; x++)
		{
			XMFLOAT3 p0 = g_Vertex[z * (nNumBlockXField + 1) + x].Position;
			XMFLOAT3 p1 = g_Vertex[z * (nNumBlockXField + 1) + (x + 1)].Position;
			XMFLOAT3 p2 = g_Vertex[(z + 1) * (nNumBlockXField + 1) + x].Position;
			XMFLOAT3 p3 = g_Vertex[(z + 1) * (nNumBlockXField + 1) + (x + 1)].Position;

			// 三角ポリゴンだから２枚分の当たり判定
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

	// 何処にも当たっていなかった時
	*HitPosition = org;
	return FALSE;
}

// フィールドの平均法線の計算
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
	XMFLOAT3* normals;	// 法線を格納ため

	normals = new XMFLOAT3[size];

	for (int j = 0; j < (hmInfo.terrainHeight - 1); j++)
	{
		for (int i = 0; i < (hmInfo.terrainHeight - 1); i++)
		{
			index1 = ((j + 1) * hmInfo.terrainWidth) + i;      // 左下頂点
			index2 = ((j + 1) * hmInfo.terrainWidth) + (i + 1);  // 右下頂点
			index3 = (j * hmInfo.terrainWidth) + i;          // 左上頂点

			// 頂点を習得
			vertex1[0] = hmInfo.heightMap[index1].x;
			vertex1[1] = hmInfo.heightMap[index1].y;
			vertex1[2] = hmInfo.heightMap[index1].z;

			vertex2[0] = hmInfo.heightMap[index2].x;
			vertex2[1] = hmInfo.heightMap[index2].y;
			vertex2[2] = hmInfo.heightMap[index2].z;

			vertex3[0] = hmInfo.heightMap[index3].x;
			vertex3[1] = hmInfo.heightMap[index3].y;
			vertex3[2] = hmInfo.heightMap[index3].z;

			// ベクトルの計算
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (hmInfo.terrainWidth - 1)) + i;

			// ベクトルの外積計算を行い（Un-normalizeｄ値を習得）
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

			// ベクトルの長さ
			len = (float)sqrt((normals[index].x * normals[index].x) + (normals[index].y * normals[index].y) +
				(normals[index].z * normals[index].z));

			// 値をノーマライズします
			normals[index].x = (normals[index].x / len);
			normals[index].y = (normals[index].y / len);
			normals[index].z = (normals[index].z / len);

		}
	}

	// 各面の法線和を計算
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			// 初期化
			sum.x = 0.0f;
			sum.y = 0.0f;
			sum.z = 0.0f;

			// 左下面
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (hmInfo.terrainWidth - 1)) + (i - 1);

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// 右下面
			if ((i < (hmInfo.terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (hmInfo.terrainWidth - 1)) + i;

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// 左上面
			if (((i - 1) >= 0) && (j < (hmInfo.terrainHeight - 1)))
			{
				index = (j * (hmInfo.terrainWidth - 1)) + (i - 1);

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// 右上面
			if ((i < (hmInfo.terrainWidth - 1)) && (j < (hmInfo.terrainHeight - 1)))
			{
				index = (j * (hmInfo.terrainWidth - 1)) + i;

				sum.x += normals[index].x;
				sum.y += normals[index].y;
				sum.z += normals[index].z;
			}

			// 長さを計算する
			len = (float)sqrt((sum.x * sum.x) + (sum.y * sum.y) + (sum.z * sum.z));

			index = (j * hmInfo.terrainWidth) + i;

			// 法線の設定
			g_Vertex[index].Normal.x = sum.x / len;
			g_Vertex[index].Normal.y = sum.y / len;
			g_Vertex[index].Normal.z = sum.z / len;

		}
	}

	delete[] normals;
	normals = 0;
}

// TangentとBinormalを計算する関数
void CalculateTerrainVector(void)
{

	XMFLOAT3 Tangent = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Binormal = { 0.0f,0.0f,0.0f };

	int faceCount = (hmInfo.terrainWidth) * (hmInfo.terrainHeight) / 3;	// 三角形

	int index = 0;

	// 頂点データがTemp関数にコピー
	for (int i = 0; i < faceCount; i++)
	{
		// 頂点1番目
		temp1.x = g_Vertex[index].Position.x;
		temp1.y = g_Vertex[index].Position.y;
		temp1.z = g_Vertex[index].Position.z;
		temp1.tu = g_Vertex[index].TexCoord.x;
		temp1.tv = g_Vertex[index].TexCoord.y;
		index++;

		// 頂点２番目
		temp2.x = g_Vertex[index].Position.x;
		temp2.y = g_Vertex[index].Position.y;
		temp2.z = g_Vertex[index].Position.z;
		temp2.tu = g_Vertex[index].TexCoord.x;
		temp2.tv = g_Vertex[index].TexCoord.y;
		index++;

		// 頂点3番目
		temp3.x = g_Vertex[index].Position.x;
		temp3.y = g_Vertex[index].Position.y;
		temp3.z = g_Vertex[index].Position.z;
		temp3.tu = g_Vertex[index].TexCoord.x;
		temp3.tv = g_Vertex[index].TexCoord.y;
		index++;

		/**********************************
		  TANGENT、NORMAL、BINORMALの計算
		**********************************/

		float vector1[3] = { 0.0f,0.0f,0.0f };
		float vector2[3] = { 0.0f,0.0f,0.0f };

		float tuVector[2] = { 0.0f,0.0f };
		float tvVector[2] = { 0.0f,0.0f };

		float den = 0.0f;
		float length = 0.0f;

		// ベクトルの計算
		vector1[0] = temp2.x - temp1.x;
		vector1[1] = temp2.y - temp1.y;
		vector1[2] = temp2.z - temp1.z;

		vector2[0] = temp3.x - temp1.x;
		vector2[1] = temp3.y - temp1.y;
		vector2[2] = temp3.z - temp1.z;

		// tuやtvのベクトル計算
		tuVector[0] = temp2.tu - temp1.tu;
		tvVector[0] = temp2.tv - temp1.tv;

		tuVector[1] = temp3.tu - temp1.tu;
		tvVector[1] = temp3.tv - temp1.tv;

		// denominatorの計算
		den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

		// TangentやBinormalを習得するため、外積計算
		Tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		Tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		Tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		Binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		Binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		Binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Tangentの長さを計算
		length = (float)sqrt((Tangent.x * Tangent.x) + (Tangent.y * Tangent.y) + (Tangent.z * Tangent.z));

		// Tangentの長さのノーマライズ
		Tangent.x = Tangent.x / length;
		Tangent.y = Tangent.y / length;
		Tangent.z = Tangent.z / length;

		// Binormalの長さの計算
		length = (float)sqrt((Binormal.x * Binormal.x) + (Binormal.y * Binormal.y) + (Binormal.z * Binormal.z));

		// Binormalの長さのノーマライズ
		Binormal.x = Binormal.x / length;
		Binormal.y = Binormal.y / length;
		Binormal.z = Binormal.z / length;

		// Tangent、Binormal、法線を設定する
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