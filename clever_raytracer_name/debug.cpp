#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "debug.hpp"

#ifdef DEBUG_LOG

//USE GETLOG FUNCTION TO GAIN ACCESS
Debug *Debug::debugLog = NULL;

Debug * Debug::getLog()
{
    if(debugLog == NULL)
    {
        atexit(Debug::deleteLog);
        
        debugLog = new Debug();
    }
    return(debugLog);
}

void Debug::deleteLog(void)
{
    if(debugLog)
    {
        delete debugLog;
        debugLog = NULL;
    }
}

void Debug::log(const char *file, long line, const char *function, const char *fmt, ...)
{
    FILE *ptr = getFilePtr();
    
    va_list ap;
    va_start(ap, fmt);
    vfprintf(ptr, fmt, ap);
    va_end(ap);
    
#ifdef DEBUG_LOG_FILE_INFO
    fprintf(ptr, " [file: %s, line: %ld, function: %s]\n", file, line, function);
#else
    fprintf(ptr, "\n");
#endif
    
    fflush(ptr);
    
}

Debug::Debug()
{
    logFilePtr = createFilePtr();
}

Debug::~Debug()
{
    if(logFilePtr)
    {
        fflush(logFilePtr);
        fclose(logFilePtr);
    }
}

FILE * Debug::createFilePtr()
{
    char outputFilename[1024];
    
    //FIX: CHANGE 0 TO GETMPIRANK()
    snprintf(outputFilename, sizeof(outputFilename), "log_%d_%ld_.txt", 0, pthread_self());
    FILE *ptr = fopen(outputFilename, "w");
    
    if(ptr == NULL)
    {
        //FIX: CHANGE 0 TO GETMPIRANK()
        snprintf(outputFilename, sizeof(outputFilename), "log_%d_%ld_d.txt", 0, pthread_self(), rand());
        ptr = fopen(outputFilename, "w");
        
        if(ptr == NULL)
        {
            fprintf(stderr, "Can't open debug file %s!\n", outputFilename);
            exit(1);
        }
        
    }
    
    return(ptr);
    
}

void getMemorySize(unsigned long &size, unsigned long &rss)
{
    size = 0;
    rss = 0;
    
    FILE *file = fopen("/proc/self/statm", "r");
    if(file)
    {
        int num = fscanf(file, "%lu%lu", &size, &rss);
        if(num == 2)
        {
            num = getpagesize();
            size *= (unsigned long)num;
            rss *= (unsigned long)num;
        }
        
        fclose(file);
    }
    
}

#endif //DEBUG_LOG
