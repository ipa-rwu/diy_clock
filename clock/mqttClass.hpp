#ifndef __MQTT_HPP__
#define __MQTT_HPP__
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <string>


class MQTTClass
{
  public:
    MQTTClass(const char* client_name, const char* mqttServer);
    ~MQTTClass()
    {
    };
    
    void setup_mqtt();
    boolean reconnect();
    PubSubClient *clientMQTT;
    String str_payload;
  private:
  
    WiFiClient _espClient;
    
    const char *_topic_adjuse_time, *_topic_on_off, *_topic_color;
    const char* _mqttServer, *_client_name;
    const int _mqttPort = 1883;
    void _callback(char* topic, byte* payload, unsigned int length);

};


#endif __MQTT_HPP__
