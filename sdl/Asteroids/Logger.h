#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <ostream>

#define LOG_DEBUG() Logger(__FILE__, __LINE__)
#define LOG_FATAL() Logger(__FILE__, __LINE__, Logger::LOG_LEVEL_FATAL)

void LogDebug(char const * file, int lineNumber, std::string const msg);
void LogFatal(char const * file, int lineNumber, std::string const msg);

class Logger
{
public:

   enum LogLevel
   {
      LOG_LEVEL_DEBUG,
      LOG_LEVEL_WARNING,
      LOG_LEVEL_FATAL
   };

   Logger(enum LogLevel logLevel = LOG_LEVEL_DEBUG);

   Logger(char const * filename, int lineNumber, enum LogLevel logLevel = LOG_LEVEL_DEBUG);

   ~Logger();

   Logger& operator<< (std::string rhs);
   Logger& operator<< (int rhs);
   Logger& operator<< (float rhs);

protected:

   static const int FILE_NAME_PRINTED_LENGTH;

   // Logging level
   int _level;

   // output stream
   std::ostringstream _oss;

};

