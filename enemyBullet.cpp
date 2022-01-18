//=============================================================================
//
// エネミーバレット処理 [EnemyBullet.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)				// テクスチャの数

#define	EnemyBullet_SIZE_X		(75.0f)	// 頂点サイズ
#define	EnemyBullet_SIZE_Y		(75.0f)	// 頂点サイズ
#define	VALUE_MOVE_EnemyBullet	(5.0f)		// 移動速度

#define	DISP_SHADOW							// 影の表示
//#undef DISP_SHADOW


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemyBullet(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;		// 頂点バッファ

static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;					// テクスチャ番号

static ENEMYBULLET				g_EBullet[MAX_EBULLET];		// エネミーバレットワーク
static XMFLOAT3					playerPos;						// ビルボード発生位置
static float					g_roty = 0.0f;					// 移動方向
static float					g_spd = 0.0f;					// 移動スピード

static char *g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/Circ_Deg13.png",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyBullet(void)
{
	// 頂点情報の作成
	MakeVertexEnemyBullet();

	// テクスチャ生成
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

	// エネミーバレットワークの初期化
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
// 終了処理
//=============================================================================
void UninitEnemyBullet(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemyBullet(void)
{
	{
		for(int nCntEnemyBullet = 0; nCntEnemyBullet < MAX_EBULLET; nCntEnemyBullet++)
		{
			if(g_EBullet[nCntEnemyBullet].bUse)
			{// 使用中
				g_EBullet[nCntEnemyBullet].pos.x += g_EBullet[nCntEnemyBullet].move.x;
				g_EBullet[nCntEnemyBullet].pos.z += g_EBullet[nCntEnemyBullet].move.z;

				g_EBullet[nCntEnemyBullet].move.x += (0.0f - g_EBullet[nCntEnemyBullet].move.x) * 0.015f;
				g_EBullet[nCntEnemyBullet].move.z += (0.0f - g_EBullet[nCntEnemyBullet].move.z) * 0.015f;

#ifdef DISP_SHADOW
				if(g_EBullet[nCntEnemyBullet].nIdxShadow != -1)
				{// 影使用中
					float colA;

					// 影の位置設定
					SetPositionShadow(g_EBullet[nCntEnemyBullet].nIdxShadow, XMFLOAT3(g_EBullet[nCntEnemyBullet].pos.x, 0.1f, g_EBullet[nCntEnemyBullet].pos.z));

					// 影の色の設定
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

#ifdef _DEBUG	// デバッグ情報を表示する
		PrintDebugProc("\nEnemyBullet:X:%f Y:%f Z:%f\n", g_EBullet[0].pos.x, g_EBullet[0].pos.y, g_EBullet[0].pos.z);
#endif

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyBullet(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ライティングを無効に
	SetLightEnable(FALSE);

	// Z比較無し
	SetDepthEnable(FALSE);

	// フォグ無効
	SetFogEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for(int nCntEnemyBullet = 0; nCntEnemyBullet < MAX_EBULLET; nCntEnemyBullet++)
	{
		if(g_EBullet[nCntEnemyBullet].bUse)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			mtxWorld = XMMatrixInverse(nullptr, mtxView);
			mtxWorld.r[3].m128_f32[0] = 0.0f;
			mtxWorld.r[3].m128_f32[1] = 0.0f;
			mtxWorld.r[3].m128_f32[2] = 0.0f;

			// スケールを反映
			mtxScl = XMMatrixScaling(g_EBullet[nCntEnemyBullet].scale.x, g_EBullet[nCntEnemyBullet].scale.y, g_EBullet[nCntEnemyBullet].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_EBullet[nCntEnemyBullet].pos.x, g_EBullet[nCntEnemyBullet].pos.y, g_EBullet[nCntEnemyBullet].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_EBullet[nCntEnemyBullet].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較有効
	SetDepthEnable(TRUE);

	// フォグ有効
	SetFogEnable(TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEnemyBullet(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-EnemyBullet_SIZE_X / 2, EnemyBullet_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(EnemyBullet_SIZE_X / 2, EnemyBullet_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-EnemyBullet_SIZE_X / 2, -EnemyBullet_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(EnemyBullet_SIZE_X / 2, -EnemyBullet_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorEnemyBullet(int nIdxEnemyBullet, XMFLOAT4 col)
{
	g_EBullet[nIdxEnemyBullet].material.Diffuse = col;
}

//=============================================================================
// エネミーバレットの発生処理
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
			// 影の設定
			g_EBullet[nCntEnemyBullet].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
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
