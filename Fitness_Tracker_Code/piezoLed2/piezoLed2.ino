const int sensorPin=5;
const int ledPin= 13;
const int threshold= 100;
int stepCount=0;

void setup(){
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
    Serial.println("start"); 
}
void loop(){
    int val= analogRead(sensorPin);
    
    if (val >= threshold)
    {
//        digitalWrite(ledPin, HIGH);
        delay(20);
//        digitalWrite(ledPin, LOW);
        stepCount++;
        Serial.println(stepCount);
    }
    else{;
//        digitalWrite(ledPin, LOW);
    }
}
