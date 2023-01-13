#pragma once
#include "Timer.h"

namespace Ikigai {
	class DeltaTime
	{
	public:
		DeltaTime(float time) : m_Time(time) {}

		operator float() const
		{
			return m_Time;
		}
	private:
		float m_Time;
	};
}
