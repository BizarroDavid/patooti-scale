# Overview 

This project implements an IoT scale. The scale periodically uploads weight data from an Arduino MKR1000 to your Adafruit.IO [account](https://adafruit.io/). 

On start up the software will automatically tare any weight that it detects, then it will hop onto your wifi network and upload weight readings to your Adafruit.IO feed. Every minute the software will upload the maximum weight it has recorded in that time period. The time period can be adjusted in `main.ino`

*Note: Due to plotting limitations on the Adafruit.IO charts, using an upload time period of once per minute is appropriate for charts that display the last 24 hours of weight data. If you use a higher frequency you may see your data readings get agregated in unexpected ways*

# Prereqs

* You have installed the Arduino IDE 
* You have set up an account at [adafruit.io](https://adafruit.io/). 
* This repo is set to build in VSCode. Install the Arduino for Visual Studio Code [extension.](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino).  


# Hardware Setup 

This project assumes the following hardware setup: 

* Load sensors attached to an HX711_ADC 
* HX711_ADC attached to an Arduino MKR1000

The Arduino MKR1000 contains built in WiFI capabilities, however any internet enabled Arduino setup should work.

See my [blog post](https://wordpress.com/post/strangeday.press/56) for more details on the physical assembly. 

# Getting started 

## Install these Arduino libraries 

* WiFi101 / WifiNINA Firmware Updater 
* HX711_ADC by Olav Kallhovd ([link](https://github.com/olkal/HX711_ADC))
* Adafruit IO Arduino Library ([link](https://github.com/adafruit/Adafruit_IO_Arduino))

*Note: You should be able to install all of these from the Arduino library manager.*


## Upload the Adafruit.IO certificate onto your MKR1000

Follow these [instructions](https://www.arduino.cc/en/Tutorial/FirmwareUpdater) to upload the Adafruit.IO certificate onto your arduino. 

*Note: Update your Wifi firmware while you are at it.*

## Define your secrets 

Create a file called `secrets.h`in the main directory and populate it with your Wifi credenfials and your Adafruit.IO API username and key. The file should look like this:

```
#define SECRETS__IO_USERNAME "xxxxx"
#define SECRETS__IO_KEY "xxxxxxxxxx"
#define SECRETS__WIFI_SSID "xxxxxxx"
#define SECRETS__WIFI_PASS "xxxxxxx"
```

# Software Architecture 

![Software Layer Diagram](https://www.lucidchart.com/publicSegments/view/4c97a1a3-2922-46e0-92ff-906c0b2e82f6/image.png)

The software is layered as follows, starting from the lowest level: 

* CircularBuffer: The HX711 library is used to feed load sensor data into a circular buffer which is used to perform smoothing of the weight signal.

* LoadCell_MaxInTimeWindow: This layer is used to keep track of the max weight reading within a given time period. 

* Main: The main layer obtains these periodic measurements and pushed them up to your Adafruit.IO feed. 



# Notes 


## Getting VSCode intellisense to work 

See the last section here: https://github.com/Microsoft/vscode-arduino/issues/765. You need to make sure VSCode recognizes your headers, you will need to update c_cpp_properties.json.

## How to add a new SSL cert to the Arduino MKR1000

- load the firmware update sketch from the WiFI samples (in the Arduino IDE)
- In the tools menu open the update prompt. You can add certificates from here. 

