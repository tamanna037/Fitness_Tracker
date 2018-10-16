#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];

char MESSAGE[300];
char phone[12] = "01534907008";

#define PIN_TX    8
#define PIN_RX    7
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("start");

  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      Serial.print("Sim808 init error\r\n");
      delay(1000);
  }


  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");


   sprintf(MESSAGE, "Maa....Maa");
    
   Serial.println("Start to send message ...");
    
   Serial.println(MESSAGE);
   Serial.println(phone);
  
   sim808.sendSMS(phone,MESSAGE);
//   Serial.print("res: ");
//   Serial.println(res);
//      
}

void loop()
{
      //************* Turn off the GPS power ************
      sim808.detachGPS();
}
