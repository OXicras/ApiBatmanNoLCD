#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
//#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

//LiquidCrystal_I2C lcd (0x27, 20, 4);
String pedirParaApi(int);

const int pinBotao = 0;
int contador = 1;
const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
String enderecoAPI = "https://api.batmanapi.com/v1/characters/";


JsonDocument doc;

void printApiLcd();
void printApiSerial();

void setup() 
{
  Serial.begin(115200);
  //lcd.init();
  //lcd.backlight();
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);
  pinMode(pinBotao, INPUT_PULLUP);

  //lcd.setCursor(0,0);
  Serial.println();
  while(WiFi.status() != WL_CONNECTED)
  {
    //lcd.print("#");
    Serial.print("#");
    delay(500);
  }
  Serial.println();
  //lcd.clear();

  String batmanApi = pedirParaApi(contador);
  DeserializationError erroJson = deserializeJson(doc, batmanApi);

  if(erroJson)
  {
    Serial.printf("Erro ao deserializar: %s\n\r", erroJson.c_str());
    return;
  }  
  //printApiLcd();
  printApiSerial();
}

void loop() 
{
  bool estadoBotao = digitalRead(pinBotao);
  static bool estadoAnterior = estadoBotao;
  if(estadoBotao != estadoAnterior)
  {
    if(!estadoBotao)
    {
      contador++;
      String batmanApi = pedirParaApi(contador);

      DeserializationError erroJson = deserializeJson(doc, batmanApi);
      if(erroJson)
      {
        Serial.printf("Erro ao deserializar: %s\n\r", erroJson.c_str());
        return;
      }
      //printApiLcd();
      printApiSerial();
    }
  }
  estadoAnterior = estadoBotao;
}

String pedirParaApi(int codigo)
{
  HTTPClient http;
  http.begin(enderecoAPI + codigo + "/");
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

void printApiLcd()
{
  // const char* nome = doc["data"]["attributes"]["name"];
  // const char* apelido = doc["data"]["attributes"]["alias"];
  // bool vivo = doc["data"]["attributes"]["alive"];
  // const char* cargo = doc["data"]["attributes"]["role"];
  
  // lcd.clear();
  // lcd.setCursor(0,0);
  // lcd.printf("Nome: %s", nome);
  // lcd.setCursor(0,1);
  // lcd.printf("Apelido: %s", apelido);
  // lcd.setCursor(0,2);
  // lcd.printf("Status: %s", vivo ? "Vivo" : "Morto");
  // lcd.setCursor(0,3);
  // lcd.printf("Cargo: %s", cargo);
}

void printApiSerial()
{
  const char* nome = doc["data"]["attributes"]["name"];
  const char* apelido = doc["data"]["attributes"]["alias"];
  bool vivo = doc["data"]["attributes"]["alive"];
  const char* cargo = doc["data"]["attributes"]["role"];

  Serial.printf("Nome: %s\n\r", nome);
  Serial.printf("Apelido: %s\n\r", apelido);
  Serial.printf("Status: %s\n\r", vivo ? "Vivo" : "Morto");
  Serial.printf("Cargo: %s\n\r", cargo);
}