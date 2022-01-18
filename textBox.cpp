//=============================================================================
//
// スコア処理 [TextBox.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "textBox.h"
#include "input.h"
#include "sprite.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(500.0f)	// キャラサイズ
#define TEXTURE_HEIGHT				(300.0f)	// 
#define TEXTURE_MAX					(5)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/frame.png",
	"data/TEXTURE/cursor.png",
	"data/TEXTURE/title_back.png",
	"data/TEXTURE/yes_text.png",
	"data/TEXTURE/no_text.png",
};

static BOOL						g_Load = FALSE;

static TEXTBOX					g_TextBox[TEXTURE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTextBox(void)
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

	// 初期化
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_TextBox[i].use = FALSE;
		g_TextBox[i].width = TEXTURE_WIDTH;
		g_TextBox[i].height = TEXTURE_HEIGHT;
		g_TextBox[i].position = { 0.0f, 0.0f, 0.0f };
		g_TextBox[i].cursoPos = 0;
		g_TextBox[i].texNo = i;
	}

	g_TextBox[FRAME_BOX].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f };

	g_TextBox[TITLE_TEXT].width = 400.0f;
	g_TextBox[TITLE_TEXT].height = 80.0f;
	g_TextBox[TITLE_TEXT].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y - 50.0f, 0.0f };

	g_TextBox[YES_TEXT].width = 300.0f;
	g_TextBox[YES_TEXT].height = 80.0f;
	g_TextBox[YES_TEXT].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f };

	g_TextBox[NO_TEXT].width = 300.0f;
	g_TextBox[NO_TEXT].height = 80.0f;
	g_TextBox[NO_TEXT].position = { SCREEN_CENTER_X, SCREEN_CENTER_Y + 50.0f, 0.0f };

	g_TextBox[CURSOR].width = 100.0f;
	g_TextBox[CURSOR].height = 50.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTextBox(void)
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
void UpdateTextBox(void)
{

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTextBox(void)
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
		if (g_TextBox[i].use)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TextBox[i].texNo]);

			// スコアの位置やテクスチャー座標を反映
			float px = g_TextBox[i].position.x;
			float py = g_TextBox[i].position.y;
			float pw = g_TextBox[i].width;
			float ph = g_TextBox[i].height;

			float tw = 1.0f;		// テクスチャの幅
			float th = 1.0f;		// テクスチャの高さ
			float tx = 0.0f;		// テクスチャの左上X座標
			float ty = 0.0f;		// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void ShowTextBox(BOOL flag)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_TextBox[i].use = flag;
	}
}

TEXTBOX * GetTextBox(void)
{
	return &g_TextBox[0];
}
