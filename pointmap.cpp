//=============================================================================
//
// スコア処理 [pointMap.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(3.0f)	// キャラサイズ
#define TEXTURE_HEIGHT				(3.0f)	// 
#define BORDER_SIZE					(10.0f)	// キャラサイズ
#define TEXTURE_MAX					(1)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/MINIMAP/point.jpg",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPoint(void)
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


	// ポイントの初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { 0.0f , 0.0f , 0.0f };
	g_TexNo = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void UpdatePoint(float posX, float posY)
{
	MINIMAP *miniMap = GetMiniMap();

	// ミニマップの境界を設定する
	float xMinBound = miniMap->position.x - (miniMap->width * miniMap->scale.x - BORDER_SIZE) * 0.5f;
	float xMaxBound = miniMap->position.x + (miniMap->width * miniMap->scale.x - BORDER_SIZE) * 0.5f;

	float yMinBound = miniMap->position.y - (miniMap->height* miniMap->scale.y  - BORDER_SIZE) * 0.5f;
	float yMaxBound = miniMap->position.y + (miniMap->height* miniMap->scale.y  - BORDER_SIZE) * 0.5f;

	float percentX = posX / GetMapWidth() * 0.1f * miniMap->scale.x;
	float percentY = -posY / GetMapHeight() * 0.1f * miniMap->scale.y;

	g_Pos.x = miniMap->position.x + miniMap->width * percentX;
	g_Pos.y = miniMap->position.y + miniMap->height * percentY;

	// ポイントがミニマップから離れないように
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

#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("point Position: X:%f Y:%f \n", percentX,percentY);
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPoint(void)
{
	MINIMAP *miniMap = GetMiniMap();

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

	float px = g_Pos.x;								// ポイントの表示位置X
	float py = g_Pos.y;								// ポイントの表示位置Y
	float pw = g_w * miniMap->scale.x;				// ポイントの表示幅
	float ph = g_h * miniMap->scale.y;				// ポイントの表示高さ

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