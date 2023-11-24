#include <ESP8266WiFi.h>
#include <DHT.h>

char ssid[] = "FVML";
char pass[] = "fvml1234";

WiFiServer server(80);

IPAddress ip(192, 168, 10, 40);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

DHT dht(2, DHT22);

float umidade = 0;
float temperatura = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, pass);
  delay(500);
  server.begin();

  Serial.println("ESP Server Initialized");
  Serial.print("IP: "); Serial.println(WiFi.softAPIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/update") != -1) {
    if (request.indexOf("humidity=") != -1) {
      umidade = request.substring(request.indexOf("humidity=") + 9).toFloat();
    }

    if (request.indexOf("temperature=") != -1) {
      temperatura = request.substring(request.indexOf("temperature=") + 12).toFloat();
    }

    // Aqui, estamos apenas imprimindo os valores recebidos
    Serial.print("Umidade recebida: ");
    Serial.println(umidade);
    Serial.print("Temperatura recebida: ");
    Serial.println(temperatura);
  }

   String html = "<!DOCTYPE html><html lang=\"en\"><head>";
  html += "<link rel=\"stylesheet\" href=\"style.css\">";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<title>Sistema de monitoramento de compressores</title>";
  html += "</head><body>";
  html += "<header>";
  html += "<div class=\"cabecalho\">";
  html += "<input type=\"button\" class=\"botao-home\" value=\"HOME\" onclick=\"home()\">";
  html += "<h1 class=\"titulo-cabecalho\"><strong>Sistema de monitoramento dos compressores de ar do SENAI-Araçatuba.</strong></h1>";
  html += "</div>";
  html += "</header>";
  html += "<section class=\"corpo-compressor\">";
  html += "<h1 class=\"titulo-corpo-compressor\">Compressor 1 selecionado!</h1>";
  html += "<input type=\"button\" class=\"botao-compressor\" value=\"ATUALIZAR\" onclick=\"refresh('LIGADO', 10, 100, 50)\">";
  html += "<p class = \"texto-corpo-compressor\">";
  html += "O compressor está com uma umidade de :   " + String(umidade) + "% 💦</p>";
  html += "O compressor está com uma temperatura de: " + String(temperatura) + "°C</p>";
  html += "</section>";
  html += "<img src=\"senai logo.png\" class=\"imagem-logo\" alt=\"logo-senai\">";
  html += "</section>";
  html += "<footer class=\"rodape\">";
  html += "<p class=\"texto-rodape\">";
  html += "&copy;Produzido pelos alunos de Eletroeletrônica 2023";
  html += "</p>";
  html += "</footer>";
  html += "</body>";
  html += "<script src=\"atalho.js\"></script>";
  html += "</html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  delay(200);
}
