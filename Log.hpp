/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef LOG__
#define LOG__

#include <memory>
#include <fstream>

class Log
{
public:
	typedef std::shared_ptr<Log> ptr;
private:
	std::ofstream logStream;
	Log();
	static ptr self;
public:
	Log(const Log&) = delete;
	enum LogSeverity
	{
		INFO, NORMAL, HIGH, ERROR
	};
	static ptr getInstance();
	void log(LogSeverity severity, std::string text);
	static void info(std::string text);
};

#endif