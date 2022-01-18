//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/main_menu.png",
	"data/TEXTURE/title_logo.png",
	"data/TEXTURE/press_any_button.png",
	"data/TEXTURE/keyboard_tutorial.png",
	"data/TEXTURE/gamepad_tutorial.png",
	"data/TEXTURE/tutorial.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;					// ポリゴンの座標
static XMFLOAT3					g_PosLogo;					// ポリゴンの座標
static XMFLOAT3					g_PosPressStart;			// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

float	alpha;
BOOL	flag_alpha;

static BOOL						g_Load = FALSE;
static bool						isTutorial = false;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();
	isTutorial = false;

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


	// 変数の初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos             = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_PosLogo         = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_PosPressStart   = XMFLOAT3(g_w/2, g_h/2 + 75.0f, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_title);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	if (isTutorial)
	{
		if (g_TexNo == 5)
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter押したら、ステージを切り替える
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_click);
			}
			// ゲームパッドで入力処理
			else if (IsButtonTriggered(0, BUTTON_START))
			{
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_click);
			}
			else if (IsButtonTriggered(0, BUTTON_B))
			{
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_click);
			}

			return;

		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo++;
		}
		// ゲームパッドで入力処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo++;
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo++;
		}

	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo = 3;
			isTutorial = true;
		}
		// ゲームパッドで入力処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo = 3;
			isTutorial = true;
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_click);
			g_TexNo = 3;
			isTutorial = true;
		}
	}

	if (flag_alpha == TRUE)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			flag_alpha = FALSE;
		}
	}
	else
	{
		alpha += 0.02f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			flag_alpha = TRUE;
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
void DrawTitle(void)
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

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	if (!isTutorial)
	{
		// タイトルのロゴを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, g_PosLogo.x, g_PosLogo.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// タイトルのロゴを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, g_PosPressStart.x, g_PosPressStart.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO * 0.75f, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}





