#include <SoftwareSerial.h>
#define RX 2
#define TX 3
int state = 0;
int inp_nodemcu = 6;
int buzz = 7;
String AP = "Nokia 1";       // AP NAME
String PASS = "Chetan@123"; // AP PASSWORD
String API = "FOIWDP2BXVBFQ6IU";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;
SoftwareSerial esp8266(RX, TX);

float resolution = 5.0 / 1023; // 3.3 is the supply volt  & 1023 is ma
void setup() {
  pinMode(inp_nodemcu, INPUT);
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop() {
  valSensor = getSensorData();
  if (valSensor > 1)
  {
	digitalWrite(buzz, HIGH);
  String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(valSensor);
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); delay(1500); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  }
	
  else
  {	
	delay(100);
	digitalWrite(buzz, LOW);
  }
}

float getSensorData() {
  //return random(1000); // Replace with your own sensor code
  state = digitalRead(inp_nodemcu);

  if ( state == HIGH) {


    Serial.println("Motion detected");

    float temp = (analogRead(A0)) * resolution;
    return temp;
    delay(5000);
  } else {
    Serial.println("Motion not detected!");
    delay(2000);
  }
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}
