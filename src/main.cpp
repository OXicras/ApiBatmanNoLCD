#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include <esp_sntp.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 20, 4);
String pedirParaApi();

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://api.batmanapi.com/v1/characters/1";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);
  lcd.init();
  lcd.backlight();

  Serial.println();
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("#");
    delay(500);
  }
  Serial.println();
  String batmanApi = pedirParaApi();
  Serial.println(batmanApi);
}

void loop() {}

String pedirParaApi()
{
  HTTPClient http;
  http.begin(enderecoAPI);
  
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