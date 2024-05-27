#pragma once

#ifdef RW_PLATFORM_WINDOWS
	#ifdef RW_BUILD_DLL
		#define RW_API __declspec(dllexport)
	#else
		#define RW_API __declspec(dllimport)
	#endif
#else
	#error Rosewood only supports windows !
#endif

#ifdef RW_ENABLE_ASSERTS
	#define RW_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RW_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RW_ASSERT(x, ...)
	#define RW_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RW_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)