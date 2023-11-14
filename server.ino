#include <SPI.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

char ssid[] = "FVML";
char pass[] = "fvml1234";

WiFiServer server(80);

IPAddress ip(192, 168, 10, 40);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

DHT dht(2, DHT22); // Use o mesmo pino que você usou no cliente

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

  Serial.print("Data Received: "); 
  Serial.println(request);

  // Gere uma página HTML com os dados do sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

   String html = "<html><head>";
    html += "<style>";
    html += "body { background-color: read; text-align: center; }";
    html += "h1 { color: white; }";
    html += "p { font-size: 30px; }";
    html += "</style>";
    html += "</head><body>";
    html += 
   
"<h1>Dados do Sensor</h1>";
    html += "<p>Umidade: " + String(h) + "%</p>";
    html += 
   
"<p>Temperatura: " + String(t) + "*C</p>";
    html += 
    html +=

    html;
"</body></html>";

  // Envie a página HTML como resposta
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  delay(200);
}
