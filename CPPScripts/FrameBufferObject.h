#pragma once
#include "pubh.h"
#include "PublicStruct.h"

namespace ZXEngine
{
	class FrameBufferObject
	{
	public:
		uint32_t ID = 0;
		uint32_t ColorBuffer = 0;
		uint32_t DepthBuffer = 0;
		bool isFollowWindow = false;
		ClearInfo clearInfo = {};
		FrameBufferType type;

		FrameBufferObject() { this->type = FrameBufferType::Normal; };
		FrameBufferObject(FrameBufferType type) { this->type = type; };
	};
}