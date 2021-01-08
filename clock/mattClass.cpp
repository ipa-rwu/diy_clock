#include "mqttClass.hpp"

MQTTClass::MQTTClass(const char* client_name, const char* mqttServer, String payload): _client_name(client_name), _mqttServer(mqttServer), _payload(payload)
{
  _topic_adjuse_time = "/clock/adjuse";
  _topic_color = "/clock/color";
  _topic_on_off = "/clock/onoff";
  PubSubClient client(_espClient);
}

void MQTTClass::setup_mqtt()
{
  client.setServer(_mqttServer, 1883);
  client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->_callback(topic, payload, length); });
}

boolean MQTTClass::reconnect() {
  if (client.connect(_client_name)) {
    // Once connected, publish an announcement...
    client.publish("esp/reconnect", "hello"); //Topic name
    client.subscribe(_topic_adjuse_time); 
    client.subscribe(_topic_on_off);  
  }
  return client.connected();
}

void MQTTClass::_callback(char* topic, byte* payload, unsigned int length) 
{
  if (strcmp(topic, _topic_adjuse_time) == 0 || strcmp(topic, _topic_on_off) == 0 || strcmp(topic, _topic_color) == 0 )
  {
    payload[length] = '\0';
    //Serial.print("Message:");
    for (int i = 0; i < length; i++) {
      Serial.println((char)payload[i]);
    }
    _payload = (char *)payload;
    Serial.print("_payload: ");
    Serial.println(_payload);   
  }

  Serial.println();
  Serial.println("-----------------------");
 
}
