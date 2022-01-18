//=============================================================================
//
// スコア処理 [enemyPoint.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "minimap.h"
#include "enemyPoint.h"
#include "meshfield.h"
#include "sprite.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(3.0f)	// キャラサイズ
#define TEXTURE_HEIGHT				(3.0f)	// 
#define BORDER_SIZE					(10.0f)	// キャラサイズ
#define TEXTURE_MAX					(1)		// テクスチャの数
#define MAX_POINT					(5)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/MINIMAP/enemyPoint.jpg",
};

ENEMYPOINT						g_EPoint[MAX_POINT];
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyPoint(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < MAX_POINT; i++)
	{
		g_EPoint[i].use = TRUE;
		g_EPoint[i].width = TEXTURE_WIDTH;
		g_EPoint[i].height = TEXTURE_HEIGHT;
		g_EPoint[i].pos = { 0.0f , 0.0f , 0.0f };

	}
	g_TexNo = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyPoint(void)
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
// 更新処理
//=============================================================================
void UpdateEnemyPoint(float posX, float posY, int i)
{
	MINIMAP *miniMap = GetMiniMap();

	// ミニマップの境界を設定する
	float xMinBound = miniMap->position.x - (miniMap->width * miniMap->scale.x - BORDER_SIZE) * 0.5f;
	float xMaxBound = miniMap->position.x + (miniMap->width * miniMap->scale.x - BORDER_SIZE) * 0.5f;

	float yMinBound = miniMap->position.y - (miniMap->height* miniMap->scale.y  - BORDER_SIZE) * 0.5f;
	float yMaxBound = miniMap->position.y + (miniMap->height* miniMap->scale.y  - BORDER_SIZE) * 0.5f;

	float percentX = posX / GetMapWidth() * 0.1f * miniMap->scale.x;
	float percentY = -posY / GetMapHeight() * 0.1f * miniMap->scale.y;

	g_EPoint[i].pos.x = miniMap->position.x + miniMap->width * percentX;
	g_EPoint[i].pos.y = miniMap->position.y + miniMap->height * percentY;

	// ポイントがミニマップから離れないように
	if (g_EPoint[i].pos.x < xMinBound)
	{
		g_EPoint[i].pos.x = xMinBound;
	}

	if (g_EPoint[i].pos.x > xMaxBound)
	{
		g_EPoint[i].pos.x = xMaxBound;
	}

	if (g_EPoint[i].pos.y < yMinBound)
	{
		g_EPoint[i].pos.y = yMinBound;
	}

	if (g_EPoint[i].pos.y > yMaxBound)
	{
		g_EPoint[i].pos.y = yMaxBound;
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyPoint(void)
{
	MINIMAP *miniMap = GetMiniMap();

	for (int i = 0; i < MAX_POINT; i++)
	{
		if (g_EPoint[i].use)
		{
			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

			// マトリクス設定
			SetWorldViewProjection2D();

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			SetMaterial(material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// スコアの位置やテクスチャー座標を反映
			float px = g_EPoint[i].pos.x;									// ポイントの表示位置X
			float py = g_EPoint[i].pos.y;									// ポイントの表示位置Y
			float pw = g_EPoint[i].width * miniMap->scale.x;				// ポイントの表示幅
			float ph = g_EPoint[i].height * miniMap->scale.y;				// ポイントの表示高さ

			float tw = 1.0f;			// テクスチャの幅
			float th = 1.0f;			// テクスチャの高さ
			float tx = 0.0f;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

ENEMYPOINT * GetEnemyPoint(void)
{
	return &g_EPoint[0];
}
