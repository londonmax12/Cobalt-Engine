#pragma once

#include <chrono>

#include "Platforms/Platform.h"

namespace Ikigai {

	class Timer
	{
	public:
		void Start()
		{
			m_StartTime = Platform::GetAbsTime();
			m_Elapsed = 0;
		}
		void Reset() {
			Start();
		}

		double Elapsed()
		{
			if (m_StartTime != 0) {
				m_Elapsed = Platform::GetAbsTime() - m_StartTime;
			}
			return m_Elapsed;
		}

		void Stop() {
			if (m_StartTime != 0) {
				m_Elapsed = Platform::GetAbsTime() - m_StartTime;
			}
			m_StartTime = 0;
		}
	private:
		double m_StartTime;
		double m_Elapsed;
	};

}