#include "RenderPassAfterEffectRendering.h"
#include "RenderAPI.h"
#include "ZMesh.h"
#include "ZShader.h"
#include "Resources.h"
#include "FBOManager.h"

namespace ZXEngine
{
	RenderPassAfterEffectRendering::RenderPassAfterEffectRendering()
	{
		InitScreenQuad();
		InitGaussianBlur();
		InitExtractBrightArea();
		InitBloomBlend(true);
	}

	void RenderPassAfterEffectRendering::Render(Camera* camera)
	{
		// 关闭深度测试和写入，整个后处理Pass都不需要关心深度
		RenderAPI::GetInstance()->EnableDepthTest(false);
		RenderAPI::GetInstance()->EnableDepthWrite(false);
		// 整个后处理都在这个覆盖屏幕的四边形上渲染
		screenQuad->Use();

		// 提取画面高亮部分
		string res1 = BlitExtractBrightArea("Main");
		
		// 高斯模糊高亮区域
		string res2 = BlitGaussianBlur(res1, 1, 3.0f);

		// 混合原图和高亮模糊
		string res3 = BlitBloomBlend("Main", res2, true);
	}

	void RenderPassAfterEffectRendering::CreateShader(string name, string path)
	{
		if (aeShaders.count(name) > 0)
		{
			Debug::LogError("Try to add an existing shader");
			return;
		}
		aeShaders.insert(pair<string, Shader*>(name, new Shader(Resources::GetAssetFullPath(path).c_str())));
	}

	Shader* RenderPassAfterEffectRendering::GetShader(string name)
	{
		map<string, Shader*>::iterator iter = aeShaders.find(name);
		if (iter != aeShaders.end())
			return iter->second;
		else
			return nullptr;
	}

	void RenderPassAfterEffectRendering::InitScreenQuad()
	{
		// 这里自己在代码里写一个Quad模型，就不从硬盘加载了
		Vector3 points[4] =
		{
			Vector3(1, 1, 0),
			Vector3(1, -1, 0),
			Vector3(-1, 1, 0),
			Vector3(-1, -1, 0),
		};
		Vector2 coords[4] =
		{
			Vector2(1, 1),
			Vector2(1, 0),
			Vector2(0, 1),
			Vector2(0, 0),
		};
		vector<Vertex> vertices;
		vector<unsigned int> indices =
		{
			2, 3, 1,
			2, 1, 0,
		};
		for (unsigned int i = 0; i < 4; i++)
		{
			Vertex vertex;
			vertex.Position = points[i];
			vertex.Normal = Vector3(1);
			vertex.Tangent = Vector3(1);
			vertex.Bitangent = Vector3(1);
			vertex.TexCoords = coords[i];
			vertices.push_back(vertex);
		}
		screenQuad = new Mesh(vertices, indices);
	}

	void RenderPassAfterEffectRendering::InitExtractBrightArea(bool isFinal)
	{
		CreateShader(ExtractBrightArea, "Shaders/ExtractBrightArea.zxshader");
		if (!isFinal)
			FBOManager::GetInstance()->CreateFBO(ExtractBrightArea, FrameBufferType::Color);
	}

	string RenderPassAfterEffectRendering::BlitExtractBrightArea(string sourceFBO, bool isFinal)
	{
		FBOManager::GetInstance()->SwitchFBO(isFinal ? ScreenBuffer : ExtractBrightArea);
		RenderAPI::GetInstance()->ClearFrameBuffer();
		auto shader = GetShader(ExtractBrightArea);
		shader->Use();
		shader->SetTexture("_RenderTexture", FBOManager::GetInstance()->GetFBO(sourceFBO)->ColorBuffer, 0);
		RenderAPI::GetInstance()->Draw();
		// 返回输出的FBO名字
		return ExtractBrightArea;
	}

	void RenderPassAfterEffectRendering::InitGaussianBlur(bool isFinal)
	{
		CreateShader(GaussianBlur, "Shaders/GaussianBlur.zxshader");
		FBOManager::GetInstance()->CreateFBO("GaussianBlurVertical", FrameBufferType::Color);
		FBOManager::GetInstance()->CreateFBO("GaussianBlurHorizontal", FrameBufferType::Color);
	}

	// blurTimes 反复模糊次数，次数越多越模糊，但是这个很影响性能，别开太高
	// texOffset 采样偏移距离，1代表偏移1像素，越大越模糊，这个不影响性能，但是太大了效果会不正常
	string RenderPassAfterEffectRendering::BlitGaussianBlur(string sourceFBO, int blurTimes, float texOffset, bool isFinal)
	{
		bool isHorizontal = true;
		string pingpongBuffer[2] = { "GaussianBlurHorizontal", "GaussianBlurVertical" };
		auto shader = GetShader(GaussianBlur);
		shader->Use();
		shader->SetFloat("_TexOffset", texOffset);
		for (int i = 0; i < blurTimes * 2; i++)
		{
			FBOManager::GetInstance()->SwitchFBO(pingpongBuffer[isHorizontal]);
			string colorFBO = i == 0 ? sourceFBO : pingpongBuffer[!isHorizontal];
			shader->SetBool("_Horizontal", isHorizontal);
			shader->SetTexture("_RenderTexture", FBOManager::GetInstance()->GetFBO(colorFBO)->ColorBuffer, 0);
			RenderAPI::GetInstance()->Draw();
			isHorizontal = !isHorizontal;
		}
		// 返回最终输出的FBO名字
		return pingpongBuffer[!isHorizontal];
	}

	void RenderPassAfterEffectRendering::InitBloomBlend(bool isFinal)
	{
		CreateShader(BloomBlend, "Shaders/BloomBlend.zxshader");
		if (!isFinal)
			FBOManager::GetInstance()->CreateFBO(BloomBlend, FrameBufferType::Color);
	}

	string RenderPassAfterEffectRendering::BlitBloomBlend(string originFBO, string blurFBO, bool isFinal)
	{
		FBOManager::GetInstance()->SwitchFBO(isFinal ? ScreenBuffer : BloomBlend);
		auto shader = GetShader(BloomBlend);
		shader->Use();
		shader->SetTexture("_BrightBlur", FBOManager::GetInstance()->GetFBO(blurFBO)->ColorBuffer, 0);
		shader->SetTexture("_RenderTexture", FBOManager::GetInstance()->GetFBO(originFBO)->ColorBuffer, 1);
		RenderAPI::GetInstance()->Draw();
		return BloomBlend;
	}
}