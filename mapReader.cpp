//=============================================================================
//
// メッシュ地面の処理 [mapReader.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "mapReader.h"
#include "renderer.h"
#include "debugproc.h"

static BOOL			g_Load = FALSE;

// 16bitフィールドデータを読み込む関数
BOOL LoadRawHeightMap(char * filename, HEIGHTMAPINFO & hmInfo)
{
	FILE *filePtr;
	unsigned long long size, index;

	// ファイルを開く
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return FALSE;

	// 画像の幅と高さを取得
	hmInfo.terrainWidth = 1025;
	hmInfo.terrainHeight = 1025;

	// 画像のサイズ
	size = hmInfo.terrainHeight * hmInfo.terrainWidth;

	// 配列を初期化（色データを格納配列）
	unsigned short* rawImage = new unsigned short[size];

	// 画像データを読む
	fread(rawImage, sizeof(unsigned short), size, filePtr);

	// ファイルを閉める
	fclose(filePtr);

	// heighMapの配列を初期化
	hmInfo.heightMap = new XMFLOAT3[size];

	// 画像データを読む
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			index = (hmInfo.terrainWidth * j) + i;

			hmInfo.heightMap[index].y = (float)rawImage[index];
		}
	}

	delete[] rawImage;
	rawImage = 0;

	return TRUE;
}

// カーラーデータを読み込む関数
BOOL LoadBitMap(char * filename, HEIGHTMAPINFO & hmInfo)
{
	FILE *filePtr;
	BITMAPFILEHEADER bmFileHeader;	// 読み込んだファイルのデータを格納
	BITMAPINFOHEADER bmInfoHeader;	// 読み込んだファイルの中にある画像データを格納
	int size, index;

	// ファイルを開く
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return FALSE;

	// bitmapヘッダーを読み込み
	fread(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// bitmapインフォメーションを読み込み
	fread(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// 画像の幅と高さを取得
	hmInfo.terrainWidth = bmInfoHeader.biWidth;
	hmInfo.terrainHeight = bmInfoHeader.biHeight;

	// 画像のサイズ ×　3（RGB）+ 1（ビットをずらす為）
	size = hmInfo.terrainHeight * ((hmInfo.terrainWidth * 3) + 1);

	// 配列を初期化（色データを格納配列）
	unsigned char* bitmapImage = new unsigned char[size];

	// ファイルのポインターをセットする
	fseek(filePtr, bmFileHeader.bfOffBits, SEEK_SET);

	// 画像データを読む
	fread(bitmapImage, sizeof(unsigned char), size, filePtr);

	// ファイルを閉める
	fclose(filePtr);

	int k = 0;

	// 画像データを読む
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			index = (j * hmInfo.terrainWidth) + i;

			hmInfo.heightMap[index].y = (float)bitmapImage[index];

			k += 3;
		}

		k++;
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return TRUE;
}

// カーラーデータを読み込む関数
BOOL LoadColorMap(char * filename, HEIGHTMAPINFO & hmInfo)
{
	FILE *filePtr;
	BITMAPFILEHEADER bmFileHeader;	// 読み込んだファイルのデータを格納
	BITMAPINFOHEADER bmInfoHeader;	// 読み込んだファイルの中にある画像データを格納
	int size, index;

	// ファイルを開く
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return FALSE;

	// bitmapヘッダーを読み込み
	fread(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// bitmapインフォメーションを読み込み
	fread(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// 画像の幅と高さを取得
	hmInfo.terrainWidth = bmInfoHeader.biWidth;
	hmInfo.terrainHeight = bmInfoHeader.biHeight;

	// 画像のサイズ ×　3（RGB）+ 1（ビットをずらす為）
	size = hmInfo.terrainHeight * ((hmInfo.terrainWidth * 3) + 1);

	// 配列を初期化（色データを格納配列）
	unsigned char* bitmapImage = new unsigned char[size];

	// ファイルのポインターをセットする
	fseek(filePtr, bmFileHeader.bfOffBits, SEEK_SET);

	// 画像データを読む
	fread(bitmapImage, sizeof(unsigned char), size, filePtr);

	// ファイルを閉める
	fclose(filePtr);

	int k = 0;

	// 画像データを読む
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			index = (j * hmInfo.terrainWidth) + i;

			hmInfo.bmColor[index].x = (float)bitmapImage[k] / 255.0f;		// R
			hmInfo.bmColor[index].y = (float)bitmapImage[k + 1] / 255.0f;	// G
			hmInfo.bmColor[index].z = (float)bitmapImage[k + 2] / 255.0f;	// B
			hmInfo.bmColor[index].w = 1.0f;

			k += 3;
		}

		k++;
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return TRUE;
}