#pragma once


#ifdef SL_PLATFORM_WINDOWS
	#ifdef SL_BUILD_DLL
		#define SOUL_API __declspec(dllexport)
	#else
		#define SOUL_API __declspec(dllimport)
	#endif
#else
	#error Soul only support Windows
#endif

#define BIT(x) (1 << x)