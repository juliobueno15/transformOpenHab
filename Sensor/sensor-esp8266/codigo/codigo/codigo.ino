// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <Arduino.h>

#include <mqttSetup8266.hpp>
#include <wifiSetup8266.hpp>
#include <tempo8266.hpp>

#include <ArduinoJson.h>

#include "credencials.h"


#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


// Initialize DHT sensor
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01

DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266
//Declaração do objeto que será usado para controle da leitura do sensor DHT

//================================== Wifi Controller ========================================

//Criação do objeto responsavel pelo wifi
  wifiSetup wifiClient;

//====================================================================================END WIFI

//================================== MQTT Controller ========================================
  const char* topico_umidade = "umidadeInterna";
  const char* topico_temperatura = "temperaturaInterna";


void mqtt_callback(char* topic, byte* payload, unsigned int length){

    }

//parameters defined in credencials.h
MqttSetup mqttClient(mqtt_broker_ip, mqtt_port, mqtt_callback);

//====================================================================================END MQTT

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11 Sensor!"));

// SETUP WIFI
    wifiClient.connect(ssid, password); //conexão com Wifi

// SETUP TIME
    setup_time();  // sincroniza o relogio interno com o do servidor ntp

// SETUP MQTT

  mqttClient.setWifi(&wifiClient);  //Objeto do wifi eh passado para verificação por parte do objeto cliente MQTT, para checagem de conexão de ambos no loop


  dht.begin();    //Configuração do sensor DHT(Temperatura e Humidade)

  pinMode(4, INPUT); //Pino do DHT
}


void loop() {
  //Checagem da conexão com o broker e com a rede Wifi, se ambos tiverem conectados, o código segue, caso contrário, tenta reconexão
  mqttClient.checkConnection();
  //Checa se chegaram msgs em algum tópico em que está inscrito, no caso nenhum
  mqttClient.checkMsgs();
  // Wait a few seconds between measurements.
  delay(10000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //buffer auxiliar para publicar nos topicos do MQTT, já que não eh possivel publicar JSON diretamente
  char buffer_aux[50];

  StaticJsonDocument<50> doc_umidade;
  doc_umidade["humidity"] = h;
  serializeJson(doc_umidade, buffer_aux);
  mqttClient.publish(topico_umidade, buffer_aux);

   StaticJsonDocument<50> doc_temp;
  doc_temp["temperature"] = t;
  serializeJson(doc_temp, buffer_aux);
  mqttClient.publish(topico_temperatura, buffer_aux);


  yield();
}
