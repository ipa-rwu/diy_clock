#include <Arduino.h>
#include "gettimeNTP.h"
#include <TimeLib.h>
#include "numberClass.hpp"
#include "mqttClass.hpp"

#define Test 1

int i=0;
time_t prevDisplay = 0; // when the digital clock was displayed
struct TimeStruct time_split;
int premin = 0;
int nowmin = 0;

//time_t current_time;
time_t on_time = 0;
time_t off_time = 0;
bool flag = 0;

// MQTT
const char* mqttServer = "192.168.0.53";
//"192.168.179.57";
const char* client_name = "clock_client";
String payload = "", half_payload = "";
long lastReconnectAttempt = 0;
int time_diff = 0, flag_on = 0;


NumberClass *number_obj = new NumberClass;
NumberClass::colorHSV color_hsv;
MQTTClass *mqtt_obj = new MQTTClass(client_name, mqttServer, payload);

void setup() {
  Serial.begin(115200);
  color_hsv.h = 39;
  color_hsv.s = 92;
  color_hsv.v = 100;
  number_obj->setup_led();

  mqtt_obj->setup_mqtt();

  SetupTime();
  
  // setup_led();
  number_obj->fade_all();

  lastReconnectAttempt = 0;
 
}


void loop() 
{ 
  if (!mqtt_obj->client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_obj->reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }
  else
  {
    if (payload != "")
    {
      half_payload = getValue(payload, ':', 1);
      if ((getValue(payload, ':', 0)) == "onoff"){
        flag_on = half_payload.toInt();
      }

      if ((getValue(payload, ':', 0)) == "adjuse"){
        time_diff = half_payload.toInt();
      }
      if ((getValue(payload, ':', 0)) == "color"){
        color_hsv.h = getValue(half_payload, ',', 0).toInt();
        color_hsv.s = getValue(half_payload, ',', 1).toInt();
        color_hsv.v = getValue(half_payload, ',', 2).toInt();
      }

      payload = "";
      half_payload = "";
    }
    
    if (timeStatus() != timeNotSet)
    {
      if (now() != (prevDisplay)) 
      { 
        //update the display only if time has changed
        Serial.print(prevDisplay);
        Serial.print("\n");
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
          
          if (premin != nowmin)
          {

            number_obj->fade_number(0, 21*2);
            number_obj->fade_number(2+2*21, 21*2);
            
            number_obj->show_number(time_split.hour_1,0, color_hsv);
            number_obj->show_number(time_split.hour_2,1*21, color_hsv);
         
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
    mqtt_obj->client.loop();
   }
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
