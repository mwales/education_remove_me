#include "Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>


#include <execinfo.h>

// Directly from backtrace manpage
void printBacktrace(void)
{
   int j, nptrs;
#define SIZE 100
    void *buffer[1000];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);

}

// Changing this constant also requires changing the length of text fields in both constructors
const int Logger::FILE_NAME_PRINTED_LENGTH = 19;

Logger::Logger(enum LogLevel logLevel):
   _level(logLevel)
{
   // Empty block to make it align with other log messages
   _oss << "[                    ]";

   _timestamp = SDL_GetTicks();
   _oss << " " << _timestamp << ":";
}

Logger::Logger(char const * filename, int lineNumber, enum LogLevel logLevel):
   _level(logLevel)
{
   char buf[30];

   // If filename over 20 characters, only print the last characters
   size_t nameLen = strlen(filename);
   if (nameLen > FILE_NAME_PRINTED_LENGTH)
   {
      filename += nameLen - FILE_NAME_PRINTED_LENGTH;
   }

   snprintf(buf, sizeof(buf), "[%20s:%4d]", filename, lineNumber);
   _oss << buf;

   _timestamp = SDL_GetTicks();
   _oss << " " << _timestamp << ":";
}

Logger::~Logger()
{
   /// @todo One day in the far off future, convert this to calling a singleton that could ensure messages don't
   /// get smashed together from multiple threads and what not

   switch(_level)
   {
      case LOG_LEVEL_DEBUG:
         std::cout << "\x1b[32m" << _oss.str() << "\x1b[0m" << std::endl;
         break;

      case LOG_LEVEL_WARNING:
         std::cout << "\x1b[33m" << _oss.str() << "\x1b[0m" << std::endl;
         break;

      default: // FATAL
         std::cout << "\x1b[41m\x1b[30m" << _oss.str() << "\x1b[0m" << std::endl;

         printBacktrace();
         SDL_Quit();
         exit(0);
   }
}
