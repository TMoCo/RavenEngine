#include "AnimationCurve.h"
#include "Math/MathUtils.h"

namespace Raven
{

    AnimationCurve AnimationCurve::Linear(float timeStart, float valueStart, float timeEnd, float valueEnd)
    {
		AnimationCurve curve;
		float tangent = (valueEnd - valueStart) / (timeEnd - timeStart);
		curve.AddKey(timeStart, valueEnd, tangent, tangent);
		curve.AddKey(timeEnd, valueEnd, tangent, tangent);
		return curve;
    }

    void AnimationCurve::AddKey(float time, float value, float inTangent, float outTangent)
    {
		keys.push_back({ time, value, inTangent, outTangent });
    }

    float AnimationCurve::Evaluate(float time) const
    {
		if (keys.empty())
		{
			return 0;
		}

		const auto& back = keys.back();
		if (time >= back.time)
		{
			return back.value;
		}

		for (int i = 0; i < keys.size(); ++i)
		{
			const auto& key = keys[i];

			if (time < key.time)
			{
				if (i == 0)
				{
					return key.value;
				}
				else
				{
					return Evaluate(time, keys[i - 1], key);
				}
			}
		}

		return 0;
    }

    float AnimationCurve::Evaluate(float time, const Key& k0, const Key& k1)
    {

		//result.Time = k0.time + (k1.time - k0.time) * alpha;
		//AnimationUtils::Interpolate(a.Value, b.Value, alpha, result.Value);
		float dt = std::abs((time - k0.time) / (k1.time - k0.time));
		return MathUtils::Lerp(k0.value, k1.value, dt, false);
    }
};