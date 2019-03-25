#include <NTPClient.h>
#include "ESP8266WiFi.h"
#include <WiFiUdp.h>
#include <FC16.h>

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}

const int csPin = D4;            // CS pin used to connect FC16
const int displayCount = 4;         // Number of displays; usually 4 or 8
const int scrollDelay = 30;    // Scrolling speed - pause in ms

FC16 display = FC16(csPin, displayCount); // Define display parameters

static const char* ssid = "ChulaWiFi";
static const char* username = "61702784";
static const char* password = "Kengkkun1234";
const char* host = "host_ip";

const long utcOffsetInSeconds = 25200;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tu", "Wed", "Th", "Fri", "Sat"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.ku.ac.th", utcOffsetInSeconds);


void setup(){
  Serial.begin(115200);
  
  display.begin();      // turn on display
  display.setIntensity(1);  // set medium brightness
  display.clearDisplay();   // turn all LED off
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
   // Setting ESP into STATION mode only (no AP mode or dual mode)
  wifi_set_opmode(0x01);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);

  wifi_station_set_config(&wifi_config);

  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();

  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));

  wifi_station_connect();
  Serial.print("Wifi station connect status:");
  Serial.println(wifi_station_get_connect_status());

  // Wait for connection AND IP address from DHCP
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(2000);
    Serial.println("Not connected");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  delay(1000);
  timeClient.begin();
  display.setText("Welcome");
  
}

void loop()
{
 // bool done;
 // done = display.update();
 // delay(100);
  
 // if(done){
//  for(int i = 30; i >= 0; i--){
  timeClient.update();
  display.setClock(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds());
  delay(1000);
//   } 
 //    done != display.update(); 
 //    display.setText(daysOfTheWeek[timeClient.getDay()]);
//  }
  
 

 
  //delay(scrollDelay);


//  Serial.print(daysOfTheWeek[timeClient.getDay()]);
//  Serial.print(", ");
//  Serial.print(timeClient.getHours());
//  Serial.print(":");
//  Serial.print(timeClient.getMinutes());
//  Serial.print(":");
//  Serial.println(timeClient.getSeconds());
//  Serial.println(timeClient.getFormattedTime());

//  Serial.println(done);
  

}
