#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

/* Controlling LED using Firebase console by CircuitDigest(www.circuitdigest.com) */
#include <ESP8266WiFi.h>                                                // esp8266 library
#define FIREBASE_HOST "e-qua69.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "8brn1Ht4srckeFQ7k5kVE3pKnv2mXcjjN3k0m8Kk"                    // the secret key generated from firebase
#define WIFI_SSID "DP"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "giveme100bucks"                                    //password of wifi ssid
int relay=D4;
void setup() {
  Serial.begin(9600);
//  pinMode(pin,INPUT);
pinMode(relay,OUTPUT);
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to firebase
  
}

void loop() {
String path="/";
FirebaseObject object=Firebase.get(path);
String W=object.getString("usr/admin/Status");
Serial.println(W);
if(W=="STABLE")
{
    String f1 = "NOT PROCESSING";   
  Serial.print("Work Status:");  
  Serial.print("NOT PROCESSING");
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Work Status",f1 ); 
    digitalWrite(relay,HIGH);
  delay(2000);
}
else if(W=="UNSTABLE")
{
    String f1 = "PROCESSING";   
  Serial.print("Work Status:");  
  Serial.print("PROCESSING");
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Work Status",f1 );
  digitalWrite(relay,LOW);
  delay(1000);
  digitalWrite(relay,HIGH) ;
  delay(90000);
}
else
{
    String f1 = "CHECK IMMEDIATELY";   
  Serial.print("Work Status:");  
  Serial.print("CHECK IMMEDIATELY");
  Serial.println(" ");                                       
  delay(5);
  Firebase.setString("/usr/admin/Work Status",f1 );
  digitalWrite(relay,HIGH);
  delay(2000);
}
}
