#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <iostream>

LiquidCrystal_I2C lcd (0x27, 20, 4);
String pedirParaApi(int);

const int pinBotao = 0;
const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://api.batmanapi.com/v1/characters/";

void setup() 
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);
  pinMode(pinBotao, INPUT_PULLUP);

  lcd.setCursor(0,0);
  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.print("#");
    delay(500);
  }
  lcd.clear();

  String batmanApi = pedirParaApi(1);

  JsonDocument doc;
  DeserializationError erroJson = deserializeJson(doc, batmanApi);

  if(erroJson)
  {
    Serial.printf("Erro ao deserializar: %s\n\r", erroJson.c_str());
    return;
  }

  const char* nome = doc["data"]["attributes"]["name"];
  const char* apelido = doc["data"]["attributes"]["alias"];
  bool vivo = doc["data"]["attributes"]["alive"];
  const char* cargo = doc["data"]["attributes"]["role"];

  lcd.setCursor(0,0);
  lcd.printf("Nome: %s", nome);
  lcd.setCursor(0,1);
  lcd.printf("Apelido: %s", apelido);
  lcd.setCursor(0,2);
  lcd.printf("Status: %s", vivo ? "Vivo" : "Morto");
  lcd.setCursor(0,3);
  lcd.printf("Cargo: %s", cargo);
}

void loop() 
{
  static int contador = 1;
  bool estadoBotao = digitalRead(pinBotao);
  static bool estadoAnterior = estadoBotao;

  if(estadoBotao != estadoAnterior)
  {
    if(!estadoBotao)
    {
      contador++;
      lcd.clear();
      estadoAnterior = estadoBotao;
      String batmanApi = pedirParaApi(contador);

      JsonDocument doc;
      DeserializationError erroJson = deserializeJson(doc, batmanApi);

      if(erroJson)
      {
        Serial.printf("Erro ao deserializar: %s\n\r", erroJson.c_str());
        return;
      }

      const char* nome = doc["data"]["attributes"]["name"];
      const char* apelido = doc["data"]["attributes"]["alias"];
      bool vivo = doc["data"]["attributes"]["alive"];
      const char* cargo = doc["data"]["attributes"]["role"];

      lcd.setCursor(0,0);
      lcd.printf("Nome: %s", nome);
      lcd.setCursor(0,1);
      lcd.printf("Apelido: %s", apelido);
      lcd.setCursor(0,2);
      lcd.printf("Status: %s", vivo ? "Vivo" : "Morto");
      lcd.setCursor(0,3);
      lcd.printf("Cargo: %s", cargo);
    }
  }
}

String pedirParaApi(int codigo)
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