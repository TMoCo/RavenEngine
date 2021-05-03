//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

namespace Raven
{
	class AnimationCurve
	{
	private:
		struct Key
		{
			float time;
			float value;
			float inTangent;
			float outTangent;
		};



	public:
		static AnimationCurve Linear(float timeStart, float valueStart, float timeEnd, float valueEnd);
		void AddKey(float time, float value, float inTangent, float outTangent);
		float Evaluate(float time) const;

		template<class Archive>
		void load(Archive& archive)
		{
			LoadVectorBinary(archive, keys);
		}

		// Serialization Save.
		template<class Archive>
		void save(Archive& archive) const
		{
			SaveVectorBinary(archive, keys);
		}


	private:
		static float Evaluate(float time, const Key& k0, const Key& k1);

	private:
		std::vector<Key> keys;

	};
};