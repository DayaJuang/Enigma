//=============================================================================
//
// スコア処理 [minimap.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "minimap.h"
#include "input.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(200.0f)	// キャラサイズ
#define TEXTURE_HEIGHT				(200.0f)	// 
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
	"data/MINIMAP/minimap.png",
};


static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;
bool							isMiniMapMaximize = false;

MINIMAP							g_MiniMap;					// スコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMiniMap(void)
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

	// ミニマップの初期化
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
// 終了処理
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
// 更新処理
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


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMiniMap(void)
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
	float px = g_MiniMap.position.x;					// ミニマップの表示位置X
	float py = g_MiniMap.position.y;					// ミニマップの表示位置Y
	float pw = g_MiniMap.width * g_MiniMap.scale.x;		// ミニマップの表示幅
	float ph = g_MiniMap.height * g_MiniMap.scale.y;	// ミニマップの表示高さ

	float tw = 1.0f;			// テクスチャの幅
	float th = 1.0f;			// テクスチャの高さ
	float tx = 0.0f;			// テクスチャの左上X座標
	float ty = 0.0f;			// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

MINIMAP * GetMiniMap(void)
{
	return &g_MiniMap;
}
