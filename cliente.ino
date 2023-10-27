//==================================================================================//
// CLIENT                                                                           //
//----------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------
// Bibliotecas necessárias
//------------------------------------------------------------------------------------
#include <SPI.h>

#include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// WIFI Criado no Server
//------------------------------------------------------------------------------------
char ssid[] = "FVML";
char pass[] = "fvml1234";


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
    client.print("I am Client\r");
   
    //Receber dados do server
    ClientContinue();
    
}   
