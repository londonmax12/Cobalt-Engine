#pragma once

#include <xhash>

namespace Ikigai {
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std {

	template<>
	struct hash<Ikigai::UUID>
	{
		std::size_t operator()(const Ikigai::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}