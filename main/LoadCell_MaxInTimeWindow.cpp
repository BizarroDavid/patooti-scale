#include "LoadCell_MaxInTimeWindow.h"
#include "Stopwatch.h"

LoadCell_MaxInTimeWindow::LoadCell_MaxInTimeWindow(HX711_ADC* loadCell,
unsigned int averageWindowSamples, unsigned int timePeriodMillis, CircularBuffer<float>* buffer, Stopwatch* stopwatch )
{   
    _loadCell = loadCell;
    _stopwatch = stopwatch; 
    _averageWindowSamples = averageWindowSamples;
    _timePeriodMillis = timePeriodMillis;
    _buffer = buffer;
    _maxWindowSum = 0; 
    _iteration = 0;

    _stopwatch->Reset();
    _stopwatch->Start();     
}


void LoadCell_MaxInTimeWindow::Update(){
    _iteration++; 
    unsigned long elapsedTime = _stopwatch->Elapsed(); 
    
    _loadCell->update();    
    float data = _loadCell->getData();
    _buffer->Push(data);    
    _maxWindowSum = max( _maxWindowSum, _buffer->Sum());

    if( elapsedTime >= _timePeriodMillis ){        
        _hasEvent = true; 
        _eventWindowSum = _maxWindowSum; 
        _maxWindowSum = 0; 
        _stopwatch->Reset(); 
    }       

//#define DO_LOGGING
#ifdef DO_LOGGING 
    Serial.println("---");
    Serial.print("_iteration = ");
    Serial.print(_iteration); Serial.println("");    

    Serial.print("_buffer->Sum() = ");
    Serial.print(_buffer->Sum()); Serial.println("");    

    Serial.print("data = ");
    Serial.print(data); Serial.println("");    

    Serial.print("_maxWindowSum = ");
    Serial.print(_maxWindowSum); Serial.println("");    

    Serial.print("_hasEvent = ");
    Serial.print(_hasEvent); Serial.println("");

    Serial.print("_eventWindowSum = ");
    Serial.print(_eventWindowSum); Serial.println("");

    Serial.print("elapsedTime = ");
    Serial.print(elapsedTime); Serial.println("");    

    Serial.print("System time =");
    Serial.print(millis()); Serial.println("");
#endif    
}

bool LoadCell_MaxInTimeWindow::HasEvent(){
    return _hasEvent;
}
float LoadCell_MaxInTimeWindow::GetEvent(){
    _hasEvent = false;     
    return (float) _eventWindowSum / (float) _averageWindowSamples;
}
