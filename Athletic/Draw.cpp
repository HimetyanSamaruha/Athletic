//∞----------------------------------------------------∞
//∞*ファイル名：Draw.cpp								∞
//∞*内容　　　：描画クラス								∞
//∞*制作者　　：Ayaka Yamanaka							∞
//∞----------------------------------------------------∞


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

//Device、Contextを読み込む
void Draw::InitializeStatic(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext)
{
	m_d3dContext = d3dContext;
	m_d3dDevice = d3dDevice;
	// スプライトバッチ
	m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext);
	// スプライトフォント
	m_spriteFont = std::make_unique<SpriteFont>(m_d3dDevice, L"myfile.spritefont");

}
//取得
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

