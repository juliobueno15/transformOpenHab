#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "wifiSetup.hpp"
//#include <WiFiClientSecure.h>

#define MQTT_MSG_SIZE 50
#define MQTT_TOPIC_SIZE 50

class MqttSetup{
  private:
    WiFiClient _espClient;
    //unsigned int _pubs_count;
    //unsigned int _subs_count;
    std::vector<String> _subs;
    wifiSetup* _wifi { nullptr };
  public:
    MqttSetup(const uint8_t *mqtt_broker_ip, uint16_t port, MQTT_CALLBACK_SIGNATURE);
    MqttSetup(IPAddress mqtt_broker_ip, uint16_t port, MQTT_CALLBACK_SIGNATURE);
    MqttSetup(const char *mqtt_broker_domain, uint16_t port, MQTT_CALLBACK_SIGNATURE);
    //~ MqttSetup();
    PubSubClient MQTT_client { PubSubClient(_espClient) };
    char MQTT_msg[MQTT_MSG_SIZE];
    void checkConnection();
    void checkMsgs();
    void reconnect();
    void addSubscription(const char *topic, const char *topic_modifier = "");
    void publish(const char *topic, const char *payload);
    void setWifi(wifiSetup* wifi);
};
#endif
