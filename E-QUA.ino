#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <ESP8266WiFi.h>                                                // esp8266 library

#define FIREBASE_HOST "e-qua69.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "VANXyIu7A1OWuYMIjwMGe55xrKNOW3RIpWn6UHux"                    // the secret key generated from firebase
#define WIFI_SSID "DP"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "giveme100bucks"                                    //password of wifi ssid
#define SensorPin A0 
int k;

float b;
int buf[10],temp;
unsigned long int avgValue;

void setup(void) {
//pinMode(LED,OUTPUT);
Serial.begin(9600);
Serial.println("pH meter experiment!"); //Test the serial monitor
//  pinMode(pin,INPUT);
  delay(1000);                 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}


void loop(void){
 for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
float DO=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
float  phValue=3.5;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  String fire = String(phValue);   
  Serial.print("    Do:");  
  Serial.print(DO,2);
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Ph",fire );    //pHValue,voltage
  float T=26+273;
  String fire1 = String(T);                                          
  delay(5);
 Firebase.setString("/usr/admin/temp",fire1 );
 
 float S=((T*phValue)-(0.5523*phValue)-2559.7)/(4.5+(0.01391*phValue));
 Serial.print("Salinity :"); 
  Serial.print(-S/2,6);
  Serial.println(" ");
  if(S<0)
  {
  String fire2 = String(-S/2);
   Firebase.setString("/usr/admin/salinity",fire2 );
  }
  else
  {
   String fire2 = String(S/2);  
    Firebase.setString("/usr/admin/salinity",fire2 );
  }
 
  if(phValue>=8 && phValue<=10)
  {
      String f1 = "STABLE";   
  Serial.print("Status:");  
  Serial.print("STABLE");
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Status",f1 );
  }
  else if(phValue>6 && phValue<8)
  {
  String f2 = "UNSTABLE";   
  Serial.print("Status:");  
  Serial.print("UNSTABLE");
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Status",f2 ); 
  }
  else
  {
  String f3 = "EMERGENCY";   
  Serial.print("Status:");  
  Serial.print("EMERGENCY");
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Status",f3 );     
  }
  delay(500);
}
