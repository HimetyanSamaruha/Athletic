//��----------------------------------------------------��
//��*�t�@�C�����FDraw.h 								��
//��*���e�@�@�@�F�`��N���X								��
//��*����ҁ@�@�FAyaka Yamanaka							��
//��----------------------------------------------------��

#pragma once

#include <d3d11.h>
#include "CommonStates.h"
#include "PrimitiveBatch.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <string.h>
#include <SpriteFont.h>
#include <CommonStates.h>


class Draw
{
public:
	
	//�ÓI�����o�ϐ��̏�����
	void InitializeStatic(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext);

	static Draw & GetInstance();

	~Draw();

	//�f�o�C�X
	ID3D11Device* m_d3dDevice;
	//�f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_d3dContext;


protected:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::CommonStates> m_commonStates;

private:
	Draw();
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

