//this is master

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);

char state = 0;
const int ledPin = 8;
const int buttonPin = 2;
int buttonState = 1;

void setup() 
{ 
    BTSerial.begin(38400);
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH); 
    Serial.println("Start"); 
}

void loop()
{
   if(BTSerial.available() > 0)
   { 
      // Checks whether data is comming from the serial port
      state = BTSerial.read(); // Reads the data from the serial port
      Serial.println(state);
      
   }
 
   if(Serial.available()>0){
      BTSerial.print(Serial.read());
      
   }
}
