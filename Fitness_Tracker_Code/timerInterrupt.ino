const int sensorPin=5;
const int ledPin= 13;
const int threshold= 100;
int stepCount=0;
int timer1_counter;

void setup(){
    pinMode(ledPin, OUTPUT);
    setupTimerForPeizo();
    Serial.begin(115200);
    Serial.println("start"); 
}

void setupTimerForPeizo(){
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
//  TCNT1 = timer1_counter;   // preload timer
//  timer1_counter = 34286;
//  Serial.println(timer1_counter);
//  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

void loop(){
    int val= analogRead(sensorPin);
    
    if (val >= threshold)
    {
//        digitalWrite(ledPin, HIGH);
        delay(100);
//        digitalWrite(ledPin, LOW);
        stepCount++;
        Serial.println(stepCount);
    }
    else{;
//        digitalWrite(ledPin, LOW);
    }
}
