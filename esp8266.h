#ifndef __ESP8266_H_
#define __ESP8266_H_

#include <Arduino.h>


//Messages received by the ESP are returned as instances of this class.
class Message {
public:
	int channel; //The channel on which the message was received. Set to -1 to indicate an invalid message.
	String data; //The data receieved
};


typedef enum {baud9600, baud19200, baud38400, baud74880, baud115200, baud230400, baud460800, baud921600} espbaud_t;


class ESP8266{
public:
	/*
	espserial is the hardware serial connection to the ESP. For example, on the Mega this may be 
	the Serial2 object. esp_baud_rate is the baud rate of the ESP. Normally this is 115200, but sometimes 
	they are set to 9600. You can change this by sending the ESP the following AT 
	command, and then resetting: AT+CIOBAUD=115200
	*/
	ESP8266(HardwareSerial& espserial, espbaud_t esp_baud_rate);

	/*
	Resets the ESP. Returns true once the ESP has replied "ready", false if the 
	reset takes more time than the serial timeout.
	*/
	bool reset();

	/*
	Checks that the library can communicate with the ESP correctly. 
	*/
	bool checkConnection();

	/*
	Connects to an access point with the SSID and password given.
	*/
	bool connectToAP(String ssid, String pass);

	/*
	Leaves the currently connected access point.
	*/
	bool quitAP();

	/*
	Creates an access point with the provided SSID and password.
	*/
	bool setupAsAP(String ssid, String pass);

	/*
	Create TCP server on the specified port. Must be connected to an AP first.
	*/
	bool createServer(int port);

	/*
	Once connected to an AP, this will request the ESP's IP address and
	copy it into the buffer, null-terminated, as a string.
	The buffer must be at least 16 bytes.
	Returns true if buf was successfully filled with a valid IP address.
	*/
	bool getIP(char *buf);

	/*
	Sends the string msg to the client connected to channel chan.
	*/
	bool sendData(String msg, int chan);

	/*
	Waits for a message to be sent to the ESP. If the returned Message object 
	has a channel of -1, then an invalid message was obtained. 
	*/
	Message waitForMessage();

	/*
	Sends a raw AT command to the ESP, and wait for "\nOK" to indicate that the ESP has
	completed the task. cmd does not have to have a \n or \r appended, this is done
	automatically.
	Returns true if the ESP responded.
	*/
	bool sendCommand(String cmd);

	/*
	As with sendCommand(String) but does not wait for "OK" from the ESP before returning.
	Returns true if wait_for_OK is false, or if wait_for_OK is true and a response was 
	obtained before the serial timeout.
	*/
	bool sendCommand(String cmd, bool wait_for_OK);

private:
	//The serial connection to the ESP module.
	HardwareSerial& esp;
};


#endif
