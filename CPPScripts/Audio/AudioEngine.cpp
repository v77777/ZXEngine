#include "AudioEngine.h"
#include "AudioStream.h"
#include "AudioClip.h"
#include "../GameObject.h"

namespace ZXEngine
{
	AudioEngine* AudioEngine::mInstance = nullptr;

	void AudioEngine::Create()
	{
		if (mInstance == nullptr)
		{
			mInstance = new AudioEngine();
		}
	}

	AudioEngine* AudioEngine::GetInstance()
	{
		return mInstance;
	}

	AudioEngine::AudioEngine()
	{
		mEngine = irrklang::createIrrKlangDevice();

		if (!mEngine)
		{
			Debug::LogError("Could not startup the audio engine.");
		}
	}

	AudioEngine::~AudioEngine()
	{
		for (auto& iter : mAudioStreams)
			delete iter.second;

		mEngine->drop();
	}

	void AudioEngine::SetAllPause(bool pause)
	{
		for (auto& iter : mAudioClips)
		{
			iter->SetPause(pause);
		}
	}

	void AudioEngine::SetListener(GameObject* listener)
	{
		if (mListener)
		{
			Debug::LogWarning("There is already a listener in the scene.");
		}
		else
		{
			mListener = listener;
		}
	}

	void AudioEngine::RemoveListener(GameObject* listener)
	{
		if (mListener == listener)
		{
			mListener = nullptr;
		}
	}

	AudioClip* AudioEngine::CreateAudioClip(const string& path)
	{
		AudioStream* stream = CreateAudioStream(path);
		return new AudioClip(stream);
	}

	AudioClip* AudioEngine::CreateAudioClip(AudioStream* stream)
	{
		return new AudioClip(stream);
	}

	AudioStream* AudioEngine::CreateAudioStream(const string& path)
	{
		auto iter = mAudioStreams.find(path);
		if (iter != mAudioStreams.end())
		{
			return iter->second;
		}

		AudioStream* stream = new AudioStream(path);
		mAudioStreams[path] = stream;

		return stream;
	}
}