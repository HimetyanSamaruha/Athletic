//��----------------------------------------------------��
//��*�t�@�C�����FDraw.cpp								��
//��*���e�@�@�@�F�`��N���X								��
//��*����ҁ@�@�FAyaka Yamanaka							��
//��----------------------------------------------------��


#include "pch.h"
#include "Draw.h"
#include <string>
#include <iostream>
#include <locale> 
#include <codecvt> 
#include <cstdio>


using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

//Device�AContext��ǂݍ���
void Draw::InitializeStatic(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext)
{
	m_d3dContext = d3dContext;
	m_d3dDevice = d3dDevice;
	// �X�v���C�g�o�b�`
	m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext);
	// �X�v���C�g�t�H���g
	m_spriteFont = std::make_unique<SpriteFont>(m_d3dDevice, L"myfile.spritefont");

}
//�擾
Draw& Draw::GetInstance()
{
	static Draw* instance = new Draw();
	if (instance == nullptr)
	{
		instance = new Draw();
	}

	return *instance;
}


Draw::Draw()
{
}


Draw::~Draw()
{
}

