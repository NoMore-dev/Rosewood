#pragma once

#include "rwpch.h"
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Rosewood 
{

	class RW_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

};

// Core log macros
#define RW_CORE_CRITICAL(...)	::Rosewood::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define RW_CORE_ERROR(...)		::Rosewood::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RW_CORE_WARN(...)		::Rosewood::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RW_CORE_INFO(...)		::Rosewood::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RW_CORE_DEBUG(...)		::Rosewood::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define RW_CORE_TRACE(...)		::Rosewood::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define RW_CRITICAL(...)		::Rosewood::Log::GetClientLogger()->critical(__VA_ARGS__)
#define RW_ERROR(...)			::Rosewood::Log::GetClientLogger()->error(__VA_ARGS__)
#define RW_WARN(...)			::Rosewood::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RW_INFO(...)			::Rosewood::Log::GetClientLogger()->info(__VA_ARGS__)
#define RW_DEBUG(...)			::Rosewood::Log::GetClientLogger()->debug(__VA_ARGS__)
#define RW_TRACE(...)			::Rosewood::Log::GetClientLogger()->trace(__VA_ARGS__)