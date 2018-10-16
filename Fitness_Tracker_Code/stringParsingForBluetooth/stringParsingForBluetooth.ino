int BLUETOOTH_MSG_LEN = 6;
char BLUETOOTH_MSG_HEADER = 'P';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly.
  if(Serial.available()){
    processStringFromBluetooth();
  }
  //delay(1000);
}

void processStringFromBluetooth(){
    if(Serial.available()>=BLUETOOTH_MSG_LEN){
         char c = Serial.read() ;
         //Serial.print(c);
         if ( c == BLUETOOTH_MSG_HEADER ) {
              c = Serial.read();
              //Serial.print(c);
              if( c == BLUETOOTH_MSG_HEADER){
                //Serial.println("Hi");
                 int pulseVal = 0;
                for (int i = 0; i<BLUETOOTH_MSG_LEN-2; i++) {
                   c = Serial.read();
                   //Serial.print(c);
                   if ( c=='#')break;
                   if ( c >= '0' && c <= '9') {
                      pulseVal = (10 * pulseVal) + (c - '0') ; // convert digits to a number
                   }
                   //Serial.println(pulseVal);
                }
                Serial.println();
                Serial.print("pulseValue: ");
                Serial.println(pulseVal);
              }
         }
    }
}

