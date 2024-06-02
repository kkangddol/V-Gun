#pragma once
#include <memory>
#include <windows.h>
#include <string>
#include <iostream>
#include <stdarg.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace RocketUtil
{
	class Log
	{
	public:
		static void Initialize();
		void Finalize();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static void CreateConsoleWindow();

		static std::shared_ptr<spdlog::logger> s_Logger;

		static CONSOLE_SCREEN_BUFFER_INFO m_ConsoleInfo;
		static HANDLE m_stdioHandle;
		static int m_ConsoleHandleR;
		static int m_ConsoleHandleW;
		static FILE* m_ConsoleFilePtr;
	};
}

#define DEBUG_ERROR(...) ::RocketUtil::Log::GetLogger()->error(__VA_ARGS__);
#define DEBUG_WARN(...) ::RocketUtil::Log::GetLogger()->warn(__VA_ARGS__);
#define DEBUG_INFO(...) ::RocketUtil::Log::GetLogger()->info(__VA_ARGS__);
#define DEBUG_TRACE(...) ::RocketUtil::Log::GetLogger()->trace(__VA_ARGS__);


