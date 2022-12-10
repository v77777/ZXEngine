#include "InputManager.h"
#include "RenderEngine.h"
#include "EventManager.h"

// 因为GLFW的函数接口问题，没办法传递成员函数，所以这里用普通函数包了一层
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

namespace ZXEngine
{
	InputManager* InputManager::mInstance = nullptr;

	void InputManager::Create()
	{
		mInstance = new InputManager();
		mInstance->RegisterMouse();
		mInstance->InitButtonRecord();
	}

	InputManager* InputManager::GetInstance()
	{
		return mInstance;
	}

	void InputManager::RegisterMouse()
	{
		glfwSetCursorPosCallback(RenderEngine::GetInstance()->window, CursorPosCallback);
		glfwSetScrollCallback(RenderEngine::GetInstance()->window, ScrollCallback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(RenderEngine::GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void InputManager::Update()
	{
		UpdateKeyInput();
	}

	void InputManager::UpdateMousePos(double xpos, double ypos)
	{
		EventManager::GetInstance()->FireEvent(EventType::UPDATE_MOUSE_POS, to_string(xpos) + "|" + to_string(ypos));
	}

	void InputManager::UpdateMouseScroll(double xoffset, double yoffset)
	{
		Debug::Log("xoffset " + to_string(xoffset) + " yoffset " + to_string(yoffset));
	}

	void InputManager::UpdateKeyInput()
	{
		// 鼠标按键
		CheckButton(GLFW_MOUSE_BUTTON_1, InputButton::MOUSE_BUTTON_1, EventType::MOUSE_BUTTON_1_PRESS);
		CheckButton(GLFW_MOUSE_BUTTON_2, InputButton::MOUSE_BUTTON_2, EventType::MOUSE_BUTTON_2_PRESS);

		// 从0到9
		for (int i = 0; i < 10; i++)
		{
			CheckButton(GLFW_KEY_0 + i, InputButton(InputButton::KEY_0 + i), EventType(EventType::KEY_0_PRESS + i));
		}

		// 从A到Z
		for (int i = 0; i < 26; i++)
		{
			CheckButton(GLFW_KEY_A + i, InputButton(InputButton::KEY_A + i), EventType(EventType::KEY_A_PRESS + i * 3));
		}

		CheckButton(GLFW_KEY_SPACE, InputButton::KEY_SPACE, EventType::KEY_SPACE_PRESS);
		CheckButton(GLFW_KEY_ESCAPE, InputButton::KEY_ESCAPE, EventType::KEY_ESCAPE_PRESS);
		CheckButton(GLFW_KEY_RIGHT, InputButton::KEY_RIGHT, EventType::KEY_RIGHT_PRESS);
		CheckButton(GLFW_KEY_LEFT, InputButton::KEY_LEFT, EventType::KEY_LEFT_PRESS);
		CheckButton(GLFW_KEY_DOWN, InputButton::KEY_DOWN, EventType::KEY_DOWN_PRESS);
		CheckButton(GLFW_KEY_UP, InputButton::KEY_UP, EventType::KEY_UP_PRESS);
	}

	void InputManager::CheckButton(int id, InputButton button, EventType e)
	{
		GLFWwindow* window = RenderEngine::GetInstance()->window;
		int state = glfwGetKey(window, id);
		
		if (state == GLFW_PRESS && buttonState[button] == GLFW_PRESS)
			EventManager::GetInstance()->FireEvent(e, ""); // Press
		else if (state == GLFW_PRESS && buttonState[button] == GLFW_RELEASE)
			EventManager::GetInstance()->FireEvent(e + 1, ""); // Down
		else if (state == GLFW_RELEASE && buttonState[button] == GLFW_PRESS)
			EventManager::GetInstance()->FireEvent(e + 2, ""); // Up

		buttonState[button] = state;
	}

	void InputManager::InitButtonRecord()
	{
		for (int i = 0; i < InputButton::END; i++)
		{
			buttonState[i] = GLFW_RELEASE;
		}
	}
}


void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	ZXEngine::InputManager::GetInstance()->UpdateMousePos(xpos, ypos);
}
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ZXEngine::InputManager::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}