#include "mqttClass.hpp"

MQTTClass::MQTTClass(const char* client_name, const char* mqttServer): _client_name(client_name), _mqttServer(mqttServer)
{
  _topic_adjuse_time = "/clock/adjust";
  _topic_color = "/clock/color";
  _topic_on_off = "/clock/onoff";

  PubSubClient _clientMQTT(_espClient);
  clientMQTT = &_clientMQTT;
}

void MQTTClass::setup_mqtt()
{
  clientMQTT->setServer(_mqttServer, 1883);
  clientMQTT->setCallback([this] (char* topic, byte* payload, unsigned int length) { this->_callback(topic, payload, length); });
  Serial.print("Will try to MQTT server");
  Serial.println();
}

boolean MQTTClass::reconnect() {
  if (clientMQTT->connect(_client_name)) {
    // Once connected, publish an announcement...
    Serial.print("connected to mqtt");
    Serial.println();
    clientMQTT->publish("esp/reconnect", "hello"); //Topic name
    clientMQTT->subscribe(_topic_adjuse_time); 
    clientMQTT->subscribe(_topic_on_off);  
  }
  else
  {
    Serial.print("Not connect to mqtt, try again");
    Serial.println();
  }
  return clientMQTT->connected();
}

void MQTTClass::_callback(char* topic, byte* payload, unsigned int length) 
{
  if (strcmp(topic, _topic_adjuse_time) == 0 || strcmp(topic, _topic_on_off) == 0 || strcmp(topic, _topic_color) == 0 )
  {
    payload[length] = '\0';
    //for (int i = 0; i < length; i++) {
    //  Serial.println((char)payload[i]);
    //}
    str_payload = (char *)payload; 
  }
 
}
