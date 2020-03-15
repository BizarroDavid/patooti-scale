#include <Arduino.h>
#include "Stopwatch.h"

void Stopwatch::Start(){
    if( !_isStarted ) {
        _isStarted = true; 
        _startTimeMillis = millis();
        _latestTimeMillis = millis(); 
    }
}

void Stopwatch::Stop(){
    if( _isStarted ) {
        _isStarted = false;
        _latestTimeMillis = millis(); 
    }
}

void Stopwatch::Reset(){
    _startTimeMillis = millis();
    _latestTimeMillis = millis(); 
} 

unsigned long Stopwatch::Elapsed(){        
    if( _isStarted ) {
        _latestTimeMillis = millis();
    }    
    return _latestTimeMillis - _startTimeMillis;
} 