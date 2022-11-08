#include "SceneManager.h"
#include "Resources.h"

namespace ZXEngine
{
	SceneManager* SceneManager::mInstance = nullptr;

	void SceneManager::Create()
	{
		mInstance = new SceneManager();
	}

	SceneManager* SceneManager::GetInstance()
	{
		return mInstance;
	}

	Scene* SceneManager::GetCurScene()
	{
		return curScene;
	}

	void SceneManager::SetCurScene(const char* path)
	{
		curScene = new Scene(Resources::LoadScene(path));
	}
}