#include "CubeMap.h"

namespace ZXEngine
{
	CubeMap::CubeMap(vector<string> path)
	{
		ID = RenderAPI::GetInstance()->LoadCubeMap(path);
	}

	CubeMap::~CubeMap()
	{
		RenderAPI::GetInstance()->DeleteTexture(ID);
	}

	unsigned int CubeMap::GetID()
	{
		return ID;
	}
}