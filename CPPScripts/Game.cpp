#include "Game.h"
#include "EventManager.h"
#include "RenderEngine.h"
#include "RenderAPI.h"
#include "RenderQueueManager.h"
#include "InputManager.h"
#include "Time.h"
#include "SceneManager.h"
#include "Resources.h"
#include "RenderPassManager.h"
#include "LuaManager.h"
#include "GameLogicManager.h"
#include "FBOManager.h"

namespace ZXEngine
{
	void Game::Play()
	{
		EventManager::Create();
		RenderQueueManager::Creat();
		SceneManager::Create();
		RenderEngine::Create();
		RenderEngine::GetInstance()->InitWindow(1280, 720);
		RenderAPI::Creat();
		RenderAPI::GetInstance()->InitRenderSetting();
		FBOManager::Create();
		InputManager::Create();
		Resources::SetAssetsPath("../../ExampleProject/Assets/");
		RenderPassManager::Create();
		LuaManager::Create();
		GameLogicManager::Create();
		SceneManager::GetInstance()->SetCurScene("Scenes/MyWorld.zxscene");

		while (!RenderEngine::GetInstance()->WindowShouldClose())
		{
			// �߼�
			Update();

			// ��Ⱦ
			Render();
		}
	}

	void Game::Update()
	{
		Time::Update();

		InputManager::GetInstance()->Update();

		GameLogicManager::GetInstance()->Update();

		Debug::Update();
	}

	void Game::Render()
	{
		RenderEngine::GetInstance()->BeginRender();

		SceneManager::GetInstance()->GetCurScene()->Render();

		RenderEngine::GetInstance()->EndRender();
	}
}