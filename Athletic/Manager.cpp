#include "pch.h"
#include "Manager.h"


Manager::Manager()
{
	m_base = nullptr;
}


Manager::~Manager()
{
}

void Manager::Scene(SceneBase* nextScene)
{
	// 現在のステートを破棄する
	if (m_base != nullptr) delete m_base;

	m_base = nextScene;
}

void Manager::Update()
{
	m_base->Update(this);
	m_base->SceneChange(this);
}

void Manager::Render()
{
	m_base->Render();
}

