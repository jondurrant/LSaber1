# LSaber1
IOT Saber Wall Light based on Raspberry PI Pico, ESP-01S and WS2812 LEDs.

PIR and Switch activated wall light. Works as a group able to turn each other on and off via MQTT hub topics. 


# Hardware
+ Raspberry Pico
+ ESP-01S (1Mb) running ESP-AT 2.1
+ WS2812 144 LED strip
+ AM312 PIC
+ RGB CC LED
+ Resistors and Caps as in Schema
+ 5Amp 5Volt power supply
+ 5 Amp Fuse and 100C Thermal Fuse
+ M2 inserts for scew fixing of PCB to case and pommel onto case

## Schema 
A circuit diagram is available in KICAD format. Included in PCB folder.

Warning: The LEDs will draw 4Amps of current. You should assure yourself that the circuit and wiring can safely deliver that current. 

## ESP-01S
I got very cheap pack of ESP-01S but they came with 1Mb flash. Latest version of ESP-AT, version 2.2, requires 2MB. 

Fortunatly Jandrassy has built a working 2.1 firmware for 1MB. This is on github at: https://github.com/jandrassy/WiFiEspAT

To reliably upload the firmware to ESP-01S requires the RST IO0 to get pulled to zero and the released. Some further inform on how to do this at: https://blog.tiga.tech/flashing-esphome-firmware-to-esp-01s/

I used the RPI 4 to upload the firmware and the esptool.py. I had to drop frequency to 40m and change upload mode to dout in the download.config file. Then use the command
esptool.py --port /dev/ttyS0 write_flash @download.config

## Case
Case is 3D printed in PLA. I've modelled the case and wall mounts for the case in OpenSCAD. The model and SLA files available in the 3dmodels folder.

## Blade
Blade is a 20mm x 3mm x 1000mm OPAL Acrylic Tube. You can find these on ebay, though I found ebay quite expensive. Bought mine from [The Clear Plastic Shop](https://clearplastictube.co.uk/opal-acrylic-tube) in the UK. 


# Software


## C++ - For Raspberry PICO
+ [lwesp](https://github.com/MaJerle/lwesp): library for talking to ESP-AT over serial
+ + [forkedlwesp](https://github.com/jondurrant/lwespFork) Forked to allow MQTT over SSL. 
+ [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel): dependency for lwesp
+ [json-maker](https://github.com/rafagafe/json-maker)
+ [tiny-json](https://github.com/rafagafe/tiny-json)
+ [RtosComponents](https://github.com/jondurrant/RtosComponents) - my library for watchdog and RGB PWM control
+ [twinThingPicoESP](https://github.com/jondurrant/twinThingPicoESP) - IOT Library
+ [PicoLED](https://github.com/ForsakenNGS/PicoLED) - Manage the WS2812 LEDs


## Python -  Management Software
+ [TwinThingPyMQTT](https://github.com/jondurrant/TwinThingPyMQTT) - Python version of IOT Library, including TwinMgr which is how the management software talks to all the devices.
+ Python3
+ dotmap
+ numpy
+ paho-mqtt
+ pandas
+ PyMySQL
+ pyOpenSSL
+ SQLAlchemy
+ [twinThing](https://github.com/jondurrant/twinThing) 
+ mysql Db
+ [emqx](https://www.emqx.io/) using Mysql authentication 
+ [Joern Weissenborn LCARS](http://joernweissenborn.github.io/lcars/) - LCARS Look and feel

## Python Test Code
Some test code to interact with the api are included in py/test folder. They expect some environment variables to point to the MQTT hub and credentials.
+ MQTT_USER - Username and Client Id to connect to MQTT
+ MQTT_PASSWD - Password to authenticate against MQTT
+ MQTT_HOST - Hostname of MQTT server
+ MQTT_PORT - Port that MQTT is listen on, normally 1883

# Build and Deploy

##Clone project
'''
git clone --recurse-submodules https://github.com/jondurrant/LSaber1
'''
main.cpp include "Credentials.env" for the Wifi AP name and passwd, MQTT Host, Port, Username and password. I've not included this file in the repo. Please create one looking something like this:

``` C++
#define SID "SID"
#define PASSWD "PWD"

#define MQTTHOST "IP"
#define MQTTPORT 1883
#define MQTTUSER "jon"
#define MQTTPASSWD "PWD"
```


## To build C++:
``` bash
mkdir build
cd build
cmake ..
make
make install (only work if you used my config setup in the Pico Intro course as does SWD upload of code direct from make).
```

For more on how I use a Raspberry PI to build, deploy code  and debug the Pico see my course on [Udemy](https://www.udemy.com/course/introduction-to-c-development-environment-for-raspberry-pico/?referralCode=875319E04F95C9EC3414)

## Python Management Deployment
I have included a Docker file to build an image for the management software. It manipulates the Python path to include the right libraries, along with loading some from requirements.txt.

## EMQX
Setup EMQX using mysql authentication and acl. The [TwinThingPyMQTT](https://github.com/jondurrant/TwinThingPyMQTT) repo include the DDL to create the required tables.

Lights authenticate using the MAC address of the device for Client Id, Username and Password. They will require permissions within EMQX to access:
```
[
	{
	  "topic": "GRP/ALL/TPC/#",
	  "action": "sub",
	  "access": "allow"
	},
	{
	  "topic": "TNG/$CLIENTID/#",
	  "action": "pubsub",
	  "access": "allow"
	},
	{
	  "topic": "GRP/saber/TPC/#",
	  "action": "pubsub",
	  "access": "allow"
	}
]
```
The Management software will need access to:
```
[
	{
	  "topic": "GRP/ALL/TPC/#",
	  "action": "sub",
	  "access": "allow"
	},
	{
	  "topic": "TNG/$CLIENTID/#",
	  "action": "pubsub",
	  "access": "allow"
	},
	{
	  "topic": "GRP/saber/TPC/#",
	  "action": "pubsub",
	  "access": "allow"
	},
	{
	  "topic": "GRP/saber/TWIN/RES",
	  "action": "sub",
	  "access": "allow"
	},
	{
	  "topic": "GRP/saber/TWIN/GET",
	  "action": "pub",
	  "access": "allow"
	},
	{
	  "topic": "GRP/saber/TWIN/SET",
	  "action": "pub",
	  "access": "allow"
	}
]
```



