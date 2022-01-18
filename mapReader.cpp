//=============================================================================
//
// ���b�V���n�ʂ̏��� [mapReader.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "mapReader.h"
#include "renderer.h"
#include "debugproc.h"

static BOOL			g_Load = FALSE;

// 16bit�t�B�[���h�f�[�^��ǂݍ��ފ֐�
BOOL LoadRawHeightMap(char * filename, HEIGHTMAPINFO & hmInfo)
{
	FILE *filePtr;
	unsigned long long size, index;

	// �t�@�C�����J��
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return FALSE;

	// �摜�̕��ƍ������擾
	hmInfo.terrainWidth = 1025;
	hmInfo.terrainHeight = 1025;

	// �摜�̃T�C�Y
	size = hmInfo.terrainHeight * hmInfo.terrainWidth;

	// �z����������i�F�f�[�^���i�[�z��j
	unsigned short* rawImage = new unsigned short[size];

	// �摜�f�[�^��ǂ�
	fread(rawImage, sizeof(unsigned short), size, filePtr);

	// �t�@�C����߂�
	fclose(filePtr);

	// heighMap�̔z���������
	hmInfo.heightMap = new XMFLOAT3[size];

	// �摜�f�[�^��ǂ�
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

// �J�[���[�f�[�^��ǂݍ��ފ֐�
BOOL LoadBitMap(char * filename, HEIGHTMAPINFO & hmInfo)
{
	FILE *filePtr;
	BITMAPFILEHEADER bmFileHeader;	// �ǂݍ��񂾃t�@�C���̃f�[�^���i�[
	BITMAPINFOHEADER bmInfoHeader;	// �ǂݍ��񂾃t�@�C���̒��ɂ���摜�f�[�^���i�[
	int size, index;

	// �t�@�C�����J��
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return FALSE;

	// bitmap�w�b�_�[��ǂݍ���
	fread(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// bitmap�C���t�H���[�V������ǂݍ���
	fread(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// �摜�̕��ƍ������擾
	hmInfo.terrainWidth = bmInfoHeader.biWidth;
	hmInfo.terrainHeight = bmInfoHeader.biHeight;

	// �摜�̃T�C�Y �~�@3�iRGB�j+ 1�i�r�b�g�����炷�ׁj
	size = hmInfo.terrainHeight * ((hmInfo.terrainWidth * 3) + 1);

	// �z����������i�F�f�[�^���i�[�z��j
	unsigned char* bitmapImage = new unsigned char[size];

	// �t�@�C���̃|�C���^�[���Z�b�g����
	fseek(filePtr, bmFileHeader.bfOffBits, SEEK_SET);

	// �摜�f�[�^��ǂ�
	fread(bitmapImage, sizeof(unsigned char), size, filePtr);

	// �t�@�C����߂�
	fclose(filePtr);

	int k = 0;

	// �摜�f�[�^��ǂ�
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

// �J�[���[�f�[�^��ǂݍ��ފ֐�
BOOL LoadColorMap(char * filename, HEIGHTMAPINFO & hmInfo)
{
	FILE *filePtr;
	BITMAPFILEHEADER bmFileHeader;	// �ǂݍ��񂾃t�@�C���̃f�[�^���i�[
	BITMAPINFOHEADER bmInfoHeader;	// �ǂݍ��񂾃t�@�C���̒��ɂ���摜�f�[�^���i�[
	int size, index;

	// �t�@�C�����J��
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return FALSE;

	// bitmap�w�b�_�[��ǂݍ���
	fread(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// bitmap�C���t�H���[�V������ǂݍ���
	fread(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// �摜�̕��ƍ������擾
	hmInfo.terrainWidth = bmInfoHeader.biWidth;
	hmInfo.terrainHeight = bmInfoHeader.biHeight;

	// �摜�̃T�C�Y �~�@3�iRGB�j+ 1�i�r�b�g�����炷�ׁj
	size = hmInfo.terrainHeight * ((hmInfo.terrainWidth * 3) + 1);

	// �z����������i�F�f�[�^���i�[�z��j
	unsigned char* bitmapImage = new unsigned char[size];

	// �t�@�C���̃|�C���^�[���Z�b�g����
	fseek(filePtr, bmFileHeader.bfOffBits, SEEK_SET);

	// �摜�f�[�^��ǂ�
	fread(bitmapImage, sizeof(unsigned char), size, filePtr);

	// �t�@�C����߂�
	fclose(filePtr);

	int k = 0;

	// �摜�f�[�^��ǂ�
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