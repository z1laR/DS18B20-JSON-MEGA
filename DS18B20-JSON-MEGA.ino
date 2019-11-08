#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "RestClient.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define IP "192.168.0.102"
#define PORT 3000

const int pinDatosDQ = 2;
String postTemperatura = "";

RestClient client = RestClient(IP, PORT);

OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature temperaturaCorporal(&oneWireObjeto);
 
void setup()
{
  Serial.begin(9600);
  temperaturaCorporal.begin();

  Serial.println("Conectado a la red:");
  client.dhcp();

  Serial.print("IP de Placa Ethernet Shield:");
  Serial.println(Ethernet.localIP());
  
  Serial.println("Listo :D!");
}
 
void loop()
{  
  temperaturaCorporal.requestTemperatures();

  int temp = temperaturaCorporal.getTempCByIndex(0);
  
  Serial.println("");
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.println("");

  delay(2500);

  client.setHeader("Content-Type: application/json");

  StaticJsonBuffer<200> jsonBuffer;
  char json[256];  
  JsonObject& root = jsonBuffer.createObject();
  root["temperatura"] = temp;
  root.printTo(json, sizeof(json));
  Serial.println(json);

  int statusCode = client.post("http://192.168.0.102:3000/api/temperatura", json, &postTemperatura);

  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("postTemperatura body from server: ");
  Serial.println(postTemperatura);

  postTemperatura = "";

  delay(5000);

  
}
