#pragma once
#include "pubh.h"

namespace ZXEngine
{
	class Camera;
	class CubeMap;
	class GameObject;
	struct SceneStruct;
	class Scene
	{
	public:
		CubeMap* skyBox = nullptr;
		vector<GameObject*> gameObjects;

		Scene(SceneStruct* sceneStruct);
		~Scene();

		void Render();

	private:
		vector<Camera*> cameras;
	};
}