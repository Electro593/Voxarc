#ifndef VOX_LOGGING_H_
#define VOX_LOGGING_H_

namespace Voxarc {

class Logger
{
private:
    const char *filePath;
public:
    Logger(const char *filePath);
    
    void restart();
    void log(const char *message, va_list args);
    void logDefault(const char *message, ...);
    void logInfo(const char *message, ...);
    void logError(const char *message, ...);
};

}

#endif