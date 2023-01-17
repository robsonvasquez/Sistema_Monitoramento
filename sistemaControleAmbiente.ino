#include<ESP8266WiFi.h> //Biblioteca para funcionamento do WiFi
#include<ESP8266WebServer.h> //Biblioteca para funcionar como servidor
#include <DHT.h> //Biblioteca para funcionamento do sensor de temperatura e umidade
#include <IRremoteESP8266.h> //Biblioteca para enviar os sinais de infravermelho

const char* ssid = "nome da sua rede";
const char* password = "senha da sua rede";

ESP8266WebServer server(80); //server na porta 80

#define DHTPIN 4 //Pino digital D2 (GPIO5) conectado ao DHT11
#define DHTTYPE DHT22 //Tipo do sensor DHT11

#define DHT1PIN 5 //Pino digital D2 (GPIO5) conectado ao DHT11
#define DHT1TYPE DHT22 //Tipo do sensor DHT11

#define DHT2PIN 2 //Pino digital D2 (GPIO5) conectado ao DHT11
#define DHT2TYPE DHT22 //Tipo do sensor DHT11

DHT dht_ambiente(DHTPIN, DHTTYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)
float temp_ambiente; //variável para armazenar a temperatura
float umi_ambiente; //Variável para armazenar a umidade

DHT dht_server1(DHT1PIN, DHT1TYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)
float temp_server1; //variável para armazenar a temperaturaDHT

DHT dht_server2(DHT2PIN, DHT2TYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)
float temp_server2; //variável para armazenar a temperatura

IRsend irsend(14); //FUNÇÃO RESPONSÁVEL PELO MÉTODO DE ENVIO DO SINAL IR / UTILIZA O GPIO14(D5)
int frequencia = 32; //FREQUÊNCIA DO SINAL IR(32KHz)
int ar_1 = 20;
int ar_2 = 21;
int ar_3 = 22;
bool est_1 = true;
bool est_2 = true;
bool est_3 = true;

// BOTÃO LIGA / DESLIGA
unsigned int LD_ar_1[] = {3050,1700,600,1150,550,1200,500,400,550,450,500,400,550,1150,550,
350,550,400,550,1200,500,1200,500,400,550,1250,550,350,550,400,550,1200,550,1250,500,400,
550,1200,500,1200,500,450,550,400,500,1200,500,400,550,400,550,1200,550,400,550,350,550,
450,500,400,550,400,550,400,550,450,550,400,550,350,550,400,500,450,550,400,550,400,550,
350,550,400,550,350,550,350,550,1200,550,450,550,350,550,1200,550,350,550,450,550};
int tam_LD_ar_1 = 100;

unsigned int LD_ar_2[] = {};
int tam_LD_ar_2 = 0;

unsigned int LD_ar_3[] = {};
int tam_LD_ar_3 = 0;

// Botão aumenta temperatura
unsigned int TA_ar_1[] = {};
int tam_TA_ar_1 = 0;

unsigned int TA_ar_2[] = {};
int tam_TA_ar_2 = 0;

unsigned int TA_ar_3[] = {};
int tam_TA_ar_3 = 0;

// Botão diminui temperatura
unsigned int TD_ar_1[] = {};
int tam_TD_ar_1 = 0;

unsigned int TD_ar_2[] = {};
int tam_TD_ar_2 = 0;

unsigned int TD_ar_3[] = {};
int tam_TD_ar_3 = 0;


void setup() {
  Serial.begin(115200); //Inicializa a comunicação serial
  delay(50); // Intervalo para aguardar a estabilização do sistema
  dht_ambiente.begin(); //Inicializa o sensor ambiente
  dht_server1.begin();  //Inicializa o sensor server 1
  dht_server2.begin(); //Inicializa o sensor server 2

  irsend.begin();

  Serial.println("Conectando a Rede: "); //Imprime a mensagem
  Serial.println(ssid); //Imprime o nome da Rede Wi-Fi

  WiFi.begin(ssid, password); //Inicialização da comunicação Wi-Fi

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { //Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print("."); //Imprime pontos
  }
  Serial.println("");
  Serial.println("WiFi Conectado"); //Imprime a mensagem
  Serial.print("IP: "); //Imprime a mensagem
  Serial.println(WiFi.localIP()); //Função para exibir o IP que a placa pego na rede

  server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  server.begin(); //Inicializa o servidor
  Serial.println("Servidor HTTP inicializado"); //Imprime a mensagem

}

void loop() {
  server.handleClient(); //Chama o método handleClient()
}

void handle_OnConnect() {
  temp_ambiente = dht_ambiente.readTemperature();  //Realiza a leitura da temperatura do ambiente
  umi_ambiente = dht_ambiente.readHumidity(); //Realiza a leitura da umidade do ambiente
  temp_server1 = dht_server1.readTemperature();  //Realiza a leitura da temperatura do server 1
  temp_server2 = dht_server2.readTemperature();  //Realiza a leitura da temperatura do server 2
  
  Serial.print("Temperatura: ");
  Serial.print(temp_ambiente); //Imprime no monitor serial o valor da temperatura do ambiente
  Serial.println(" ºC");
  Serial.print("Umidade: ");
  Serial.print(umi_ambiente); //Imprime no monitor serial o valor da umidade do ambiente
  Serial.println(" %");

  envia_sinal(); // Chama a função de enviar sinal
  server.send(200, "text/html; charset=utf-8", cria_HTML()); //Chama a função para cria a página web

}

