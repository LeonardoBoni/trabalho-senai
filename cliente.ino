#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 2  // Pino que estamos conectados (GPIO2 no ESP-01)
#define DHTTYPE DHT22  // DHT 22

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "FVML";
char pass[] = "fvml1234";

IPAddress server(192, 168, 10, 40);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Aguardando conexão: ");
  WiFi.begin(ssid, pass);
  dht.begin();

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("ESP Client Connected");
  Serial.print("IP: ");       Serial.println(WiFi.localIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) {
    Serial.println("Falha ao ler o sensor DHT");
    return;
  }

  ContinuousConnection(h, t);
  delay(5000); // Atraso entre leituras
}

void ContinuousConnection(float h, float t) {
  if (!client.connect(server, 80)) {
    Serial.println("Falha na conexão com o servidor");
    return;
  }
  
  Serial.println("Conectado ao servidor");

  client.print("GET /update?humidity=");
  client.print(h);
  client.print("&temperature=");
  client.print(t);
  client.println(" HTTP/1.1");
  client.println("Host: 192.168.10.40");
  client.println("Connection: close");
  client.println();
  delay(10); // Aguarda resposta do servidor

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }

  client.stop();
}
