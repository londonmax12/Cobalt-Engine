#pragma once

namespace Ikigai {
	class Debug {
	public:
		static void SetFrameRate(int frameRate) { m_FrameRate = frameRate; }
		static int GetFrameRate() { return m_FrameRate; }
	private:
		inline static int m_FrameRate;
	};
}