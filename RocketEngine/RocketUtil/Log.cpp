#include "Log.h"
#include <io.h>
#include <fcntl.h>


namespace RocketUtil
{
	CONSOLE_SCREEN_BUFFER_INFO Log::m_ConsoleInfo;
	HANDLE Log::m_stdioHandle = nullptr;
	int Log::m_ConsoleHandleR = 0;
	int Log::m_ConsoleHandleW = 0;
	FILE* Log::m_ConsoleFilePtr = nullptr;

	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Initialize()
	{
		CreateConsoleWindow();
		
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("Debug Tracer");
		s_Logger->set_level(spdlog::level::trace);
	}

	void Log::Finalize()
	{

	}

	void Log::CreateConsoleWindow()
	{
		BOOL res = AllocConsole();
		std::wstring strW = L"Debug Tracer";
		SetConsoleTitle(strW.c_str());

		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
		DrawMenuBar(GetConsoleWindow());

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &m_ConsoleInfo);

		m_stdioHandle = GetStdHandle(STD_INPUT_HANDLE);
		m_ConsoleHandleR = _open_osfhandle(reinterpret_cast<intptr_t>(m_stdioHandle), _O_TEXT);
		m_ConsoleFilePtr = _fdopen(m_ConsoleHandleR, "r");
		*stdin = *m_ConsoleFilePtr;
		setvbuf(stdin, NULL, _IONBF, 0);

		m_stdioHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		m_ConsoleHandleW = _open_osfhandle(reinterpret_cast<intptr_t>(m_stdioHandle), _O_TEXT);
		m_ConsoleFilePtr = _fdopen(m_ConsoleHandleW, "w");
		*stdout = *m_ConsoleFilePtr;
		setvbuf(stdout, NULL, _IONBF, 0);

		m_stdioHandle = GetStdHandle(STD_ERROR_HANDLE);
		*stderr = *m_ConsoleFilePtr;
		setvbuf(stderr, NULL, _IONBF, 0);

	}

}


