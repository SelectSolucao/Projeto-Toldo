//Conexões Display TFT ST7735 em Ordem
//VCC → 3.3V
//GND → GND
//CS → GPIO 15
//RES → GPIO 4 
//DC → GPIO 2
//SCL → GPIO 18 
//SDA → GPIO 23

//GitHub 

//Bibliotecas usadas 
#include <Adafruit_GFX.h>    // Biblioteca Gráfica GFX
#include <Adafruit_ST7735.h> // Biblioteca para o display ST7735
#include <SPI.h>             // Biblioteca SPI para comunicação
#include <WiFi.h> //Biblioteca
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ESP32Servo.h> 
//Fontes usadas no display
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <ArduinoJson.h>
// Definir pinos para conexão SPI
#define TFT_CS     15  // Chip select
#define TFT_RST    4   // Reset
#define TFT_DC     2   // Data/Command

// Inicializa o display ST7735 com os pinos definidos
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Configurações OpenWeatherMap
String URL = "http://api.openweathermap.org/data/2.5/weather?"; //Url da API
String ApiKey = "5472aa23a2878f2603898bab6a887709"; //Chave da Api

//Latitude e Longitude de Cornélio. 
String lat = "-23.1813"; 
String lon = "-50.648";



//Iniciar Servo
Servo myservo;  

//Pinos do Sensor de Chuva e Pino do Servo
#define CHUVAPIN_D 14 //Digital
#define CHUVAPIN_A 27 //A
#define servoPin 25

//Configurações do wifi
const char* ssid = "KITNET-01"; // Nome da sua Rede
const char* password = "kitnet2022"; // Senha da Sua Rede
const char* mqtt_server = "test.mosquitto.org"; //Serve de teste do mqtt



//Tópicos de Assinar e Receber Informações. 
String my_payload; 
String my_topic; 


//Variaveis -- Sensor de chuva
bool chuva_D0 = true; //logica inversa - 1 é seco e 0 é molhado
int chuva_A0 = 0;
int lerchuva; 



//Variáveis - Servo Motor 
bool toldo = false;
int cont;
int i;



// Variáveis de armazenamento do display
int umidade = 0;
int temperatura = 0;
String city = "CP";



//Definir o nome do seu servidor.  
WiFiClient Esp32DHT;
PubSubClient client(Esp32DHT);



//Imagens do Lcd convertidas em Hexas
const unsigned char cloud[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000011,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000011,B00000000,B00000000,B00000000,B00000000,
  B00000000,B11000011,B00000110,B00000000,B00000000,B00000000,
  B00000000,B11000011,B00001110,B00000000,B00000000,B00000000,
  B00000000,B01100011,B00001100,B00000000,B00000000,B00000000,
  B00000000,B01100011,B00011000,B00000000,B00000000,B00000000,
  B00010000,B00111111,B11111000,B00000000,B00000000,B00000000,
  B00111100,B00111111,B11110000,B01110000,B00000000,B00000000,
  B00001110,B01111111,B11111101,B11100000,B00000000,B00000000,
  B00000111,B11111000,B00111111,B10000000,B00000000,B00000000,
  B00000001,B11110000,B00011110,B00000000,B00000000,B00000000,
  B00000001,B11100000,B00001000,B01111111,B10000000,B00000000,
  B00000001,B11000000,B00000001,B11111111,B11100000,B00000000,
  B01111111,B11000000,B00000011,B11111111,B11110000,B00000000,
  B01111111,B11000000,B00000111,B11000000,B11111000,B00000000,
  B00000001,B11000000,B00001111,B00000000,B01111100,B00000000,
  B00000001,B11100001,B11111111,B00000000,B00111100,B00000000,
  B00000001,B11100111,B11111110,B00000000,B00011110,B00000000,
  B00000111,B10001111,B11111100,B00000000,B00011110,B00000000,
  B00001110,B00001111,B00000100,B00000000,B00001110,B00000000,
  B00111000,B00011110,B00000000,B00000000,B00001111,B10000000,
  B00000000,B00011100,B00000000,B00000000,B00001111,B11000000,
  B00000000,B01111100,B00000000,B00000000,B00000111,B11100000,
  B00000011,B11111100,B00000000,B00000000,B00000001,B11100000,
  B00000111,B11111100,B00000000,B00000000,B00000000,B11110000,
  B00001111,B11111100,B00000000,B00000000,B00000000,B01110000,
  B00001111,B00000000,B00000000,B00000000,B00000000,B01110000,
  B00001110,B00000000,B00000000,B00000000,B00000000,B01110000,
  B00011110,B00000000,B00000000,B00000000,B00000000,B01110000,
  B00011110,B00000000,B00000000,B00000000,B00000000,B11110000,
  B00001110,B00000000,B00000000,B00000000,B00000000,B11110000,
  B00001111,B00000000,B00000000,B00000000,B00000011,B11100000,
  B00001111,B11111111,B11111111,B11111111,B11111111,B11000000,
  B00000111,B11111111,B11111111,B11111111,B11111111,B10000000,
  B00000001,B11111111,B11111111,B11111111,B11111111,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};
