#include <HX711_ADC.h>
#include "Stopwatch.h"
#include "CircularBuffer.h"
/// Used to calculate the maximum value over a non-sliding time window.
/// This is used to generate "periodic updates" i.e., to keep from flooding a receiver with too much data. 
/// e.g., if you will broad cast this over the network.
///
/// 
class LoadCell_MaxInTimeWindow{
    private: 
        HX711_ADC* _loadCell;
        CircularBuffer<float>* _buffer; 
        Stopwatch* _stopwatch;
        unsigned int _averageWindowSamples; 
        unsigned int _timePeriodMillis;         

        float _maxWindowSum = 0;                         
        bool _hasEvent = false; 
        float _eventWindowSum = 0;
        unsigned long _iteration;


    public:
        /// Initalize the object.         
        ///
        ///   HX711_ADC& loadCell is your initalized HX711_ADC load cell.
        ///
        ///   averageWindowSamples is used to smooth your signal. It's the sliding window 
        ///       (in number of samples) that the signal will be averaged over. 
        ///
        ///   timePeriodMillis is the time period in milliseconds. Every `timePeriodMillis` you will get a new 
        ///        event that consists the maximum value over this time period. 
        /// 
        ///   stopwatch is your Stopwatch object. 
        LoadCell_MaxInTimeWindow(HX711_ADC* loadCell, unsigned int averageWindowSamples, unsigned int timePeriodMillis, 
        CircularBuffer<float>* buffer, Stopwatch* stopwatch );
        
        
        /// The update method. This should be called every duty cycle. 
        void Update(); 

        /// Returns true if there is a new event to report. Typically called right after Update()
        /// Call GetEvent to reset HasEvent()
        bool HasEvent(); 

        /// Get the last event. If HasEvent() is true it is reset to false after this call.
        float GetEvent(); 
};