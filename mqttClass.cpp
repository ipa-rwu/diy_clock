#include "mqttClass.hpp"

MQTTClass::MQTTClass(Client& wifiClient):PubSubClient(wifiClient)
{
  _topic_adjuse_time = "/clock/adjust";
  _topic_color = "/clock/color";
  _topic_on_off = "/clock/onoff";
}

void MQTTClass::setup(const char* serverIP, int port, const char* myId) {
  this->_serverIPAddress.fromString(serverIP);
  this->_serverPort = port;
  setServer(_serverIPAddress, _serverPort);
  Serial.print("serverIP: ");
  Serial.print(serverIP);
  Serial.print(" port: ");
  Serial.print(port);
    
  strncpy(_id, myId, MAX_SIZE_MY_ID);
  Serial.print(" id: ");
  Serial.println(_id);
  
  if (!this->connect(_id)) {
    Serial.print("failed, rc=");
    Serial.print(this->state());
    Serial.println(" will connect later");
  }
  else{
    Serial.println("connected to mqtt");
    publish("esp/reconnect", "hello"); //Topic names
    subscribe(_topic_adjuse_time);
    subscribe(_topic_color);
    subscribe(_topic_on_off);
  }
}

bool MQTTClass::reconnect() {
  if (this->connect(_id)) {
    // Once connected, publish an announcement...
    Serial.print("connected to mqtt");
    Serial.println();
    subscribe(_topic_adjuse_time);
    subscribe(_topic_color);
    subscribe(_topic_on_off);
  }
  else
  {
    Serial.print("Not connect to mqtt, try again");
    Serial.println();
  }
  return connected();
}


void MQTTClass::onCallback(char* topic, byte* payload, unsigned int length) 
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
