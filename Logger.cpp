#include "pch.h"
#include "Logger.h"

Logger::Logger()
= default;

void Logger::Log(const LogType& type, const std::string& header, const std::string& str, const int numbering1, const int numbering2)
{

	std::time_t t = std::time(nullptr);
	std::tm time;
	localtime_s(&time, &t);

	std::string times = std::to_string(time.tm_hour) + ":" + std::to_string(time.tm_min) + ":" + std::to_string(time.tm_sec);
	std::string numbers1 = ((numbering1 >= 0) ? std::to_string(numbering1) : "  ");
	std::string numbers2 = ((numbering2 >= 0) ? std::to_string(numbering2) : "  ");


	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << std::right << "|-" << std::setfill('0') << std::setw(2) << std::to_string(time.tm_hour) << ":" << std::setw(2) << std::to_string(time.tm_min)
		<< ":" << std::setw(2) << std::to_string(time.tm_sec) << "-| " << std::setfill(' ') << std::left;
	std::string types = GetType(type);

	std::cout << std::setw(10) << "[" + types + "]";
	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << " <-> ";
	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_BLUE);
	std::cout << std::setw(17) << "[" + header + "]";
	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << " |-";
	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "[" << std::right << std::setw(2) << std::setfill(' ') << numbers1 << "]";
	std::cout << "[" << std::right << std::setw(2) << std::setfill(' ') << numbers2 << "]";
	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "-| -> ";
	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN);
	std::cout << str << std::endl;

	SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

std::string Logger::GetType(LogType type)
{
	std::string res;
	switch (type)
	{
	case LogType::INFO:
		res = "INFO";
		SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_BLUE | FOREGROUND_RED);
		break;
	case LogType::FAILED:
		res = "FAILED";
		SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_RED);
		break;
	case LogType::SUCCESS:
		res = "SUCCESS";
		SetConsoleTextAttribute(Logger::m_consoleHandle, FOREGROUND_GREEN);
		break;
	}

	return res;
}