const unsigned char CLOCK[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00111100,B01110000,B00000000,
  B00000000,B11000000,B00000100,B00000000,
  B00000001,B00000001,B00000011,B00000000,
  B00000010,B01000000,B00001000,B10000000,
  B00000100,B00000000,B00000000,B01000000,
  B00001000,B00000000,B00000000,B00100000,
  B00010000,B00000000,B00000000,B00100000,
  B00010100,B00000001,B00000000,B10010000,
  B00000000,B00000001,B00000000,B00010000,
  B00100000,B00000001,B00000000,B00001000,
  B00100000,B00000001,B00000000,B00001000,
  B00100000,B00000001,B00000000,B00001000,
  B00101000,B00000001,B11100000,B00101000,
  B00101000,B00000000,B00000000,B00101000,
  B00100000,B00000000,B00000000,B00001000,
  B00100000,B00000000,B00000000,B00001000,
  B00100000,B00000000,B00000000,B00000000,
  B00010000,B00000000,B00000000,B00010000,
  B00010000,B00000000,B00000000,B10010000,
  B00001000,B00000000,B00000000,B00100000,
  B00001000,B00000000,B00000000,B01100000,
  B00000100,B00000000,B00000000,B01000000,
  B00000010,B00000000,B00000000,B10000000,
  B00000001,B10000001,B00000011,B00000000,
  B00000000,B01100000,B00001100,B00000000,
  B00000000,B00011110,B11110000,B00000000,
  B00000000,B00000000,B00000000,B00000000
};
const unsigned char humid[] PROGMEM = {
  B00000000,B00100000,B00000000,
  B00000000,B00110000,B00000000,
  B00000000,B01111000,B00000000,
  B00000000,B11111100,B00000000,
  B00000001,B11111110,B00000000,
  B00000011,B11111110,B00000000,
  B00000011,B11111111,B00000000,
  B00000111,B11111111,B10000000,
  B00001111,B11111111,B10000000,
  B00001111,B11111111,B11000000,
  B00011111,B11111111,B11000000,
  B00011111,B11111111,B11100000,
  B00111111,B11111111,B11100000,
  B00111111,B11111111,B11110000,
  B01111111,B11111111,B11110000,
  B01111110,B01110111,B11111000,
  B01111110,B00110111,B11111000,
  B01111100,B10101111,B11111000,
  B01111110,B01101111,B11111000,
  B01111111,B11011011,B11111000,
  B01111111,B11010001,B11111000,
  B01111111,B10110101,B11111000,
  B01111111,B10110001,B11111000,
  B01111111,B01111011,B11111000,
  B00111111,B11111111,B11110000,
  B00111111,B11111111,B11110000,
  B00011111,B11111111,B11100000,
  B00001111,B11111111,B11000000,
  B00000011,B11111111,B00000000,
  B00000000,B01110000,B00000000
};
const unsigned char pressure[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00001111,B11000000,B00000000,
  B00000000,B00111100,B11110000,B00000000,
  B00000000,B11000000,B00011100,B00000000,
  B00000001,B10000000,B00000110,B00000000,
  B00000011,B00000000,B00000010,B00000000,
  B00000010,B00000000,B00000001,B00000000,
  B00000100,B00000000,B00010001,B10000000,
  B00000100,B00000000,B01100000,B10000000,
  B00001100,B00000000,B11000000,B10000000,
  B00001000,B00000011,B10000000,B11000000,
  B00001000,B00000111,B10000000,B11000000,
  B00001000,B00000111,B10000000,B11000000,
  B00001100,B00000011,B00000000,B11000000,
  B00001100,B00000000,B00000000,B10000000,
  B00000100,B00000000,B00000000,B10000000,
  B00000110,B00000000,B00000001,B10000000,
  B00000010,B00000000,B00000001,B00000000,
  B00000011,B00000000,B00000010,B00000000,
  B00000001,B10000000,B00000100,B00000000,
  B00000000,B01100000,B00011000,B00000000,
  B00000000,B00111111,B11110000,B00000000,
  B00000000,B00000111,B10000000,B00000000,
  B00000000,B00000101,B10000000,B00000000,
  B00000000,B00000101,B10000000,B00000000,
  B00111111,B11111101,B11111111,B11110000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00111111,B11111111,B11111111,B11110000
};
const unsigned char windS[] PROGMEM = {
  B00000000,B00000000,B00001111,B00000000,B11110000,
  B00000000,B00000000,B00011101,B11000011,B10011100,
  B00000000,B00000000,B00110000,B11000110,B00000110,
  B00000000,B00000000,B00100000,B01100100,B00000010,
  B00000000,B00000000,B00000000,B01101100,B00000011,
  B00000000,B00000000,B00000000,B01101100,B00000001,
  B00000000,B00000000,B00000000,B11000000,B00000011,
  B00000111,B11111111,B11111111,B10000000,B00000011,
  B00001111,B11111111,B11111111,B00000000,B00000010,
  B00000000,B00000000,B00000000,B00000000,B00000110,
  B11111111,B11111111,B11111111,B11111111,B11111100,
  B11111111,B11111111,B11111111,B11111111,B11110000,
  B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B01111111,B11111111,B11111110,B00000000,
  B00000000,B01111111,B11111111,B11111111,B00000000,
  B00000000,B00000000,B00000000,B00000001,B10000000,
  B00000000,B00000000,B00000000,B00010001,B10000000,
  B00000000,B00000000,B00000000,B00011011,B00000000,
  B00000000,B00000000,B00000000,B00001110,B00000000
};



