#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

String s="Lighton";
bool done=false;
int rel1=4;
int rel2=3;
void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("AT+CNMI=2,2,0,0,0");
  Serial.println("AT+CMGF=1");
  digitalWrite(rel1,HIGH);
  digitalWrite(rel2,HIGH);
  pinMode(rel1,OUTPUT);
  pinMode(rel2,OUTPUT);
}


void loop()

{
  if(mySerial.available()){
    String a=mySerial.readString();
    Serial.println(a);
    Serial.println(a.length());
    a.remove(0,51);
    int len=a.length();
    a.remove(len-2,2);
    Serial.println(a);
    a.trim();
    Serial.println(a.length());
    Serial.println(s.length());
    if(a=="Lighton")
      Light_on();
    if(a=="Lightoff")
      Light_off();
    if(a=="Fanon")
      Fan_on();
    if(a=="Fanoff")
      Fan_off();
      
    
  }

  if(Serial.available()){
    mySerial.write(Serial.read());
  }
}

void Light_on(){

  digitalWrite(rel1,LOW);
}

void Light_off(){

 digitalWrite(rel1,HIGH);
}
void Fan_on(){
  digitalWrite(rel2,LOW);
}

void Fan_off(){
  digitalWrite(rel2,HIGH);
}



