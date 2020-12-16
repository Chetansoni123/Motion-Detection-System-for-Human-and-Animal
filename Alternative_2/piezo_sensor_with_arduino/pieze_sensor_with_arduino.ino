#include <SoftwareSerial.h>
#define RX 2
#define TX 3
int state = 0; 
int ipin = 6;
int LED = 8;
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
//boolean valSensor = false;
SoftwareSerial esp8266(RX, TX);

float resolution = 5.0 / 1023; // 3.3 is the supply volt  & 1023 is ma
void setup() {
   pinMode(ipin, INPUT);
   pinMode(LED, OUTPUT);
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop() {
valSensor = getSensorData();
/*if(valSensor < 1){
   printSensorValue();
  String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(valSensor);
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); 
  delay(500); 
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}*/
 if(valSensor > 1){
  digitalWrite(LED, HIGH);
  
  printSensorValue();
  String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(valSensor);
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); 
  delay(500); 
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}
else{
  digitalWrite(LED, LOW);
  delay(3000);
}
}

float getSensorData() {
  //return random(1000); // Replace with your own sensor code


  int piezo_input = (analogRead(A0))*resolution;
 
  return piezo_input;

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
void printSensorValue(){
int  pirstatevalue = getSensorData();
   String a = "Sensor Value : ";
  String c = a + pirstatevalue;
  Serial.println(c);
}
