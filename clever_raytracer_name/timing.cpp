#include "timing.hpp"

#ifdef TIMER

//GLOBAL TIMING CLASS
Timing *Timing::timing = NULL;

Timing::Timing()
{
    timeline = NULL;
    timelineIndex = NULL;
    numTimeline = 0;
    numEvents = 0;
    maxEventHit = true;
    
    timeSummary = NULL;
    numUniqueEvents = 0;
}

Timing::~Timing()
{
    deleteEventMemory();
}

void Timing::deleteTiming()
{
    if(timing)
    {
        delete timing;
        timing = NULL;
    }
}

void Timing::stop(int thread_ID, const TIMER_ID start, TIMER_EVENT event_ID, const char *fmt, ...)
{
    
}

//WRITE DATA TO FILE
#define writeData() \
    for(int j=0; j < numUniqueEvents ;++j) \
    { \
        if( timeSummary[0][j].count ) \
        fprintf( timingFilePtr, "%d, %.6f, %ld\n", j, timeSummary[0][j].time, timeSummary[0][j].count ); \
    }

void Timing::saveTimingFiles(const char *filename)
{
    FILE *timingFilePtr;
    char outputFilename[1024];
    
    //SUM TIME ACROSS ALL THREADS
    for(int i = 1; i < numTimeline; ++i)
    {
        for(int j = 0; j < numUniqueEvents; ++j)
        {
            if(timeSummary[i][j].count)
            {
                timeSummary[0][j].time += timeSummary[i][j].time;
                timeSummary[0][j].count += timeSummary[i][j].count;
            }
        }
    }
    {
#if TIMER_SAVEONE
        //SAVE ALL TIMING DATA TO ONE FILE ON RANK 0
        //FIX: ADD IN HERE!!!!
    
#else
    //SAVE RANK INFO IN INDIVIDUAL FILES
    //FIX: CHANGE 0 TO GETMPIRANK()
    snprintf(outputFilename, sizeof(outputFilename), "%s_summary_%04d_.txt", filename, 0);
    timingFilePtr = fopen(outputFilename, "w");
    if(timingFilePtr == NULL)
    {
        fprintf(stderr, "Cannot open timings summary file %s!\n", outputFilename);
        return;
    }
    
    //WRITE THE DATA
    writeData();
    
    fclose(timingFilePtr);
#endif
    }
}










#endif
