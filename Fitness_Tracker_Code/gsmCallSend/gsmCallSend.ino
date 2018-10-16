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

void setup()
{
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

void loop()
{  
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      //SendMessage();
      SendMessageToUser("do some exercise");
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'e':
      RedialCall();
      break;
    case 'i':
      ReceiveCall();
      break;
     case 'p':
      /*char charArr[50];
      charArr[0]='s'; charArr[1]='t'; charArr[2]='a'; charArr[3]='r'; charArr[4]='t';charArr[5]=' ';
      charArr[6]='5';charArr[7]='.';charArr[8]='1';  charArr[9]=' ';charArr[10]='4';charArr[11]='6';
      charArr[12]='.';charArr[13]='7';charArr[14]='\0'; 

    /* charArr[0]='s'; charArr[1]='t'; charArr[2]='o'; charArr[3]='p'; charArr[4]='\0';
     charArr[0]='s'; charArr[1]='e'; charArr[2]='n'; charArr[3]='d'; charArr[4]='\0';
      
      String strChar(charArr);
      whatsOnSms(strChar);

      char charArr2[50];
      charArr2[0]='+'; charArr2[1]='8'; charArr2[2]='8'; charArr2[3]='0'; charArr2[4]='1';charArr2[5]='\0';
  
      String strChar2(charArr2);

      getPhoneNumber(strChar2);*/
      Serial.println(calculateDistance(722));
      Serial.println(getWalkingCalorie(722*2));
      
      break;

   
  }
 /*if (mySerial.available()>0)
 Serial.write(mySerial.read());*/

 if(mySerial.available()) {
  int a=mySerial.read();//mySerial.read()
  Serial.write(a);
    parseATText(a);
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
                if(subStr=="start") {Serial.println("Started");}
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
             if(subStr=="send"){}
              else if(subStr=="stop"){Serial.println("Stopped");}
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
  
