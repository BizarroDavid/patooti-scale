#ifndef ANTI_STOPWATCH_H
#define ANTI_STOPWATCH_H

/// Use the Stopwatch to keep track of elapsed time
class Stopwatch{
    private:
        bool _isStarted = false; 
        unsigned long _startTimeMillis = 0; 
        unsigned long _latestTimeMillis = 0; 

    public:
        /// Start timing 
        void Start();

        /// Stop timing 
        void Stop();

        /// Reset counters to 0
        void Reset(); 

        /// Return the number of millis since Start() was called.
        /// If the stop watch is stopped it will return the millis 
        /// between calls to Start() and Stop();
        unsigned long Elapsed(); 

    
};
#endif 