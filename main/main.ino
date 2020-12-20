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

//Number of millis between send events (Note: Adafruit has a cap of 30 calls per minute)
const unsigned int TIME_PERIOD_MILLIS = 60*1000; 

//Load cell needs some time to calibrate 
const unsigned int LOAD_CELL_STARTUP_TIME = 2000; 

//Calibration factor for load cell 
// To set reset calibration for your load cells do the following:
//    1. Set LOAD_CELL_CALIBRATION_FACTOR = 1, and define #SIMPLE mode in loop() below  
//    2. Place a known load on your device and make note of the reading (average over ~10-20 readings, may be positive or negative)
//    3. Set  LOAD_CELL_CALIBRATION_FACTOR = <AVERAGE_READING>/<KNOWN_WEIGHT> and go back to running in #NORMAL mode inside of loop()
//const float LOAD_CELL_CALIBRATION_FACTOR = 1.0;
const float LOAD_CELL_CALIBRATION_FACTOR = -99650.0/10.035442;

const char* FEED_NAME = "patooti-scale";

/*************************************/

long time; 

//Using pins 4&5 for data communication on arduino 
HX711_ADC LoadCell(4,5);

AdafruitIO_Feed *feed = io.feed(FEED_NAME);
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
  //simple mode : Just send raw data every 2 seconds. 
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
