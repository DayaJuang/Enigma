//=============================================================================
//
// スコア処理 [playerUI.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "playerUI.h"
#include "input.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FRAME_WIDTH				(510.0f)	// キャラサイズ
#define FRAME_HEIGHT			(50.0f)	// 
#define BAR_WIDTH				(450.0f)	// キャラサイズ
#define BAR_HEIGHT				(10.0f)	// 
#define TEXTURE_MAX					(3)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/healthFrame.png",
	"data/TEXTURE/playerHP.png",
	"data/TEXTURE/playerMP.png"
};

static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;

UI							g_UI[TEXTURE_MAX];					// UI

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI(void)
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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// プレイヤーUIの初期化
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
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateUI(void)
{

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
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
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		// プレイヤーUIの位置やテクスチャー座標を反映
		float px = g_UI[i].position.x;			// プレイヤーUIの表示位置X
		float py = g_UI[i].position.y;			// プレイヤーUIの表示位置Y
		float pw = g_UI[i].width;				// プレイヤーUIの表示幅
		float ph = g_UI[i].height;				// プレイヤーUIの表示高さ

		float tw = 1.0f;			// テクスチャの幅
		float th = 1.0f;			// テクスチャの高さ
		float tx = 0.0f;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}

UI * GetUI(void)
{
	return &g_UI[0];
}
