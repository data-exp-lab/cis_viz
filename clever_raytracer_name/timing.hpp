#ifndef __TIMING__
#define __TIMING__

#if defined(TIMER_SUMMARY)
    #define TIMER
#endif

#ifdef TIMER

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include <bitset>

typedef timeval TIMER_ID;
typedef unsigned int TIMER_EVENT_CAST;
typedef unsigned char TIMER_EVENT;

class Timing
{
public:
    
    static void init(int numThreads, int numEvents, int uniqueEvents)
    {
        if(timing == NULL)
        {
            atexit(Timing::deleteTiming);
            
            timing = new Timing();
            timing->restartTiming(numThreads, numEvents, uniqueEvents);
        }
    }
    
    static Timing * getTiming()
    {
        return (timing);
    }
    
    inline void restartTiming(int numThreads, int numEvents, int uniqueEvents)
    {
        timing->createEventMemory(numThreads, numEvents, uniqueEvents);
        timing->setEpoc();
    }
    
    inline static void getTime(TIMER_ID &current)
    {
        gettimeofday(&current, 0);
    }
    
    inline bool overTime(const TIMER_ID start, long int sec)
    {
        TIMER_ID current;
        gettimeofday(&current, 0);
        if((current.tv_sec - start.tv_sec) > sec)
        {
            return 1;
        }
        return 0;
    }
    
    inline bool epocOverTime(long int sec)
    {
        return(overTime(epoc, sec));
    }
    
    void stop(int thread_ID, const TIMER_ID start, TIMER_EVENT event_ID, const char *fmt, ...);
    void stop(int thread_ID, const TIMER_ID start, TIMER_EVENT event_ID)
    {
        TIMER_ID current;
        gettimeofday(&current, 0);
        
        double timeInterval = delta(start, current);
        
        timeSummary[thread_ID][(TIMER_EVENT_CAST)event_ID].time += timeInterval;
        ++(timeSummary[thread_ID][(TIMER_EVENT_CAST)event_ID].count);
    }
    
    void counter(int thread_ID, TIMER_EVENT event_ID, long count)
    {
        timeSummary[thread_ID][(TIMER_EVENT_CAST)event_ID].time += count;
        ++(timeSummary[thread_ID][(TIMER_EVENT_CAST)event_ID].count);
    }
    
    inline double restart(TIMER_ID &start) const
    {
        TIMER_ID current;
        gettimeofday(&current, 0);
        
        double elapsed = delta(start, current);
        
        start = current;
        return(elapsed);
    }
    
    inline double elapsed(const TIMER_ID start) const
    {
        TIMER_ID current;
        gettimeofday(&current, 0);
        
        return(delta(start, current));
    }
    
    void saveTimingFiles(const char *filename);
    
private:
    
    Timing();
    ~Timing();
    
    static void deleteTiming();
    
    inline double delta(const TIMER_ID &t1, const TIMER_ID &t2) const
    {
        TIMER_ID r;
        
        r.tv_sec = t2.tv_sec - t1.tv_sec;
        r.tv_usec = t2.tv_usec - t1.tv_usec;
        
        if(r.tv_usec < 0)
        {
            --r.tv_sec;
            r.tv_usec += 1000000;
        }
        
        return(r.tv_sec + (r.tv_usec / 1000000.0));
    }
    
    struct timeEvent
    {
        TIMER_ID startTime;
        TIMER_ID endTime;
        TIMER_EVENT event;
    };
    
    struct TimeSummary
    {
        double time;
        unsigned long count; //NUMBER OF TIMES IT'S CALLED
    };
    
    inline void deleteEventMemory()
    {
        if(timeline)
        {
            delete [] timeline[0];
            delete [] timeline;
        }
        if(timelineIndex)
        {
            delete [] timelineIndex;
        }
        if(timeSummary)
        {
            delete [] timeSummary[0];
            delete [] timeSummary;
        }
    }
    
    void createEventMemory(int numThreads, int numEvents, int uniqueEvents)
    {
        deleteEventMemory();
        
        numUniqueEvents = uniqueEvents + 1;
        
        timeSummary = new TimeSummary*[numThreads];
        timeSummary[0] = new TimeSummary[numThreads * numUniqueEvents];
        
        memset(timeSummary[0], 0, (sizeof(TimeSummary) * numThreads * numUniqueEvents));
        
       for(int i = 1; i < numThreads; ++i)
       {
           timeSummary[i] = timeSummary[i - 1] + numUniqueEvents;
       }
        
       numTimeline = numThreads;
       //numEvents = numEvents;
    }
        
    void setEpoc()
   {
       gettimeofday(&epoc, 0);
   }

private:

   static Timing *timing;
   TIMER_ID epoc;
        
   timeEvent **timeline;
   int *timelineIndex;
   int numTimeline;
   int numEvents;
   bool maxEventHit;

   TimeSummary **timeSummary;
   int numUniqueEvents;
};

#define initTimer(numThreads, numEvents, uniqueEvents) \
    Timing::init(numThreads, numEvents, uniqueEvents);

#define startTimer(TIMER) TIMER_ID TIMER; Timing::getTime(TIMER)

#define endTimer(thread_ID, TIMER, event_ID, fmt, ...) Timing::getTiming()->stop(thread_ID, TIMER, event_ID)

#define counterTimer(thread_ID, event_ID, count, ftm, ...) Timing::getTiming()->Counter(thread_ID, event_ID, count)

#define saveTimers(FILENAME) Timing::getTiming()->saveTimingFiles(FILENAME)

#define overTime(TIMER, SEC) Timing::getTiming()->overTime(TIMER, SEC)
#define epocOverTime(SEC) Timing::getTiming()->epocOverTime(SEC)
#else
#define initTimer(numThreads, numEvents, uniqueEvents)

#define startTimer(TIMER)
#define endTimer(thread_ID, TIMEr, event_ID, fmt, ...)

#define counterTimer(thread_ID, event_ID, count, fmt, ...)

#define saveTimers(FILENAME)

#define overTime(TIMER, SEC) 0
#define epocOverTime(SEC) 0

#endif






#endif //__TIMING__

