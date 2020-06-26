// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <Arduino.h>

#include "mqttSetup.hpp"
#include "wifiSetup.hpp"
#include "tempo.hpp"

#include <ArduinoJson.h>

#include "credencials.h"

//============================ DHT CONFIG =====================================
#define LIGHT_SENSOR_PIN 34
#define DHTPIN 4     // Digital pin connected to the DHT sensor
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

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
  DHT dht(DHTPIN, DHTTYPE);
//Declaração do objeto que será usado para controle da leitura do sensor DHT
//=====================================================================================END DHT

//=================================== DEEP SLEEP CONFIG =====================================

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 300 /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR float temperature = 0;    //VARIÁVEIS SALVAS NA RTC MEMORY
RTC_DATA_ATTR float humidity    = 0;    //a flag RTC_DATA_ATTR faz com que essas variáveis fiquem presentes mesmo após o deepSleep
RTC_DATA_ATTR float light       = 0;    //The ESP32 has 8kB SRAM on the RTC part, called RTC fast memory


//===============================================================================END DEEPSLEEP

//================================== Wifi Controller ========================================

//Criação do objeto responsavel pelo wifi
  wifiSetup wifiClient;

//====================================================================================END WIFI

//================================== MQTT Controller ========================================
  const char* topico_umidade = "umidade";
  const char* topico_temperatura = "temperatura";
  const char* topico_luminosidade = "luminosidade";

void mqtt_callback(char* topic, byte* payload, unsigned int length){
    }

//parameters defined in credencials.h
MqttSetup mqttClient(mqtt_broker_ip, mqtt_port, mqtt_callback);

//====================================================================================END MQTT

//Em deepSleep todo o código eh feito no Setup, visto que o código nunca chega ao loop

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11 Sensor!"));

  //variáveis usadas para checagem dos valores, caso algum deles tiver mudado, serão publicados, caso contrário, não, para reduzir o consumo de energia
  float old_temperature = temperature;
  float old_humidity = humidity;
  float old_light = light;
  int flag_change = 0;

// SETUP DHT
  pinMode(DHTPIN, INPUT); //Pino do DHT
  dht.begin();    //Configuração do sensor DHT(Temperatura e Humidade)

  //leitura de temperatura e umidade
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  int flag_error_dht = 0;
  // Check if any reads failed and exit early (if an error ocorred, the ESP32 will sleep).
  if (isnan(humidity) || isnan(temperature) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    flag_error_dht = 1;
  }

  //Checagem do valor da iluminação, cujo sensor está ligado na porta analogica 34.
  light = analogRead(LIGHT_SENSOR_PIN);

  if(!flag_error_dht){
    if(old_temperature != temperature){
      Serial.println("temperature change");
      flag_change = 1;
    }
    else if(old_humidity != humidity){
      Serial.println("umidade change");
      flag_change = 1;
    }
    else if((old_light+40) < light || (old_light-40) > light){
      Serial.println("light change");
      flag_change = 1;
    }
  }


  if(flag_change){
    // SETUP WIFI
        wifiClient.connect(ssid, password); //conexão com Wifi

    // SETUP TIME
        setup_time();  // sincroniza o relogio interno com o do servidor ntp

    // SETUP MQTT
        mqttClient.setWifi(&wifiClient);  //Objeto do wifi eh passado para verificação por parte do objeto cliente MQTT, para checagem de conexão de ambos no loop

    //Checagem da conexão com o broker e com a rede Wifi, se ambos tiverem conectados, o código segue, caso contrário, tenta reconexão
        mqttClient.checkConnection();

      StaticJsonDocument<50> doc_umidade;
      StaticJsonDocument<50> doc_temp;
      StaticJsonDocument<50> doc_lum;

      //buffer auxiliar para publicar nos topicos do MQTT, já que não eh possivel publicar JSON diretamente
      char buffer_aux[50];

      //ATRIBUIÇÃO DOS CAMPOS DO JSON e ENVIO VIA MQTT
      if(!flag_error_dht){
          doc_umidade["humidity"] = humidity;
          serializeJson(doc_umidade, buffer_aux);
          mqttClient.publish(topico_umidade, buffer_aux);
          delay(400);
          doc_temp["temperature"] = temperature;
          serializeJson(doc_temp, buffer_aux);
          mqttClient.publish(topico_temperatura, buffer_aux);
          delay(400);
      }

      doc_lum["light"] = (light/4096)*100;
      serializeJson(doc_lum, buffer_aux);
      mqttClient.publish(topico_luminosidade, buffer_aux);

      //Após essa linha todos os dados já foram enviados,
      delay(400);
  }

  //configuração para colocar o ESP32 em deepSleep e acordar após 300s
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep now");
  delay(300);
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");

}

void loop() {

}
