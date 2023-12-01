#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266WebServer.h>

char ssid[] = "FVML";
char pass[] = "fvml1234";

WiFiServer server(80);

IPAddress ip(192, 168, 10, 40);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

DHT dht(2, DHT22);

float umidade = 0;
float temperatura = 0;
String estadoCompressor = "";
String dataHora = "";
const char* imagemLogo = "/senai_logo.png";

const char* estiloCSS = R"=====(
body {
    background-color: black;
}

.cabecalho {
    align-items: center;
    display: flex;
    border-radius: 10px;
    height: auto;
    width: 900px;
    background-color: red;
    padding : 35px;
}

#data-hora {
    color: white;
    text-align: center;
}

.botao-home {
    margin-right: 20px;
    justify-content: center;
    display: inline-block;
    background-color: black;
    color: white;
    border-radius: 20px;
    height: 50px;
    width: 100px;
    cursor: pointer;
}

.botao-home:hover {
    background: rgb(2, 0, 36);
    background: linear-gradient(90deg, black 0%, red 100%);
}

.botao-home:active {
    transform: translate(0em, 0.2em);
}

.titulo-cabecalho {
    margin: 0 auto;
    text-align: center;
    color: white;
}

.corpo-home {
    text-align: center;
    border-radius: 20px;
    background-color: red;
    height: 500px;
    width: 800px;
    margin-left: auto;
    margin-right: auto;
    margin-top: 80px;
}

.titulo-corpo-home {
    text-decoration: underline;
    padding-top: 70px;
    margin: 0 auto;
    color: white;
    font-size: 40px;
    text-align: center;
}

.texto-corpo-home {
    padding: 30px;
    text-align: center;
    color: white;
    font-size: 25px;
    margin: 0 auto;
}

.botao-corpo {
    margin: 0 30px;
    font-size: 20px;
    width: 200px;
    height: 50px;
    background-color: black;
    color: white;
    border-radius: 20px;
    cursor: pointer;
    display: inline-block;
    justify-content: center;
    text-align: center;
    padding: 10px;
}

.botao-corpo:hover {
    background: rgb(2, 0, 36);
    background: linear-gradient(90deg, black 0%, red 100%);
}

.botao-corpo:active {
    transform: translate(0em, 0.2em);
}

.imagem-logo {
    padding: 40px;
    height: 90px;
}

.rodape {
    padding-top: 50px;
    text-align: center;
    color: white;
}

/* Página dos compressores */

.corpo-compressor {
    text-align: center;
    border-radius: 20px;
    background-color: red;
    height: 550px;
    width: 950px;
    margin: 0, auto;
    margin-top: 80px;
}

.titulo-corpo-compressor {
    text-decoration: underline;
    padding: 50px;
    margin: 0 auto;
    color: white;
    font-size: 60px;
    text-align: center;
    padding: 65px;
    margin-top: 100px;
}

#estado{
  .titulo-corpo-compressor {
    text-decoration: underline;
    padding: 30px;
    margin: 0 auto;
    color: white;
    font-size: 40px;
    text-align: center;
}
}

.botao-compressor {
  margin-top: 150px;
    margin: 0 30px;
    font-size: 20px;
    width: 200px;
    height: 50px;
    background-color: black;
    color: white;
    border-radius: 20px;
    cursor: pointer;
    display: inline-block;
    justify-content: center;
    text-align: center;
    padding: 10px;
}

.botao-compressor:hover {
    background: rgb(2, 0, 36);
    background: linear-gradient(90deg, black 0%, red 100%);
}

.botao-compressor:active {
    transform: translate(0em, 0.2em);
}

.texto-corpo-compressor {
    padding: 30px;
    color: white;
    text-align: center;
    margin: 0 auto;
    font-size: 37px;
    margin-top: 40px;
}

#estado {
    color: white;
    font-size: 25px;
    text-align: center;
    margin: 0 auto;
}

#tempo {
    color: white;
    font-size: 25px;
    text-align: center;
    margin: 0 auto;
}


#mensagem-desligado {
    color: white;
    font-size: 25px;
    text-align: center;
)=====";

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(ssid, pass);
    delay(500);
    server.begin();
    Serial.println("Servidor ESP inicializado");
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("Sinal: ");
    Serial.println(WiFi.RSSI());
}

