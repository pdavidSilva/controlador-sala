#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
// Update these with values suitable for your network.
const char* ssid = "Apartamento 201-2.4";
const char* password = "Ald50983021";
const char* mqtt_server = "tvvv";

static const char *root_ca PROGMEM = null // certicate here ;

// A single, global CertStore which can be used by all connections.
// Needs to stay live the entire time any of the WiFiClientBearSSLs
// are present.
WiFiClientSecure espClient;
PubSubClient * client;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
    Serial.print(" - length ");
    Serial.println(length);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we’re reconnected
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP8266Client";
    // Attempt to connect
    // Insert your password
    if (client->connect(clientId.c_str(), "user", "1234")) {
      Serial.println("connected");
      // Once connected, publish an announcement…
      //client->publish("esp/6203f522", "hello world");
      // … and resubscribe
      client->subscribe("esp/6203f522");
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  delay(500);
  // When opening the Serial Monitor, select 9600 Baud
  Serial.begin(115200);
  delay(500);
  setup_wifi();
  espClient.setCACert(root_ca);
  client = new PubSubClient(espClient);

  client->setServer(mqtt_server, 8884);
  client->setCallback(callback);
}

void loop() {
  if (!client->connected()) {
    reconnect();
  }
  client->loop();
}