#include <ESP8266WiFi.h>

int buzz = 12;
String apiWritekey = "FOIWDP2BXVBFQ6IU"; // replace with your THINGSPEAK WRITEAPI key here
const char* ssid = "Nokia 1"; // your wifi SSID name
const char* password = "Chetan@123" ;// wifi pasword

const char* server = "api.thingspeak.com";
float resolution = 5.0 / 1023; // 3.3 is the supply volt  & 1023 is max analog read value
WiFiClient client;

void setup() {
  Serial.begin(115200);
 
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to wifi...");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
}

void loop() {
  float pir_input = (analogRead(A0))*resolution;
  if (pir_input > 1)
  {
	digitalWrite(buzz, HIGH);
	  if (client.connect(server, 80))
	  {
	    String tsData = apiWritekey;
	    tsData += "&field3=";
	    tsData += String(pir_input);
	    tsData += "\r\n\r\n";

	    client.print("POST /update HTTP/1.1\n");
	    client.print("Host: api.thingspeak.com\n");
	    client.print("Connection: close\n");
	    client.print("X-THINGSPEAKAPIKEY: " + apiWritekey + "\n");
	    client.print("Content-Type: application/x-www-form-urlencoded\n");
	    client.print("Content-Length: ");
	    client.print(tsData.length());
	    client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
	    client.print(tsData);

 	   Serial.print("Motion detected: ");
	    Serial.print(pir_input);
	    Serial.println("uploaded to Thingspeak server....");
	  }
  
	  client.stop();

	  Serial.println("Waiting to upload next reading...");
	  Serial.println();
	  // thingspeak needs minimum 15 sec delay between updates
	  delay(15000);
  }
  else
  {
	digitalWrite(buzz, LOW);
	delay(3000);
  }
}
