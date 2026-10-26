#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include <esp_sntp.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd (0x27, 20, 4);
String pedirParaApi();

JsonDocument doc;

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://api.batmanapi.com/v1/characters/1";
String batmanApi = pedirParaApi();

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);
  lcd.init();
  lcd.backlight();

  deserializeJson(doc, batmanApi);

  lcd.setCursor(0,0);
  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.print("#");
    delay(500);
  }
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.printf("Nome: %s", doc["name"]);
  lcd.setCursor(0,1);
  lcd.printf("Apelido: %s", doc["alias"]);
  lcd.setCursor(0,2);
  lcd.printf("Status: %s", doc["alive"]);
  lcd.setCursor(0,3);
  lcd.printf("Cargo: %s", doc["role"]);
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