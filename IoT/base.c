#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <NewPing.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Configurações da rede WiFi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Configurações do broker MQTT
const char* mqtt_server = "ENDERECO_DO_BROKER";
const int mqtt_port = 1883;
const char* mqtt_user = "SEU_USUARIO";
const char* mqtt_password = "SUA_SENHA";

// Configurações do sensor DHT11
#define DHTPIN 4       // Pino onde o DHT11 está conectado
#define DHTTYPE DHT11  // Tipo do sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Configurações do sensor ultrassônico
#define TRIGGER_PIN 5  // Pino TRIGGER do sensor ultrassônico
#define ECHO_PIN 18    // Pino ECHO do sensor ultrassônico
#define MAX_DISTANCE 500 // Distância máxima que o sensor deve medir (em centímetros)
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Configurações do módulo GPS
TinyGPSPlus gps;
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

// Cliente WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Função de conexão WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

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

// Função de callback para mensagens MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Função para reconectar ao broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("conectado");
      client.subscribe("seu/topico");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// Função para coletar dados dos sensores
String coletar_dados_sensores() {
  // Coletar dados do DHT11
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Coletar dados do sensor ultrassônico
  unsigned int distancia = sonar.ping_cm();

  // Coletar dados do GPS
  float latitude = 0.0;
  float longitude = 0.0;

  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }
  }

  // Formatar os dados em JSON
  String dados = "{\"temperatura\": ";
  dados += String(temperatura);
  dados += ", \"umidade\": ";
  dados += String(umidade);
  dados += ", \"distancia\": ";
  dados += String(distancia);
  dados += ", \"latitude\": ";
  dados += String(latitude, 6);
  dados += ", \"longitude\": ";
  dados += String(longitude, 6);
  dados += "}";

  return dados;
}

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Coletar e publicar os dados
  String dados = coletar_dados_sensores();
  Serial.print("Publicando mensagem: ");
  Serial.println(dados);
  client.publish("estacao/meteorologica", dados.c_str());

  delay(10000); // Aguarde 10 segundos antes de enviar a próxima mensagem
}
