#include "pch.h"

#include "Key.h"


Key & Key::GetInstance()
{
	static Key* instance = new Key();
	if (instance == nullptr)
	{
		instance = new Key();
	}

	return *instance;
}

void Key::Update()
{
	// �L�[���͏����擾
	DirectX::Keyboard::State key = m_keyboard->GetState();
	m_keyTracker->Update(key);

}

Key::Key()
{
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
}


