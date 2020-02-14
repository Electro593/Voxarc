#include <ctime>
#include <iostream>
#include <string>
#include <cstdarg>

#include "logging.h"

namespace Voxarc {

Logger::Logger(const char *filePath)
    : filePath(filePath) { }

void Logger::restart()
{
    FILE *file = fopen(filePath, "w");
    if(!file)
    {
        fprintf(stderr, "Could not open file %s for write\n", filePath);
        exit(EXIT_FAILURE);
    }
    
    time_t now = time(nullptr);
    char *date = ctime(&now);
    
    fprintf(file, "Local Time: %s\n", date);
    
    fclose(file);
}

void Logger::log(const char *message, va_list args)
{
    FILE *file = fopen(filePath, "a");
    if(!file)
    {
        fprintf(stderr, "Could not open file %s for append\n", filePath);
        exit(EXIT_FAILURE);
    }
    
    vfprintf(file, message, args);
    
    fclose(file);
}

void Logger::logDefault(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    log(message, args);
    va_end(args);
}

void Logger::logInfo(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    std::string newMessage;
    newMessage.append("INFO: ");
    newMessage.append(message);
    log(newMessage.c_str(), args);
    va_end(args);
}

void Logger::logError(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    std::string newMessage;
    newMessage.append("ERROR: ");
    newMessage.append(message);
    log(newMessage.c_str(), args);
    va_end(args);
}

}