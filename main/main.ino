#include <HX711_ADC.h>
#include <AdafruitIO.h>
#include "LoadCell_MaxInTimeWindow.h"
#include "config.h"
#include "CircularBuffer.h"

// ^--- How to add header files and make them work with VSCode: 
//      edit c_cpp_properties.json. Find your header file and add the path in the 
//      "includePath" section. If this doesn't work add the header file in the forcedInclude section. 


/***   CONFIGURATION PARAMETERS ****/

//send data up to adafruit?
const bool SEND_TO_ADAFRUIT=true; 

//Number of samples to average over. (Note we run about once everyone 100ms)
const unsigned int BUFFER_SIZE = 10; 

//Number of millis between "events" (Note: Adafruid has a cap of 30 calls per minute)
const unsigned int TIME_PERIOD_MILLIS = 2000; 

//Load cell needs some time to calibrate 
const unsigned int LOAD_CELL_STARTUP_TIME = 2000; 

//calibration factor for load cell 
const float LOAD_CELL_CALIBRATION_FACTOR = 261452. / 2.5;

/*************************************/

long time; 
HX711_ADC LoadCell(4,5);
AdafruitIO_Feed *feed = io.feed("patooti-scale");
LoadCell_MaxInTimeWindow* maxWeightCalculator; 
CircularBuffer<float> buffer; 
Stopwatch stopwatch; 

void setup() {    
  Serial.print("Patooti Scale v1.0.0");
  Serial.print("Starting up...");
  LoadCell.begin();
  LoadCell.start(LOAD_CELL_STARTUP_TIME);  
  LoadCell.setCalFactor(LOAD_CELL_CALIBRATION_FACTOR);

  buffer.Init(BUFFER_SIZE);

  // connect to io.adafruit.com
  io.connect();  
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());

  maxWeightCalculator = new LoadCell_MaxInTimeWindow(&LoadCell, BUFFER_SIZE, TIME_PERIOD_MILLIS, 
    &buffer, &stopwatch);
}



void loop() {

  io.run();

  #define NORMAL 
  #ifdef NORMAL 
  maxWeightCalculator->Update(); 
  if( maxWeightCalculator->HasEvent()) {
    float raw = maxWeightCalculator->GetEvent();     
    Serial.print(raw);
    Serial.print("\n");    

    if( SEND_TO_ADAFRUIT ) {
      feed->save(raw);
    }
  }
  #endif

  //#define SIMPLE 
  #ifdef SIMPLE 
  LoadCell.update();  
  if( millis() > time + 2000 ) {
    float i = LoadCell.getData();
    Serial.print(i);
    Serial.print("\n");

    if( SEND_TO_ADAFRUIT ) {
      feed->save(i);
    }
    time = millis(); 
  }
  #endif

}
