//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once


#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h"
#include "AnimationCurve.h"

#include <vector>
#include <memory>
#include <string>


namespace Raven
{
	class Transform;
	class AnimationController;
	class SkeletonInstance;
	class Skeleton;

	enum class AnimationCurvePropertyType
	{
		Unknown = 0,
		LocalPositionX,
		LocalPositionY,
		LocalPositionZ,
		LocalRotationX,
		LocalRotationY,
		LocalRotationZ,
		LocalScaleX,
		LocalScaleY,
		LocalScaleZ,
		BlendShape
	};

	struct AnimationCurveProperty
	{
		AnimationCurvePropertyType type;
		std::string name;
		AnimationCurve curve;
	};

	struct AnimationCurveWrapper
	{
		//std::string path;
		int32_t index;
		std::vector<AnimationCurveProperty> properties;
	};

	enum class AnimationWrapMode
	{
		Default = 0,
		Once = 1,
		Loop = 2,
		PingPong = 4,
		ClampForever = 8,
	};

	struct AnimationClip
	{
		AnimationClip() :length(0), fps(0), wrapMode(AnimationWrapMode::Default) {}
		std::string name;
		float length;
		float fps;
		AnimationWrapMode wrapMode;
		std::vector<AnimationCurveWrapper> curves;
	};

	enum class FadeState
	{
		In,
		Normal,
		Out,
	};




	struct AnimationState
	{
		int32_t clipIndex;
		float playStartTime;
		std::vector<int32_t> targets;
		FadeState fadeState;
		float fadeStartTime;
		float fadeLength;
		float startWeight;
		float weight;
		float playingTime;
	};

	//
	class Animation : public IResource
	{
	public:
		Animation();
		virtual ~Animation() { }


		// return the resource type
		inline static EResourceType StaticGetType() noexcept { return EResourceType::RT_Animation; }

		void SetClips(const std::vector<std::shared_ptr<AnimationClip>>& clips);
		void AddClip(const Ptr<AnimationClip>& clip);
		const std::string& GetClipName(int32_t index) const;
		float GetClipLength(int32_t index) const;
		float GetCurrentClipLength() const;
		int32_t GetPlayingClip() const;
		float GetPlayingTime() const;
		void SetPlayingTime(float time);

	
		inline auto IsPaused() const { return paused; }
		inline auto IsStarted() const { return started; }
		inline auto IsPlaying() const { return !paused && !stopped; }
		inline auto GetStates() const { return states.size(); }

		inline auto IsStopped() const { return stopped; }
		inline auto GetClipCount() const { return clips.size(); }
		inline auto GetTime() const { return time; }

		void Play(int32_t index, SkeletonInstance* inSkeletonInstance, float fadeLength = 0.3f);
		void Stop();
		void Pause();

		void OnUpdate(float dt);

		inline void SetSkeleton(Ptr<Skeleton> inSkeleton) { skeleton = inSkeleton; }

	private:
		void UpdateTime(float dt);
		void Sample(AnimationState & state, float time, float weight, bool firstState, bool lastState);

	private:
		std::vector<std::shared_ptr<AnimationClip>> clips;
		std::vector<AnimationState> states;
		float time = 0;
		float seekTo = -1;
		bool paused = false;
		bool stopped = true;	
		bool started = false;

		// The skeleton this animation reference.
		Ptr<Skeleton> skeleton;

		// The skeleton this animation currently updating.
		SkeletonInstance* skeletonInstance;
	};

}