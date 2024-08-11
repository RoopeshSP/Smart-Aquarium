#include <ESP8266WiFi.h>  //Wifi module library
#include <WiFiClient.h>  //Client wifi connection library

#include <ThingSpeak.h>  //ThingSpeak Cloud library

const char* ssid = "TP-Link_8E98";  //Hotspot Username
const char* password = "86427920";  //Hotspot Password

WiFiClient client;  //client configuration

unsigned long myChannelNumber = 2291202;  //Thingspeak channel number
const char * myWriteAPIKey = "JW5BIZ13LGY3TJPZ";  //Thingspeak Write API key

String readstring = "";

String t, LDR, FS1, FS2;

int ind1; // , locations
int ind2;
int ind3;
int ind4;

void setup()
{
  Serial.begin(9600);

  //mySerial.begin(9600);

  delay(10);  
  // Connect to WiFi network  
  Serial.println();  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED)  
  {  
  delay(500);  
  Serial.print(".");  
  }  
  Serial.println("");  
  Serial.println("WiFi connected");  
  // Print the IP address  
  Serial.println(WiFi.localIP());
 
  ThingSpeak.begin(client);  
}
 
void loop()
{
  readstring = "";  //Reset the variable

  while (Serial.available())
  {  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = Serial.read(); //Conduct a serial read
  if (c == '#') {break;} //Exit the loop when the # is detected after the word
  readstring += c; //build the string
  }  

  if (readstring.length() > 0)
  {
    Serial.println(readstring);
   
    ind1  = readstring.indexOf(',');
    t   = readstring.substring(0, ind1);
    ind2  = readstring.indexOf(',', ind1+1);//finds location of second ,
    LDR  = readstring.substring(ind1+1, ind2);
    ind3  = readstring.indexOf(',',ind2+1);
    FS1   = readstring.substring(ind2+1, ind3);
    ind4  = readstring.indexOf(',',ind3+1);
    FS2   = readstring.substring(ind3+1);//captures remain part of data after last ,

    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("LDR Value: ");
    Serial.println(LDR);
    Serial.print("Float Switch 1 Value: ");
    Serial.println(FS1);
    Serial.print("Float Switch 2 Value: ");
    Serial.println(FS2);
   
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, LDR);
    ThingSpeak.setField(3, FS1);
    ThingSpeak.setField(4, FS2);
   
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    delay(1000);  
  }  
}