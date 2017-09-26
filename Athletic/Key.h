//��----------------------------------------------------��
//��*�t�@�C�����FKey.h									��
//��*���e�@�@�@�F�L�[�{�[�h����							��
//��*����ҁ@�@�FAyaka Yamanaka							��
//��----------------------------------------------------��

#pragma once
#include <Keyboard.h>

class Key
{
public:
	// �L�[�{�[�h�֌W
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyTracker;

	static Key& GetInstance();
	void Update();

private:
	Key();
};