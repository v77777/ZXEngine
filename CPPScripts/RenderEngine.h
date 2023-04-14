#pragma once
#include "pubh.h"
#include <GLFW/glfw3.h>

namespace ZXEngine
{
	class Camera;
	class RenderEngine
	{
	public:
		static void Create();
		static RenderEngine* GetInstance();

	private:
		static RenderEngine* mInstance;

	public:
		RenderEngine();
		~RenderEngine() {};

		// 窗口相关
		GLFWwindow* window;

		void InitWindow();
		void CloseWindow(string args);
		int WindowShouldClose();

		// 绘制
		void BeginRender();
		void Render(Camera* camera);
		void EndRender();
		
	private:
		// 获取输入事件
		void PollEvents();
	};
}