void handle_NotFound() { //Função para lidar com o erro 404
  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"

}

void envia_sinal(){

    if(temp_server1 < 45 && est_1 == true){
      irsend.sendRaw(LD_ar_1, tam_LD_ar_1, frequencia);
      est_1 = false;
    }

    if(temp_server2 < 45 && est_2 == true){
      irsend.sendRaw(LD_ar_2, tam_LD_ar_2, frequencia);
      est_2 = false;
    }

    if(temp_server2 < 45 && est_3 == true){
      irsend.sendRaw(LD_ar_3, tam_LD_ar_3, frequencia);
      est_3 = false;
    }
     
    if(temp_ambiente > 50){
      if(ar_1 >= 20){
         irsend.sendRaw(TD_ar_1, tam_TD_ar_1, frequencia);
         ar_1 += 1;
      }
      if(ar_2 >= 20){
         irsend.sendRaw(TD_ar_2, tam_TD_ar_2, frequencia);
         ar_2 += 1;
      }
      if(ar_3 >= 20){
         irsend.sendRaw(TD_ar_3, tam_TD_ar_3, frequencia);
         ar_1 += 1;
      }
    }
}

String cria_HTML() { //Cria página web
  String ptr = "<!DOCTYPE html> <html>\n"; //Indica o envio do código HTML
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; //Torna a página da Web responsiva em qualquer navegador Web
  ptr += "<meta http-equiv='refresh' content='2'>";//Atualizar browser a cada 2 segundos
  ptr += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css' integrity='sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh' crossorigin='anonymous'>"; // Framework
  ptr += "<title>Monitor de Temperatura do Data Center</title>\n"; //Define o título da página
  ptr += "</head>\n";

  //Configurações de fonte do título e do corpo do texto da página web;
  ptr += "<body class='bg-light'>\n";
  ptr += "<div class='container text-center'>\n";
  ptr += "<h1>Monitor de Temperatura do Data Center</h1>\n";
  ptr += "<h3>NODEMCU ESP8266 Web Server</h3>\n";
  ptr += "</div>\n";

  //Exibe as informações de temperatura e umidade na página web
  ptr += "<div class='container border border-dark text-center bg-success'>\n";
  ptr += "<h5>Temperatura e Umidade do Ambiente</h5>\n";
  if(temp_ambiente < 50){
    ptr += "<p><b>Temperatura: </b>";
    ptr += (float)temp_ambiente;
    ptr += " ºC</p>";
  }else{
    ptr += "<p class='text-danger'><b>Temperatura: </b>";
    ptr += (float)temp_ambiente;
    ptr += " ºC</p>";
  }
  ptr += "<p><b>Umidade: </b>";
  ptr += (float)umi_ambiente;
  ptr += " %</p>";
  ptr += "</div>\n";

  ptr += "<div class='container border border-dark text-center bg-warning'><h5>Temperatura das Máquinas</h5>\n";
  if(temp_server1 < 85){
    ptr += "<p><b>Temperatura Server 1: </b>";
    ptr += (float)temp_server1;
    ptr += " ºC</p>";
  }else{
    ptr += "<p class='text-danger'><b>Temperatura Server 1 ALTA: </b>";
    ptr += (float)temp_server1;
    ptr += " ºC</p>";
  }

  if(temp_server2 < 85){
    ptr += "<p><b>Temperatura Server 2: </b>";
    ptr += (float)temp_server2;
    ptr += " ºC</p>";
  }else{
    ptr += "<p class='text-danger'><b>Temperatura Server 2 ALTA: </b>";
    ptr += (float)temp_server2;
    ptr += " ºC</p>";
  }
  ptr += "</div>";

  ptr += "<div class='container border border-dark text-center bg-info'><h5>Temperatura Ar-Condicionado</h5>\n";
  if(est_1 == true){
    ptr += "<p><b>Temperatura Ar condiciona 1: </b>";
    ptr += (int)ar_1;
    ptr += " ºC</p>";
  }else{
     ptr += "<p><b> Ar condiciona 1 DESLIGADO </b>";
  }
  if(est_2 == true){
    ptr += "<p><b>Temperatura Ar condiciona 2: </b>";
    ptr += (int)ar_2;
    ptr += " ºC</p>";
  }else{
     ptr += "<p><b> Ar condiciona 2 DESLIGADO </b>";
  }
    if(est_3 == true){
    ptr += "<p><b>Temperatura Ar condiciona 3: </b>";
    ptr += (int)ar_3;
    ptr += " ºC</p>";
  }else{
     ptr += "<p><b> Ar condiciona 3 DESLIGADO </b>";
  }
  ptr += "</div>";  
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
