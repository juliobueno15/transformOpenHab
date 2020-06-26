Para uso do código há apenas a necessidade da mudança do campo SSID e PASSWORD, presentes no arquivo "credencials.h", antes do código ser inserido no ESP32.
(é possível também alterar o brokerMQTT alterando os campos de endereço de IP do mesmo)
Uma nota importante, alguns GPIO do ESP32 tem suas leituras analógicas inutilizáveis quando a antena WIFI está sendo usada, por isso o pino de leitura da informação do sensor de iluminação (DRL) está no pino 34, já que os pinos de 33 a 36 podem ser usados em conjunto ao WiFi.

*Os cabos de cor vermelha no esquemático do circuito correspondem aos ligados em 3v3;
*Os cabos pretos são os ligados em GND;
*Os cabos amarelos e verdes são os cabos de leitura da informação dos sensores

O resistor que liga um dos contatos do sensor de iluminação com o GND tem o valor de 10kOhm

