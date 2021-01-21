#include <Arduino.h>
#include "gettimeNTP.hpp"
#include <TimeLib.h>
#include "numberClass.hpp"
#include "mqttClass.hpp"
#include "secret.hpp"

#define Test 1

int i=0;
time_t prevDisplay = 0; // when the digital clock was displayed
struct TimeStruct  time_split;
int premin = 0;
int nowmin = 0;

//time_t current_time;
time_t on_time = 0;
time_t off_time = 0;
bool flag = 0;
int time_h = 0;

// MQTT
WiFiClient espClient;
const char* mqtt_server_ip = "192.168.0.52";
const int   mqtt_server_port = 1883;
const char* mqtt_client_name = "clock_client";
MQTTClass *mqtt_obj = new MQTTClass(espClient);
String half_payload = "";
int time_diff = 0, flag_on = 0;
long lastReconnectAttempt_MQTT;
long lastReconnectAttempt_Wifi;

NumberClass *number_obj = new NumberClass;
NumberClass::colorHSV color_hsv;

void setup() {
  Serial.begin(115200);

  int try_wifi = 0;
  // connect to wifi
  WiFi.begin(_ssid, _password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED && try_wifi <=3) {
    try_wifi++;
    delay(1000);
    Serial.println("Connecting to WiFi..");
  } 
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Not Connected to the WiFi network");
  }
  else{
    Serial.println("Connected to the WiFi network");

  }
  
  color_hsv.h = 50;
  color_hsv.s = 255;
  color_hsv.v = 255;
  number_obj->setup_led();

  mqtt_obj->setup(mqtt_server_ip, mqtt_server_port, mqtt_client_name);

  SetupTime();
  
  // setup_led();
  number_obj->fade_all();

  lastReconnectAttempt_MQTT = 0;
  lastReconnectAttempt_Wifi = 0;
 
}


void loop() 
{ 
  long now_now = millis();
  // reconnect to wifi
  if ((WiFi.status() != WL_CONNECTED))
  {
    if ((now_now - lastReconnectAttempt_Wifi) > 5000)
    {
      lastReconnectAttempt_Wifi = now_now;
      WiFi.begin(_ssid, _password);
      if ((WiFi.status() == WL_CONNECTED))
      {
        lastReconnectAttempt_Wifi = 0;
      }
    }
  }

  // reconnect to mqtt
  if ((WiFi.status() == WL_CONNECTED))
  {
    if (!mqtt_obj->connected()) {
      if ((now_now - lastReconnectAttempt_MQTT) > 5000) {
        lastReconnectAttempt_MQTT = now_now;
        // Attempt to reconnect
        Serial.println("reconnet to mqtt");
        if (mqtt_obj->reconnect()) {
          lastReconnectAttempt_MQTT = 0;
        }
      }
    }
  }
  
  if(mqtt_obj->connected()){
    Serial.print(mqtt_obj->str_payload);
    if (mqtt_obj->str_payload != "")
    {
      Serial.print(mqtt_obj->str_payload);
      Serial.println();
      half_payload = getValue(mqtt_obj->str_payload, ':', 1);
      if ((getValue(mqtt_obj->str_payload, ':', 0)) == "onoff"){
        flag_on = half_payload.toInt();
        Serial.print("switch to: ");
        Serial.print(flag_on);
        Serial.println();
      }

      if ((getValue(mqtt_obj->str_payload, ':', 0)) == "adjust"){
        time_diff = half_payload.toInt();
        Serial.print("adjuse time: ");
        Serial.print(time_diff);
        Serial.println();
      }
      if ((getValue(mqtt_obj->str_payload, ':', 0)) == "color"){
        Serial.print("hsv: ");
        Serial.print(half_payload);
        Serial.println();

        color_hsv.h = getValue(half_payload, ',', 0).toInt();
        color_hsv.s = getValue(half_payload, ',', 1).toInt();
        color_hsv.v = getValue(half_payload, ',', 2).toInt();
        Serial.print("h: ");
        Serial.print(color_hsv.h);
        Serial.println();
        Serial.print("s: ");
        Serial.print(color_hsv.s);
        Serial.println();
          Serial.print("v: ");
        Serial.print(color_hsv.v);
        Serial.println();
      }

      half_payload = "";
      mqtt_obj->str_payload = "";
    }
  }
    
    if (timeStatus() != timeNotSet)
    {
      if (now() != (prevDisplay)) 
      { 
        //update the display only if time has changed
        //Serial.print(prevDisplay);
        //Serial.print("\n");
        prevDisplay = now();
        time_split = gettime();
        nowmin = time_split.min_1 * 10 + time_split.min_2;        

        if (flag_on == 0)
        {
          number_obj->fade_all();
        }
        else if(flag_on = 1)
        {
          number_obj->blink_dots(2*21, now(), color_hsv);      
          number_obj->fade_number(0, 21*2);
          number_obj->fade_number(2+2*21, 21*2);

          time_h = time_split.hour_1*10 + time_split.hour_2 + time_diff;
          if (time_split.hour_1 == 0 && time_split.hour_2 == 0 && time_diff == -1)
          {
            time_h = 23;
          }
          number_obj->show_number(time_h/10,0, color_hsv);
          number_obj->show_number(time_h%10,1*21, color_hsv);
       
          number_obj->show_number(time_split.min_1,2+2*21, color_hsv);
          number_obj->show_number(time_split.min_2,2+3*21, color_hsv);
          
          if (premin != nowmin)
          {

            number_obj->fade_number(0, 21*2);
            number_obj->fade_number(2+2*21, 21*2);

            time_h = time_split.hour_1*10 + time_split.hour_2 + time_diff;
            number_obj->show_number(time_h/10,0, color_hsv);
            number_obj->show_number(time_h%10,1*21, color_hsv);
         
            number_obj->show_number(time_split.min_1,2+2*21, color_hsv);
            number_obj->show_number(time_split.min_2,2+3*21, color_hsv);
          }

          premin = nowmin;
          // Serial.print(premin);
          //Serial.println();
        }

        //Serial.print(prevDisplay);
        //Serial.print(time_split.hour_1);
        //Serial.print(time_split.hour_2);
        //Serial.print(":");
        //Serial.print(time_split.min_1);
        //Serial.print(time_split.min_2);
        //Serial.println();
      }
    }
    mqtt_obj->loop();
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