//Configuração do Wi-fi
void setup_wifi() {

  delay(10);
  //Conectando com wifi
  Serial.println();
  Serial.print("Conectando em: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 



  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}


//Receber informações para ativar o Esp32
void callback(char* topic, byte* payload, unsigned int length) {
  
  String conc_payload;
  for (int i = 0; i < length; i++) {
    conc_payload += ((char)payload[i]);
  }
  my_payload = conc_payload;
  my_topic = topic;
}

//Receber informações. 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Atenção MQTT Conectando...");
    
    if (client.connect("Esp32DHT")) {
      Serial.println("Conectado");

      client.subscribe("ESP32_RECEBE");
    } else {
      Serial.print("Erro,");
      Serial.print(client.state());
      Serial.println(" Será tentado novamente, espere 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  //Iniciando o Serial
  Serial.begin(115200);
  setup_wifi();
  Serial.println(F("Projeto do Toldo!"));

  pinMode(14, INPUT);
  pinMode(2, OUTPUT);  //Led para mostrar se está conectado 
  pinMode(15, OUTPUT);  //Led para mostrar se está conectado 
  

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883); //Definindo o server e a porta dele 
  client.setCallback(callback); //Setando o callback para assinar tópicos 
  myservo.attach(18); //Definição do servo para 180 graus






  tft.initR(INITR_BLACKTAB);   // Iniciando o display com as configurações fixas de bitmap.
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  delay(500);
  tft.fillScreen(ST7735_BLACK);
    tft.drawFastHLine(0, 0, tft.width(), ST7735_WHITE);
    tft.drawFastHLine(1, 159, tft.width(), ST7735_WHITE);
    tft.drawFastVLine(0, 0, tft.height(), ST7735_WHITE);
    tft.drawFastVLine(127, 0, tft.height(), ST7735_WHITE);
    tft.drawBitmap(6, 5, cloud, 45, 38,ST7735_YELLOW);
    tft.drawBitmap(90, 47, CLOCK, 30, 28,ST7735_WHITE);
    tft.drawBitmap(6, 75, humid, 22, 30,0x07FF);
    tft.drawBitmap(95, 100, pressure, 30, 30,ST7735_RED);
    tft.drawBitmap(6, 131, windS, 40, 19,0xAFE5);
     tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(50,20);
    tft.setTextSize(1);
    tft.print(city);
    tft.setFont();
    tft.setTextSize(1);
    tft.setCursor(55,31);
    tft.setTextColor(0x07FF);
    tft.setFont(&FreeSans9pt7b);
    tft.setTextSize(1);
    tft.setCursor(70,100);
    tft.print("%");
    tft.setFont();
    tft.setTextColor(0x07FF);
    tft.setCursor(95,85);
    tft.setTextSize(1);
    tft.print("umid");
     delay(1000);




}

//Ativar Led para verificar conexão com wifi
void ConectadoWifi(){
  if(WiFi.status())
  digitalWrite(2, HIGH);
  else
  digitalWrite(2, LOW);
}

//Ativar Led para verificar conexão com servidor 
void ConectadoServidor(){
  if(client.connected())
  digitalWrite(15, HIGH);
  else
  digitalWrite(15, LOW);
}

void Display(){
      //Função que envia a temperatura e a umidade para o display
      tft.fillRect(14, 45 , 68, 28, ST7735_BLACK);
      tft.fillRect(24, 70 , 55, 10, ST7735_BLACK);
      tft.fillRect(73, 24 , 25, 21, ST7735_BLACK);
      tft.fillRect(40, 80 , 26, 21, ST7735_BLACK);
      tft.fillRect(20, 105 , 55, 18, ST7735_BLACK);
      tft.fillRect(50, 130 , 45, 19, ST7735_BLACK);

      tft.setTextColor(ST7735_YELLOW);  //Cor da Fonte
      tft.setFont(&FreeMono12pt7b); //Tipo da Fonte
      tft.setTextSize(1); //Tamanho da Fonte
      tft.setCursor(40,70); //Localização em X & Y
      tft.print(temperatura); //Temperatura 
      //tft.setFont();

      tft.setTextColor(ST7735_YELLOW);
      tft.setFont(&FreeMono12pt7b);
      tft.setTextSize(1);
      tft.setCursor(70, 70);
      tft.print("C");


      tft.setTextColor(0x07FF);
       tft.setFont(&FreeMono12pt7b);
      tft.setTextSize(1);
      tft.setCursor(40,100);
      tft.print(umidade);


      tft.setFont();
      tft.setTextColor(ST7735_YELLOW);
      tft.setCursor(0,60);
      tft.setTextSize(1);
      tft.print("temp");
   
    
}

void Open(){ 
  // Configurações da api do openweathermap

   HTTPClient http;

    //Informações de Acesso 
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

    //Iniciar a conexão 
    int httpCode = http.GET();

  
    if (httpCode > 0) {

      //Receber o Json com as informações
      String JSON_Data = http.getString(); //Recebe

     
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      //Informações que irão pro display 
      const char* description = obj["weather"][0]["description"].as<const char*>();
      const int temp = obj["main"]["temp"].as<float>();
      const int humidity = obj["main"]["humidity"].as<float>();
      delay(3000);
      temperatura = temp;
      umidade = humidity;
  }
}

void Chuvas(){
  lerchuva = digitalRead(CHUVAPIN_D);
  

  if(lerchuva == 0)
  {
    chuva_D0 = false;
    Serial.println("Está chovendo");
    delay(1000);
  }
  if(lerchuva == 1)
  {
    chuva_D0 = true;
     Serial.println("Não está chovendo");
    delay(1000);
  }
}

void ServoMot(){
  if(chuva_D0 == false & toldo == false)
  {
  for(i = 0; i <= 20; i++)
  {
    Serial.println("Abrindo Toldo");
    Serial.println(cont);
    myservo.write(cont);
    cont++;
    delay(1000);
  }
    toldo = true; 
  }

}

void fechaToldo()
{
 if(chuva_D0 == true & toldo == true)
  {
    for(i == 21; i >= 0; i--)
    {
    Serial.println("Fechando Toldo");
    Serial.println(cont);
    myservo.write(cont);
    cont--;
    delay(1000);
    }
      toldo = false; 
  }

}

void Dashboard(){
  client.publish("WOWKI_TEMP", String(temperatura).c_str());
  client.publish("WOWKI_UMI", String(umidade).c_str());
}
//Comandar o toldo Manualmente com o Servidor. 

void Recebe(){
  int Valor;
  Valor = my_payload.toInt();
  myservo.write(Valor); 
  delay(50);
}




void loop() {
  Display(); //Função do Display(temperatura & umidade)
  reconnect(); //Função de Enviar informações
  ConectadoWifi(); //Função de Confirmação de Conexão Wi-FI
  client.loop();
  ConectadoServidor(); //Função de Confirmação de Conexão Servidor 
  Open(); //Função da API
  Chuvas(); //Função de Verificação de Chuva
  ServoMot();
  fechaToldo();
  Recebe();
  Dashboard();
}