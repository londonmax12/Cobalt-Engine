#pragma once

#include <memory>
#include <functional>

#define IKIGAI_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define IKIGAI_BIT(x) (1 << x)

namespace Ikigai {
	template<typename T>
	using ScopeRef = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr ScopeRef<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}