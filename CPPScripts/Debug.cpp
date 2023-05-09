#include "Debug.h"
#include <iostream>
#include "PublicEnum.h"
#include "ProjectSetting.h"
#include "Editor/EditorDataManager.h"

namespace ZXEngine
{
	void Debug::Log(const std::string& message)
	{
		std::cout << "Log:     " << message << std::endl;

		if (ProjectSetting::logToFile)
		{
			WriteToFile("Log:     " + message);
		}

#ifdef ZX_EDITOR
		EditorDataManager::GetInstance()->AddLog(LogType::Message, message);
#endif
	}

	void Debug::LogWarning(const std::string& message)
	{
		std::cout << "Warning: " << message << std::endl;

		if (ProjectSetting::logToFile)
		{
			WriteToFile("Warning: " + message);
		}

#ifdef ZX_EDITOR
		EditorDataManager::GetInstance()->AddLog(LogType::Warning, message);
#endif
	}

	void Debug::LogError(const std::string& message)
	{
		std::cout << "Error:   " << message << std::endl;

		if (ProjectSetting::logToFile)
		{
			WriteToFile("Error:   " + message);
		}

#ifdef ZX_EDITOR
		EditorDataManager::GetInstance()->AddLog(LogType::Error, message);
#endif
	}

#ifdef ZX_DEBUG
	int Debug::drawCallCount;
	void Debug::Update() 
	{
		Log("Draw Call: " + std::to_string(drawCallCount));
		drawCallCount = 0;
	}
#endif

	void Debug::WriteToFile(const std::string& message)
	{
		std::ofstream f(ProjectSetting::projectPath + "/log.txt", std::ios::app);

		if (!f.is_open())
		{
			std::cerr << "Open log file failed !" << std::endl;
		}

		f << message << std::endl;

		f.close();
	}
}