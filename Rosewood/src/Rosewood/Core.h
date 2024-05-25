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

#define BIT(x) (1 << x)