/*
This simple sketch is useful for interacting directly with the ESP.

Set debugserial and espserial according to your hardware connections,
then use the Arduino IDE's serial monitor to send commands directly 
to the ESP and read the output.

Don't forget to set the correct baud rates.


Example boot sequences
----------------------

To connect to an access point called SSID with password PASSWORD 
and setup a TCP server on port 8266:

AT+CWMODE=1
AT+RST
AT+CWJAP="SSID","PASSWORD"
AT+CIPMUX=1
AT+CIPSERVER=1,8266

To create an access point called "esp8266" with a TCP server on 
port 8266:

AT+CWMODE=2
AT+RST
AT+CWSAP="esp8266","",11,0
AT+CIPMUX=1
AT+CIPSERVER=1,8266
*/

HardwareSerial& debugserial = Serial;
HardwareSerial& espserial = Serial2;


void setup() {
	espserial.begin(115200);
	espserial.setTimeout(10000);
	debugserial.begin(115200);
	while(!debugserial);
	while(!espserial);
	while(espserial.available() > 0) espserial.read();
	debugserial.println("Begin ESP8266 connection...");
}

void loop() {
	int buf;

	if (debugserial.available() > 0) {
		buf = debugserial.read();
		espserial.write(buf);
	}

	if (espserial.available() > 0) {
		buf = espserial.read();
		debugserial.write(buf);
	}	

}