void loop() {
    WiFiClient client = server.available();

    if (!client) {
        return;
    }

    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/update") != -1) {
        int umidadeIndex = request.indexOf("humidity=");
        int temperaturaIndex = request.indexOf("temperature=");

        if (umidadeIndex != -1) {
            umidade = request.substring(umidadeIndex + 9, request.indexOf('&', umidadeIndex)).toFloat();
        }

        if (temperaturaIndex != -1) {
            temperatura = request.substring(temperaturaIndex + 12).toFloat();
        }

        Serial.print("Umidade recebida: ");
        Serial.println(umidade);
        Serial.print("Temperatura recebida: ");
        Serial.println(temperatura);

        // Atualize a hora atual (você pode usar a lógica específica do seu projeto aqui)
        dataHora = "data e hora atual";

        String jsonResponse = "{\"estado\":\"" + estadoCompressor + "\", \"umidade\":" + String(umidade) + ", \"temperatura\":" + String(temperatura) + ", \"dataHora\":\"" + dataHora + "\"}";
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println(jsonResponse);
        delay(200);
        return;
    }

  String html = "<!DOCTYPE html><html lang=\"en\"><head>";
html += "<style>";
html += estiloCSS;
html += "</style>";
html += "<script>";
html += "const zeroFill = n => ('0' + n).slice(-2);";
html += "const updateData = () => {";
html += "  if (estadoCompressor === 'LIGADO') {"; // Verifica se o compressor está ligado
html += "    const xhr = new XMLHttpRequest();";
html += "    xhr.open('GET', '/update', true);";
html += "    xhr.onload = function () {";
html += "      if (xhr.status === 200) {";
html += "        const data = JSON.parse(xhr.responseText);";
html += "        document.getElementById('estado').innerHTML = data.estado;";
html += "        document.getElementById('umidade').innerHTML = data.umidade + \"% 💦\";";
html += "        document.getElementById('temperatura').innerHTML = data.temperatura + \"°C 🌡️\";";
html += "        document.getElementById('data-hora').innerHTML = data.dataHora;";
html += "        document.getElementById('mensagem-desligado').innerHTML = '';";
html += "      }";
html += "    };";
html += "    xhr.send();";
html += "  }";
html += "};";
html += "const intervalo = setInterval(updateData, 5000);";
html += "function home() { window.location.href = 'index.html'; }";
html += "function estadoon() {estadoCompressor = 'LIGADO'; document.getElementById('estado').innerHTML = estadoCompressor; document.getElementById('mensagem-desligado').innerHTML = ''; }";

html += "function estadooff() { estadoCompressor = 'DESLIGADO'; document.getElementById('estado').innerHTML = estadoCompressor; document.getElementById('umidade').innerHTML = ''; document.getElementById('temperatura').innerHTML = ''; document.getElementById('mensagem-desligado').innerHTML = ''; umidade = 0; temperatura = 0;}";
html += "</script>";


html += "<meta charset=\"UTF-8\">";
html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
html += "<title>Sistema de monitoramento de compressores</title>";
html += "</head><body>";
html += "<header>";
html += "<div class=\"cabecalho\">";
html += "<input type=\"button\" class=\"botao-home\" value=\" HOME\" onclick=\"home()\">";
html += "<h1 class=\"titulo-cabecalho\"><strong>Sistema de monitoramento dos compressores de ar do SENAI-Araçatuba.</strong> </h1>";
html += "</div>";
html += "</header>";
html += "<section class=\"corpo-compressor\">";
html += "<h1 class=\"titulo-corpo-compressor\">Compressor está <br> <p id=\"estado\"></p></h1>";
html += "<input type=\"button\" class=\"botao-compressor\" value=\"LIGA\" onclick=\"estadoon()\">";
html += "<input type=\"button\" class=\"botao-compressor\" value=\"DESLIGA\" onclick=\"estadooff()\">";
html += "<p class=\"texto-corpo-compressor\">";
html += "O compressor está com uma umidade de: <span id=\"umidade\"></span><br>";  // Arredondando para a unidade
html += "O compressor está com uma temperatura de: <span id=\"temperatura\"></span></p>";
html += "<img src=\"" + String(imagemLogo) + "\" class=\"imagem-logo\" alt=\"logo-senai\">";
html += "<p id=\"mensagem-desligado\" style=\"color: white; font-size: 25px; text-align: center;\"></p>";
html += "</section>";
html += "<img src=\"senai logo.png\" class=\"imagem-logo\" alt=\"logo-senai\">";
html += "</section>";
html += "<footer class=\"rodape\">";
html += "<p class=\"texto-rodape\">";
html += "©Produzido pelos alunos de Eletroeletrônica 2023";
html += "</p>";
html += "</footer>";
html += "</body>";
html += "</html>";

   client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println(html);
    delay(200);
}
