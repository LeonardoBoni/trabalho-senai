//==================================================================================//
// SERVER                                                                           //
//----------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------
// Bibliotecas necessárias
//------------------------------------------------------------------------------------
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <DHT.h> // Biblioteca do sensor DHT

//------------------------------------------------------------------------------------
// WIFI
//------------------------------------------------------------------------------------
char ssid2[]= "Automacao";
char pass2[]= "Senaisp901";


char ssid[] = "FVML";
char pass[] = "fvml1234";
WiFiServer server(80);

  IPAddress ip(192, 168, 10, 40);            // IP fixo do server
  IPAddress gateway(192, 168, 10, 1);        // gateway do server
  IPAddress subnet(255, 255, 255, 0);        // subrede do server

//====================================================================================
void setup() {
  //Criação de comunicação serial para debug
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP_STA);                    // envio de comandos por TCP/IP

  //Configurando a rede
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, pass);
  delay(500);
  //Iniciando o Server
  server.begin();

//------------------------------------------------------------------------------------
// Parametros da rede - Somente para debug - Pode apagar
//------------------------------------------------------------------------------------  
  Serial.println("ESP Server Initialized");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
}

void loop() {
  //Verificando se possui um cliente conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //recebe os dados vindo do cliente
  String request = client.readStringUntil('\r');
  client.flush();

  //Mostra os dados vindo do cliente
  Serial.print("Data Received: "); Serial.println(request);
  delay(200);
   String request2 = client.readStringUntil('hum');
   Serial.println(request2);

 

  //Envia os dados para o cliente
  client.print("I am server");
  delay(200);
}
