#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include <esp_sntp.h>

String pedirParaApi(String);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://api.batmanapi.com/v1/characters/1";

//TODO =========== Servidores ===========
const char* NTP1 = "a.st1.ntp.br";
const char* NTP2 = "pool.ntp.org";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

  Serial.println();
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("#");
  }
  Serial.println();
}

void loop() 
{
  unsigned long now = millis();
  static unsigned long tempoInicial = 0;
  const long umMinuto = 60000;
}

String pedirParaApi(String codigo)
{
  HTTPClient http;
  http.begin(enderecoAPI + codigo);
  
  int codigoHTTP = http.GET();
  String resposta;

  if(codigoHTTP == 200)
  {
    resposta = http.getString();
  }
  else
  {
    Serial.printf("\n\rDeu erro. Código: %d", codigoHTTP);
  }
  http.end();
  return resposta;
}