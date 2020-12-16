
int pirstate = 0;
int piezoState = 0;
int threshold =1000;
int sensorOutput = A5;
void setup() {
  // put your setup code here, to run once:
pinMode(8, INPUT);
pinMode(13, OUTPUT);
pinMode(sensorOutput,INPUT);
pinMode(6,OUTPUT);
Serial.begin(9600); // Other baud rates can be used...
Serial.println("My Sketch has started");

}

void loop() {
  // put your main code here, to run repeatedly:
  
piezoState = analogRead(sensorOutput);


if( piezoState >= threshold  ){
  
Serial.println("piezo detected!");
digitalWrite(13, HIGH);
delay(1000);

}
else {
  digitalWrite(13, LOW);
}

}
  
  
 

   
      
