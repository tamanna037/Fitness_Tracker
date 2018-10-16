#include <Time.h>

const int sensorPin=5;
const int ledPin= 13;
const int threshold= 100;
int stepCount=0;
boolean valZero=true;
int maxVal = 0;
int startTime;
int interval = 10;

void setup(){
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
    setTime(0);
    startTime = millis();
    Serial.println("start"); 
}
void loop(){
  //if(millis()-startTime == interval){
      int val= analogRead(sensorPin);
        
//        if(val>maxVal)
//            maxVal = val;
//        if(val!=0 && valZero == true){
//          Serial.println("O");
//          valZero = false;
//        }
//        if(val==0 && valZero == false){
//          Serial.println();
//          valZero = true;
//        }
//        if(val != 0){
//            //MSerial.print("val: ");
//            Serial.println(val);
//            //Serial.print(" max: ");
//            //Serial.println(maxVal);
//        }
//        else{
//          
//        }
  //}
Serial.println(val);
delay(30);
   
//    if (val >= threshold)
//    {
////        digitalWrite(ledPin, HIGH);
//        delay(20);
////        digitalWrite(ledPin, LOW);
//        stepCount++;
//        Serial.println(stepCount);
//    }
//    else{;
////        digitalWrite(ledPin, LOW);
//    }
}
