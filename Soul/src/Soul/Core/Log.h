#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Soul
{
	class SOUL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define SL_CORE_TRACE(...)   ::Soul::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SL_CORE_INFO(...)    ::Soul::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SL_CORE_WARN(...)    ::Soul::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SL_CORE_ERROR(...)   ::Soul::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SL_CORE_FATAL(...)   ::Soul::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define SL_TRACE(...)        ::Soul::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SL_INFO(...)         ::Soul::Log::GetClientLogger()->info(__VA_ARGS__)
#define SL_WARN(...)         ::Soul::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SL_ERROR(...)        ::Soul::Log::GetClientLogger()->error(__VA_ARGS__)
#define SL_FATAL(...)        ::Soul::Log::GetClientLogger()->fatal(__VA_ARGS__)