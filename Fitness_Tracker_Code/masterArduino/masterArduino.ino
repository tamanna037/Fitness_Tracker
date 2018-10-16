/*THIS TUTORIAL USED GSM SIM900A MINI V3.9.2
 
  Connect 5VT to D9 and 5VR to D10
  Feed GSM SIM900A with Arduino's 5V

  Code by IDAYU SABRI - MYBOTIC
*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 5);
char msg;
char call;
float calorie=0;
float height=5.6;
float weight=73;

String phnNumber;

//bluetooth
SoftwareSerial BTSerial(10, 11);
int BLUETOOTH_MSG_LEN = 6;
char BLUETOOTH_MSG_HEADER = 'P';
int pulseVal=0;

//piezoSensor
int currTime = 0;
int stepDetected = 0;
int vals[13];
int lastStepTime = 0;
int noise = 10;
int expectingStep = 0;
int maxVal = 0;
int maxValIdx = 0;
int        firstAvg = 0;
int       secAvg=0;
int       thirdAvg=0;

const int sensorPin=5;  
int stepCount=0;

//booleans
boolean started = false;
boolean sendMsg = false;
boolean stopNow = false;



void initPiezo(){
  int i=0;
  for(i=0;i<13;i++){
    vals[i] = 0;
  }
  stepCount = 0;
}

void initBluetooth(){
  BTSerial.begin(38400);
}


enum _parseState {
  PS_DETECT_MSG_TYPE,

  PS_IGNORING_COMMAND_ECHO,

  PS_READ_CMTI_STORAGE_TYPE,
  PS_READ_CMTI_ID,

  PS_READ_CMGR_STATUS,
  PS_READ_CMGR_NUMBER,
  PS_READ_CMGR_SOMETHING,
  PS_READ_CMGR_DATE,
  PS_READ_CMGR_CONTENT
};


byte state = PS_DETECT_MSG_TYPE;

char buffer[80];
byte pos = 0;

int lastReceivedSMSId = 0;
boolean validSender = false;

void resetBuffer() {
  memset(buffer, 0, sizeof(buffer));
  pos = 0;
}

void initGSM(){
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("GSM SIM900A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");  
  Serial.println("e : to redial");
  Serial.println();
  delay(100);
  
}

void setup()
{
  initPiezo();
  initBluetooth();
  initGSM();
  MakeCall();
  Serial.println("Setup complete");

}

void loop()
{
  if(started ){
      Serial.println("loop");
      stepDetect();
      readBluetoothData(); 
      if(sendMsg){
          
          String msg="step: "+stepCount;
          msg +=" pulse: "+pulseVal;
          float dist = calculateDistance(stepCount);
          float cal = getWalkingCalorie(stepCount*2);
          //msg +=" dist: "+ dist;
          //msg += "cal: "+ cal;
          SendMessageToUser(msg);
          sendMsg = false;
      }
  } 
   if(mySerial.available()) {
      int a=mySerial.read();//mySerial.read()
      Serial.write(a);
      parseATText(a);
  }

}

void readBluetoothData(){
  if(BTSerial.available() > 0)
   { 
      Serial.println("bluetooth");
      // Checks whether data is comming from the serial port
      //state = BTSerial.read(); // Reads the data from the serial port
      processStringFromBluetooth();
   }
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
                 pulseVal = 0;
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

void stepDetect(){
  
  int val= analogRead(sensorPin);
  
  int i=0;
  for( i=0; i<12; i++)
  {                // shift data in the rate array
    vals[i] = vals[i+1];                  // and drop the oldest IBI value 
  }
        

  currTime = millis();
  int N = currTime - lastStepTime;
  printf("currTime: %d\n",currTime);
  if(N>400 || lastStepTime==0){
      vals[12] = val;
      if(expectingStep !=0){
        
        printf("hope %d\n",expectingStep);
        /*if(maxVal < val){
          maxVal = val;
          maxValIdx = 12;
        }else{
          maxValIdx = 12-expectingStep;
        }*/
        maxValIdx = 12-expectingStep;
        printf("maxVal %d idx %d\n",maxVal,maxValIdx);
        expectingStep++;
        if(maxValIdx==6){
          firstAvg = 0;
          secAvg = 0;
          thirdAvg = 0;
          /*for(i=0;i<5;i++)
            firstAvg += vals[i];
          for(i=4;i<5;i++)
            secAvg += vals[i];
          for(i=8;i<5;i++)
            thirdAvg += vals[i];
          int total = firstAvg+secAvg+thirdAvg;
          total /= 15;
          firstAvg /= 5;
          secAvg /=5;
          thirdAvg/=5;*/
          int total=0;
          for(i = 3;i<10;i++){
            total+=vals[i];
          }
          printf("total %d\n",total);
          total /= 7;
          printf("total %d\n",total);
          //printf("first %d sec %d third %d\n",firstAvg,secAvg,thirdAvg);
          /*if(firstAvg > 10 && secAvg > 10 && thirdAvg > 16 
            && secAvg > firstAvg && thirdAvg < secAvg){
          stepDetected = 1;
          lastStepTime = currTime;
          printf("step detected\n");
        
          }else{ */  
          if(total > 28){
            stepDetected = 1;
            lastStepTime = currTime;
            stepCount++;
            printf("step detected %d\n",N);
            Serial.print("step detected ");
            Serial.println(stepCount);
          }           
        expectingStep = 0;
        maxVal = 0;
        maxValIdx = 0;
        firstAvg = 0;
        secAvg=0;
        thirdAvg=0;         
          //}
        
        }
      }
    if(val >= 40 && expectingStep == 0){
      printf("hope\n");
      expectingStep = 1;
      if(maxVal < val){
        maxVal = val;
        maxValIdx = 12;
      }
    }
      }
        
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"01920924741\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("sim900a sms");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (mySerial.available()>0)
  {
    msg=mySerial.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  mySerial.println("ATD01920924741;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  mySerial.println("ATA");
  delay(1000);
  {
    call=mySerial.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  mySerial.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}


void parseATText(byte b) {

  buffer[pos++] = b;

  if ( pos >= sizeof(buffer) )
    resetBuffer(); // just to be safe

  /*
   // Detailed debugging
   Serial.println();
   Serial.print("state = ");
   Serial.println(state);
   Serial.print("b = ");
   Serial.println(b);
   Serial.print("pos = ");
   Serial.println(pos);
   Serial.print("buffer = ");
   Serial.println(buffer);*/

  switch (state) {
  case PS_DETECT_MSG_TYPE: 
    {
      if ( b == '\n' )
        resetBuffer();
      else {        
        if ( pos == 3 && strcmp(buffer, "AT+") == 0 ) {
          state = PS_IGNORING_COMMAND_ECHO;
        }
        else if ( pos == 6 ) {
          //Serial.print("Checking message type: ");
          //Serial.println(buffer);

          if ( strcmp(buffer, "+CMTI:") == 0 ) {
            Serial.println("Received CMTI");
            state = PS_READ_CMTI_STORAGE_TYPE;
          }
          else if ( strcmp(buffer, "+CMGR:") == 0 ) {
            Serial.println("Received CMGR");            
            state = PS_READ_CMGR_STATUS;
          }
          resetBuffer();
        }
      }
    }
    break;

  case PS_IGNORING_COMMAND_ECHO:
    {
      if ( b == '\n' ) {
        //Serial.print("Ignoring echo: ");
        //Serial.println(buffer);
        state = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMTI_STORAGE_TYPE:
    {
      if ( b == ',' ) {
        Serial.print("SMS storage is ");
        Serial.println(buffer);
        state = PS_READ_CMTI_ID;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMTI_ID:
    {
      if ( b == '\n' ) {
        lastReceivedSMSId = atoi(buffer);
        Serial.print("SMS id is ");
        Serial.println(lastReceivedSMSId);

        mySerial.print("AT+CMGR=");
        mySerial.println(lastReceivedSMSId);
        //delay(500); don't do this!

        state = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMGR_STATUS:
    {
      if ( b == ',' ) {
        Serial.print("CMGR status: ");
        Serial.println(buffer);
        state = PS_READ_CMGR_NUMBER;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMGR_NUMBER:
    {
      if ( b == ',' ) {
        Serial.print("CMGR number: ");
        Serial.println(buffer);
        String strBuffer(buffer);
        getPhoneNumber(strBuffer);

        // Uncomment these two lines to check the sender's cell number
        //validSender = false;
        //if ( strcmp(buffer, "\"+0123456789\",") == 0 )
        validSender = true;

        state = PS_READ_CMGR_SOMETHING;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMGR_SOMETHING:
    {
      if ( b == ',' ) {
        Serial.print("CMGR something: ");
        Serial.println(buffer);
        state = PS_READ_CMGR_DATE;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMGR_DATE:
    {
      if ( b == '\n' ) {
        Serial.print("CMGR date: ");
        Serial.println(buffer);
        state = PS_READ_CMGR_CONTENT;
        resetBuffer();
      }
    }
    break;

  case PS_READ_CMGR_CONTENT:
    {
      if ( b == '\n' ) {
        Serial.print("CMGR content: ");
        Serial.print(buffer);
        String strBuffer(buffer);
       
        whatsOnSms(strBuffer);

        parseSMSContent();

        mySerial.print("AT+CMGD=");
        mySerial.println(lastReceivedSMSId);
        //delay(500); don't do this!

        state = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;
  }
}

void parseSMSContent() {

  char* ptr = buffer;

  while ( strlen(ptr) >= 2 ) {

    if ( ptr[0] == 'r' ) {
      if ( ptr[1] == '1' )
        digitalWrite(2, HIGH);
      else
        digitalWrite(2, LOW);
    }

    if ( ptr[0] == 'y' ) {
      if ( ptr[1] == '1' )
        digitalWrite(4, HIGH);
      else
        digitalWrite(4, LOW);
    }

    ptr += 2;
  }
}

void whatsOnSms(String buffer)
{
        
        int firstIndex=0;
        String subStr;
        int word=1;
       
        if(buffer.length()>5){
          for(int i=0;i<buffer.length();i++)
          {
            if(buffer[i]==' ')
            {
              
              subStr=buffer.substring(firstIndex,i);
              Serial.println("loop");
              Serial.println(subStr);
              if(word==1)
              {
                if(subStr=="start") {
                  Serial.println("Started");
                  started = true;
                  }
                else break;
                word=2;
                } 
               else if(word==2)
               {
                 height=subStr.toFloat();
                 word=3;
                 Serial.println(height);
                }
               
              firstIndex=i+1;
              }
              
            }
        }
          if(firstIndex!=buffer.length()){
          subStr=buffer.substring(firstIndex,buffer.length());
           Serial.println("if");
            Serial.println(subStr);
             if(subStr=="send"){
                sendMsg = true;
              }
              else if(subStr=="stop"){
                Serial.println("Stopped");
                started = false;
                }
              else if(word==3) {weight=subStr.toFloat();Serial.println(weight);}
              
            }
          
        
}

void getPhoneNumber(String buffer)
{
  String subStr;
   subStr=buffer.substring(3,buffer.length());
   Serial.println(subStr);
   phnNumber=subStr;
  
  } 


void SendMessageToUser(String msg)
{
  //String phnNumber="01920924741";
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\""+phnNumber+"\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(msg);// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

float calculateDistance(int stepCount)
{
  float distance;
  distance=stepCount*getStrideLength()*2;
  distance/=39370.1;
    return distance;
  }

  float getStrideLength()
  {
    int strideLength=convertHeightInInches()*0.413;

    Serial.println("Stride: ");
    Serial.println(strideLength);
    return strideLength;
    }

  float convertHeightInInches()
  {
    
    int heightInt=int(height);
    float heightInInches=(height-heightInt)*10;
    heightInInches+=heightInt*12.0;
    Serial.println("Height: ");
    Serial.println(heightInInches);
    
    return heightInInches;
    }

    float getWalkingCalorie(int stepCount)
    {

      float caloriePerKm=0.35418*getWeightOnPound();
      return calculateDistance(stepCount)*caloriePerKm;
     
     }
    float getWeightOnPound()
    {
      return weight*2.20462;
    }


float getSpeedForTwoSec(int stepCount)
{
  return (stepCount*2)/getStrideLength();
  }

float getCalorieForTwoSec(int stepCount)
{
  float calorieForTwoSec=getSpeedForTwoSec(stepCount)*weight;
  calorieForTwoSec/=400;
  calorie+=calorieForTwoSec;
  return calorieForTwoSec;
  }

float getCalorieWhileResting()
{
  return 1800/weight;
  }
  
