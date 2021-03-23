//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>

namespace Raven
{
	namespace MathUtils 
	{
		template <class T>
		inline bool Equals(T lhs, T rhs, T eps = 0.000001f) { return lhs + eps >= rhs && lhs - eps <= rhs; }
	};
};
