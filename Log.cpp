/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "Log.hpp"
#include <string>
#include <iostream>

Log::ptr Log::self = nullptr;


Log::Log()
{
	char logFile[] = "./chess.log";
	
	logStream.open(logFile);
}

Log::ptr Log::getInstance()
{
	if(!self)
	{
		self = std::shared_ptr<Log>(new Log);
	}
	return self;
}

constexpr const char* to_string(Log::LogSeverity severity)
{
	return
		severity == Log::INFO ? "[INFO]" :
		severity == Log::NORMAL ? "[NORMAL]" :
		severity == Log::HIGH ? "[HIGH]" :
		severity == Log::ERROR ? "[ERROR]" :
		"[UNKNOWN SEVERITY]";
}

void Log::log(Log::LogSeverity severity, std::string text)
{
	logStream << to_string(severity) << ' ' << text << std::endl;
}

void Log::info(std::string text)
{
	getInstance()->log(Log::INFO, text);
	//std::cerr << "[INFO] " << text << std::endl;
}