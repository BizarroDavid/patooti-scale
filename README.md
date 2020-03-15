
# Getting started 

- Make sure you define a secrets.h file with the following info 
```
#define SECRETS__IO_USERNAME "xxxxx"
#define SECRETS__IO_KEY "xxxxxxxxxx"
#define SECRETS__WIFI_SSID "xxxxxxx"
#define SECRETS__WIFI_PASS "xxxxxxx"
```


# Getting VSCode intellisense to work 

See the last section here: https://github.com/Microsoft/vscode-arduino/issues/765. You need to make sure VSCode recognizes your headers, you will need to update c_cpp_properties.json.

# How to add a new SSL cert to the Arduino MKR1000

- load the firmware update sketch from the WiFI samples (in the Arduino IDE)
- in the tools meny open the update prompt. You can add certificates from here. 

# todo 

- write destructors 