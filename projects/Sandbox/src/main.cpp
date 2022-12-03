#include "Ikigai.h"

void main() {
	Ikigai::Logging::Init();
	IKIGAI_TRACE("This is a {} log", "Trace");
	IKIGAI_DEBUG("This is a {} log", "Debug");
	IKIGAI_INFO("This is an {} log", "Info");
	IKIGAI_WARN("This is a {} log", "Warning");
	IKIGAI_ERROR("This is an {} log", "Error");
	IKIGAI_FATAL("This is a {} log", "Fatal");

	IKIGAI_DEBUG("");
	IKIGAI_DEBUG("This is a Vector2: {}", Ikigai::Vector2(1.0f, 3.0f));
	IKIGAI_DEBUG("This is a Vector3: {}", Ikigai::Vector3(1.0f, 3.0f, 1.4f) * 3);
	IKIGAI_DEBUG("This is a Vector4: {}", Ikigai::Vector4(1.0f, 3.0f, 1.4f, 5.0f));
	IKIGAI_DEBUG("This is an EulerAngle: {}", Ikigai::EulerAngle(1.0f, 3.0f, 1.4f));

	Ikigai::Vertex3D point{};
	point.Position.x = 1.2f;
	point.Position.y = 5.2f;
	point.Position.z = 8.3f;

	IKIGAI_DEBUG("");
	IKIGAI_DEBUG("This is a 3D Vertex: {}", point);
}