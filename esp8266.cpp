#include "esp8266.h"

//The default serial timeout in ms.
//This can be quite short as most operations respond quickly.
#define SERIAL_TIMEOUT 3000

//Joining an AP takes much longer, so a larger timeout must be specified.
//for that operation.
#define SERIAL_TIMEOUT_JOIN_AP 15000

//Similarly with creating an access point.
#define SERIAL_TIMEOUT_CREATE_AP 7000


ESP8266::ESP8266(HardwareSerial& esp, int esp_baud_rate) : esp(esp) {
	esp.begin(esp_baud_rate);
	esp.setTimeout(SERIAL_TIMEOUT);
	while(!esp);
	while(esp.available() > 0) esp.read();
}

/*
Main function for sending commands to the ESP. Default behaviour is
to wait for the ESP to respond with a newline and 'OK', as all AT commands
do this. However, the reset command will reply with 'ready' instead so
the waiting behavour is optional.
*/
bool ESP8266::sendCommand(String cmd, bool wait_for_OK) {
	while(esp.available() > 0) esp.read();
	esp.println(cmd);
	esp.flush();
	if(wait_for_OK) {
		return esp.find("\nOK");
	} else {
		return true;
	}
}

bool ESP8266::sendCommand(String cmd) {
	return sendCommand(cmd, true);
}

bool ESP8266::checkConnection() {
	return sendCommand("AT");
}

bool ESP8266::getIP(char *buf) {
	char c[2];
	sendCommand("AT+CIPSTA?", false);
	if(!esp.find("+CIPSTA:\"")) return false;
	for(int i = 0; i < 16; i++) {
		if(esp.readBytes(c, 1) != 1) return false;
		if(c[0] == '\"') {
			buf[i] = '\0';
			break;
		}
		buf[i] = c[0];
	}
	buf[15] = '\0';
	return true;
}

bool ESP8266::sendData(String msg, int chan) {
	if(chan < 0) return false;
	while(esp.available() > 0) esp.read();
	String cmd = "AT+CIPSEND=";
	cmd += chan;
	cmd += ",";
	cmd += msg.length();
	esp.println(cmd);
	if(!esp.find(">")) return false;
	esp.println(msg);
	if(!esp.find("OK")) return false;
	return true;
}


bool ESP8266::connectToAP(String ssid, String pass) {
	//Go to 'station' (client) mode
	if(!sendCommand("AT+CWMODE=1")) return false; 
	if(!reset()) return false;
	esp.setTimeout(SERIAL_TIMEOUT_JOIN_AP); //The join operation takes much longer than others
	if(!sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + pass + "\"")) return false;
	esp.setTimeout(SERIAL_TIMEOUT);
	if(!sendCommand("AT+CIPMUX=1")) return false;
	return true;
}

bool ESP8266::setupAsAP(String ssid, String pass) {
	//Go to 'ap' (access point) mode
	if(!sendCommand("AT+CWMODE=2")) return false; 
	if(!reset()) return false;
	esp.setTimeout(SERIAL_TIMEOUT_CREATE_AP); //The setup AP operation takes longer than others
	if(!sendCommand("AT+CWSAP=\"" + ssid + "\",\"" + pass + "\",11,0")) return false;
	esp.setTimeout(SERIAL_TIMEOUT);
	if(!sendCommand("AT+CIPMUX=1")) return false;
	return true;
}

bool ESP8266::quitAP() {
	return sendCommand("AT+CWQAP");
}

bool ESP8266::createServer(int port) {
	char outb[20];
	if(port < 0 || port > 65535) return false;
	sprintf(outb, "AT+CIPSERVER=1,%d", port);
	return sendCommand(outb);
}

Message ESP8266::waitForMessage() {
	Message m;
	m.channel = -1;

	char c;
	if(!esp.find("+IPD,")) return m;

	m.channel = esp.parseInt();

	if(esp.readBytes(&c, 1) != 1 || (c != ',')) {
		m.channel = -1;
		return m;
	}
	int len = esp.parseInt();
	if(esp.readBytes(&c, 1) != 1 || (c != ':')) {
		m.channel = -1;
		return m;
	}
	for(int i = 0; i < len; i++) {
		esp.readBytes(&c, 1);
		m.data += c;
	}
	
	return m;
}

bool ESP8266::reset() {
	sendCommand("AT+RST", false); //Reset
	return esp.find("ready");
}

