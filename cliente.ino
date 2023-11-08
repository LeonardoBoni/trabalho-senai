//==================================================================================//
// CLIENT                                                                           //
//----------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------
// Bibliotecas necessárias
//------------------------------------------------------------------------------------
#include <SPI.h>
  #include <DHT.h>



#include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------


#define DHTPIN 2  // Pino que estamos conectados (GPIO2 no ESP-01)
#define DHTTYPE DHT22  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//------------------------------------------------------------------------------------
// WIFI Criado no Server
//------------------------------------------------------------------------------------
char ssid[] = "FVML";
char pass[] = "fvml1234";

//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// IP FIXO DO SERVER -- CASO TROCAR NO SERVER, DEVE TROCAR AQUI TAMBEM
//------------------------------------------------------------------------------------
IPAddress server(192,168,10,40);
WiFiClient client;
//CÓDIGO DHT==========================================================================



//====================================================================================

void setup() {
  //Criação de comunicação serial para debug
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Aguardando conexão: ");
  WiFi.begin(ssid, pass);
  dht.begin();

  //Enquanto não conectar vai ficar preso aqui
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  
//------------------------------------------------------------------------------------
// Parametros da rede - Somente para debug - Pode apagar
//------------------------------------------------------------------------------------
  Serial.println("");
  Serial.println("ESP Client Connected");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
}

//====================================================================================

void loop() {


  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Testa se o retorno é válido, caso contrário, algo está errado.
  if (isnan(t) || isnan(h)) {
    Serial.println("Falha ao ler o sensor DHT");
  }
  ContinuousConnection();
}

//====================================================================================

 void ContinuousConnection(){
  client.connect(server, 80);                          // Conexão com o server
  EnviarDados();                                        // Enviar dados
 }

//====================================================================================

void ClientContinue(){
  //Recebe a mensagem do Server - lembrar sempre de mandar a mensagem do server com o "\n"
  String answer = client.readStringUntil('\r');
  client.flush();

  //Verifica se a variavel answer não está vazia 
  if (!answer.isEmpty()) {
    //Mostra a mensagem recebida
    Serial.println("Data Received: " + answer);
    delay(200);
  }
}
 
//====================================================================================

void EnviarDados() {
    //Envia "I am Client" para o server
    float h = dht.readHumidity();
  float t = dht.readTemperature();
    int hum=h;
    int temp=t;
    client.println("\nUmidade: "+String(h)+"%"+"\nTemperatura: "+String(t)+"*C");
   
    //Receber dados do server
    /*client.println("DADOS RECEBIDOS:");
    client.print("UMIDADE:");
    client.print(hum);
    client.println("%");
    client.print("TEMPERATURA");
    client.print(temp);
    client.println("*C");
    */
    ClientContinue();
    
}   
