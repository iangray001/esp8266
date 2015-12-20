# ESP8266

An Arduinio library for communicating with the ESP8266 wifi to serial board.

## Description
This library is designed to be simple to use with minimal code in your main sketch. The following example shows how to set up a simple TCP server which responds to incoming messages. Error checking is omitted for brevity; most library functions will return `false` if an error occured.

	#include <Arduino.h>
	#include <esp8266.h>

	//Initialise the ESP. In this case it is connected to hardware Serial2.
	ESP8266 esp(Serial2, baud115200);

	void setup() {
		esp.connectToAP("MYSSID", "MYPASSWORD");
		esp.createServer(8266);
	}

	void loop() {
		Message m = esp.waitForMessage();
		if(m.channel >= 0) {
			esp.sendData("Reply from ESP8266!", m.channel);
		}
	}

`exampleclient/exampleclient.py` is an example of how to interact with the ESP from Python on the desktop.

`serialecho/serialecho.ino` is a simple serial echo sketch which is useful for directly communicating with the ESP. Load the sketch in the Arduino IDE and upload it to your board. Then, using the serial monitor, ensure line endings are set to "Both NL & CR" and you can manually enter AT commands and observe the response from the ESP8266.

## Requirements and connections
The library currently requires a HardwareSerial connection between the Arduino and ESP. For example, for the Mega 2560 you could connect pin 17 (TX2) to RXD on the ESP and pin 16 (RX2) to TXD on the ESP and initialise the library as follows:

	ESP8266 esp(Serial2, baud115200);

Don't forget to tie pin CH_PD on the ESP to 3.3v to enable serial communications.

## Installation
To use this in your Arduino sketches you need to add it to your Arduino libraries. First, navigate to your Arduinio libraries:

On Linux or OS X: `~/Documents/Arduino/libraries/`

On Windows: `My Documents\Arduino\libraries\`

And check out the repo there:

    git clone git@github.com:iangray001/esp8266.git

If you are using the Arduino IDE you will need to restart it.
