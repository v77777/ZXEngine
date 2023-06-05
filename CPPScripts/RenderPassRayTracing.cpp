#include "RenderPassRayTracing.h"
#include "RenderAPI.h"
#include "RenderQueueManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "MaterialData.h"
#include "ZCamera.h"
#include "Light.h"
#include "FBOManager.h"
#include "GlobalData.h"

namespace ZXEngine
{
	RenderPassRayTracing::RenderPassRayTracing()
	{
		auto renderAPI = RenderAPI::GetInstance();

		asCommandID = renderAPI->AllocateDrawCommand(CommandType::RayTracing);
		rtCommandID = renderAPI->AllocateDrawCommand(CommandType::RayTracing);

		ClearInfo clearInfo = {};
		clearInfo.clearFlags = ZX_CLEAR_FRAME_BUFFER_COLOR_BIT;
		FBOManager::GetInstance()->CreateFBO("RayTracing", FrameBufferType::RayTracing, clearInfo);
	}

	Matrix4 lastVP;
	uint32_t frameCount = 0;
	void RenderPassRayTracing::Render(Camera* camera)
	{
		auto renderAPI = RenderAPI::GetInstance();
		auto renderQueue = RenderQueueManager::GetInstance()->GetRenderQueue((int)RenderQueueType::Qpaque);

		// 切换到主FBO
		FBOManager::GetInstance()->SwitchFBO("RayTracing");
		// ViewPort设置为窗口大小
		renderAPI->SetViewPort(GlobalData::srcWidth, GlobalData::srcHeight);

		// 遍历渲染队列，将所有mesh的VAO和材质数据推入光追管线
		for (auto renderer : renderQueue->GetRenderers())
		{
			renderAPI->PushRayTracingMaterialData(renderer->matetrial);
			for (auto mesh : renderer->meshes)
			{
				renderAPI->PushAccelerationStructure(mesh->VAO, renderer->matetrial->data->GetRTID(), renderer->GetTransform()->GetModelMatrix());
			}
		}

		// 构建TLAS
		renderAPI->BuildTopLevelAccelerationStructure(asCommandID);

		// 光追管线常量数据
		rtConstants.VP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		rtConstants.V_Inv = camera->GetViewMatrixInverse();
		rtConstants.P_Inv = camera->GetProjectionMatrixInverse();

		// 光源位置
		auto allLights = Light::GetAllLights();
		if (allLights.size() > 0)
			rtConstants.lightPos = Light::GetAllLights()[0]->GetTransform()->GetPosition();

		if (lastVP != rtConstants.VP)
		{
			frameCount = 0;
			lastVP = rtConstants.VP;
		}

		// 帧计数
		rtConstants.frameCount = frameCount++;

		// 光追渲染
		renderAPI->RayTrace(rtCommandID, rtConstants);

		// 清理渲染队列
		RenderQueueManager::GetInstance()->ClearAllRenderQueue();
	}
}