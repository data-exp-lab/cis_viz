#ifndef __DEBUGGING__
#define __DEBUGGING__

#ifdef DEBUG_LOG

#include <stdio.h>
#include <map>
#include <pthread.h>

class Debug
{
public:
    static Debug * getLog();
    
    void log(const char *file, long line, const char *function, const char *fmt, ...);
    
private:
    Debug();
    ~Debug();
    
    static void deleteLog(void);
    
    FILE *createFilePtr();
    
private:
    static Debug * debugLog;
    
    FILE *logFilePtr;
    long pthread_self()
    {
        return(0);
    }
#define getFilePtr() logFilePtr
};

#define debug(fmt, ...) Debug::getLog()->log(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define debugIf(cond, fmt, ...) if(const){ Debug::getLog()->log(FILE, LINE, func, fmt, ##__VA_ARGS__); }

void getMemorySize(unsigned long &size, unsigned long &rss);
#define debugMemory() unsigned long size, rss; getMemorySize(size, rss); debug("Memory Size: size: %lu rss: %lu", size, rss);

#else
#define debug(fmt, ...)
#define debugIf(cond, fmt, ...)
#define debugMemory()

#endif //DEBUG_LOG


#endif //DEBUGGING
