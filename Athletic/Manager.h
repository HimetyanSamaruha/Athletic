#pragma once
#include <Keyboard.h>

class SceneBase;

class Manager
{
public:
	Manager();
	~Manager();
	//ƒV[ƒ“‚ğ•ÏX‚·‚é
	void Scene(SceneBase* nextScene);
	void Update();

	void Render();

	
private:
	SceneBase* m_base;

};

class SceneBase
{
protected:
	int m_TimeCnt = 0;
	int m_NextScene = 0;

public:
	virtual ~SceneBase() {}
	virtual void Initialize() = 0;
	virtual void Update(Manager* main) = 0;
	virtual void Render() = 0;
	virtual void SceneChange(Manager * main) = 0;
	int m_scene = 0;

};


