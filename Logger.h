#pragma once

enum class LogType
{
	INFO = 0,
	FAILED = 1,
	SUCCESS = 2,

};

class Logger
{
	inline static std::vector<std::string> m_logs{};
	inline static bool m_isFirst = true;
	inline static HANDLE m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
private:
	Logger();
public:

	static void Log(const LogType& type, const std::string& header, const std::string& str, const int numbering1 = 0, const int numbering2 = 0);

private:
	static std::string GetType(LogType type);
	
};

