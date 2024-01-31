#include "Config.h"
#include <Update.h>
#include <string>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include "LogDiscord.h"


BLEServerService* bleConfig;

HardwareRecord hardware;
Controller controller;
WiFiService wiFiService;
DiscordLogService logDiscord;

const char* MQTT_SERVER = "192.168.1.110";
const char* MQTT_TOPIC = "/update/controlador/";


using namespace std;

WiFiClient client;
PubSubClient mqtt(client);

int contentLength = 0;
bool isValidContentType = false;

void LogConectado() {
  time_t now = time(nullptr);
  struct tm* timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  char message[256];
  snprintf(message, sizeof(message), "```Conectado MQTT: %02d:%02d:%02d```",
           timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

  // Enviar mensagem para o webhook do Discord
  logDiscord.sendLog(message);
}

String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}

String getBinName(String url) {
  int index = 0;

  // Search for last /
  for (int i = 0; i < url.length(); i++) {
    if (url[i] == '/') {
      index = i;
    }
  }

  String binName = "";

  // Create binName
  for (int i = index; i < url.length(); i++) {
    binName += url[i];
  }

  return binName;
}

String getPort(String url) {
  int index = 0;

  // Search for last /
  for (int i = 0; i < url.length(); i++) {
    if (url[i] == ':') {
      index = i;
    }
  }

  String binName = "";

  if(index < 2) return binName;

  // Create binName
  for (int i = index+1; i < url.length(); i++) {
    if(url[i] == '/') break;
    binName += url[i];
  }

  return binName;
}

String getHostName(String url) {
  int index = 0;

  // Search for last /
  for (int i = 0; i < url.length(); i++) {
    if (url[i] == '/') {
      index = i;
    }
  }

  String hostName = "";

  // Create binName
  for (int i = 0; i < index; i++) {
    hostName += url[i];
  }

  return hostName;
}

void update(String url) {
  String bin = getBinName(url);
  String host = getHostName(url);

  String resPort = getPort(url);
  int port = 80;
  if(resPort != ""){
    port = stringToInt(resPort);
  }


  Serial.println("Bin: "+bin+" | Host: "+host+" | PORT: "+port+" | URL: "+url);

  String message = "```Connecting to: " + host + "```";
  logDiscord.sendLog(message);
  Serial.println("Connecting to: " + host);
  if (client.connect(host.c_str(), port)) {
    // Connection Succeed.
    // Fecthing the bin
    Serial.println("Fetching Bin: " + bin);
    String message = "Fetching Bin: " + bin;
    logDiscord.sendLog(message);
    // Get the contents of the bin file
    client.print(String("GET ") + bin + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Cache-Control: no-cache\r\n" + "Connection: close\r\n\r\n");

    unsigned long timeout = millis();

    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Client Timeout !");
        client.stop();
        return;
      }
    }
    while (client.available()) {
      // read line till /n
      String line = client.readStringUntil('\n');
      // remove space, to check if the line is end of headers
      line.trim();

      // if the the line is empty,
      // this is end of headers
      // break the while and feed the
      // remaining `client` to the
      // Update.writeStream();
      if (!line.length()) {
        //headers ended
        break;  // and get the OTA started
      }

      // Check if the HTTP Response is 200
      // else break and Exit Update
      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
          Serial.println("Got a non 200 status code from server. Exiting OTA Update.");
          break;
        }
      }

      // extract headers here
      // Start with content length
      if (line.startsWith("Content-Length: ")) {
        contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
        Serial.println("Got " + String(contentLength) + " bytes from server");
      }

      // Next, the content type
      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");
        Serial.println("Got " + contentType + " payload.");
        if (contentType == "application/octet-stream") {
          isValidContentType = true;
        }
      }
    }
  } else {
    // Connect to S3 failed
    // May be try?
    // Probably a choppy network?
    String message = "Connection to " + host + " failed. Please check your setup";
    logDiscord.sendLog(message);
    Serial.println("Connection to " + host + " failed. Please check your setup");
    // retry??
  }

  // Check what is the contentLength and if content type is `application/octet-stream`
  Serial.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

  // check contentLength and content type
  if (contentLength && isValidContentType) {
    // Check if there is enough to OTA Update
    bool canBegin = Update.begin(contentLength);
    if (canBegin) {
      String message = "Comecando atualizacao";
      logDiscord.sendLog(message);
      Serial.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
      size_t written = Update.writeStream(client);

      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");
        // retry??
      }

      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          String message = "Atualizado com sucesso, reiniciando";
          logDiscord.sendLog(message);
          Serial.println("Update successfully completed. Rebooting.");
          ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      // not enough space to begin OTA
      // Understand the partitions and
      // space availability
      Serial.println("Not enough space to begin OTA");
      client.flush();
    }
  } else {
    String message = "Erro ao tentar baixar arquivo de atualizacao";
    logDiscord.sendLog(message);
    Serial.println("There was no content in the response");
    client.flush();
  }
}

int stringToInt(String str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Verificar sinal negativo
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    // Converter dígitos em número inteiro
    for (; i < str.length(); ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            // Caractere inválido encontrado na string
            throw std::invalid_argument("A string contém caracteres inválidos.");
        }
    }

    return result * sign;
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String _message = String((char*)payload);
  String _topic = String(topic);

  if (_topic.equals(MQTT_TOPIC) == 1) {
    Serial.println(_message);
    update(_message);
  }
}

bool conectado_mqtt = false;


void reconnect() {
  Serial.println(mqtt.connected() == false);
  int countReconnect = 0;
  while (!mqtt.connected()) {
    Serial.println(countReconnect);
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect("Controlador")) {
      Serial.println("connected");
      mqtt.subscribe(MQTT_TOPIC);
      LogConectado();
    }
  //    else if (countReconnect > 20) {
  //     String message = "Desconectou do MQTT, Reiniciando";
  //     logDiscord.sendLog(message);
  //     // Serial.print("failed, rc=");
  //     // Serial.print(mqtt.state());
  //     // Serial.println(" try again in 5 seconds");
  //     // Wait 5 seconds before retrying
  //     delay(5000);
  //     esp_restart();
  //     // countReconnect = 0;
  //     break;
  //   }
  //   countReconnect++;
  }
}


//  MQTT TESTE ---------------------------------------------------------
const char* host = "esp32";
int contador_ms = 0;
WebServer server(80);
const char* ota_html =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>"
  "<div id='prg'>Progresso: 0%</div>"
  "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('Progresso: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('Sucesso!')"
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>";


//  MQTT TESTE ---------------------------------------------------------



void setup() {

  Serial.begin(115200);
  bool init = false;

  delay(100);

  wiFiService.connect();

  logDiscord.log_vida();

  /* Define tratamentos do update de firmware OTA */
  server.on(
    "/update", HTTP_POST,
    []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = server.upload();

      if (upload.status == UPLOAD_FILE_START) {
        /* Inicio do upload de firmware OTA */
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
          Update.printError(Serial);
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* Escrevendo firmware enviado na flash do ESP32 */
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          Update.printError(Serial);
      } else if (upload.status == UPLOAD_FILE_END) {
        /* Final de upload */
        if (Update.end(true))
          Serial.printf("Sucesso no update de firmware: %u\nReiniciando ESP32...\n", upload.totalSize);
        else
          Update.printError(Serial);
      }
    });


  server.on("/ota", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", ota_html);
  });


  server.begin();

  mqtt.setServer(MQTT_SERVER, 1883);
  mqtt.setCallback(callback);

  //do {
  //	if ( controller.start(hardware) ) {
  //		if ( controller.registerHardware(hardware) ) {
  //
  //      controller.setHardwareConfig(hardware);
  //      controller.fillHardwares(hardware);
  //
  //			if ( controller.loadedDevices() )
  //				init = true;
  //
  //		}
  //	}
  //} while( !init );

  // Configure BLE Service
  //controller.setupBLEServer();
  //controller.startBLETaskServer();

  // Configure Http Service
  //controller.startTaskHttp();

  // Configure Environment Variables Service
  //controller.setupEnvironmentVariables();
}

void loop() {
  server.handleClient();
  contador_ms++;

  if(contador_ms > 10){
    Serial.println("Programa antes da atualizacao OTA");
    contador_ms = 0;
  }

  if (contador_ms > 600) {
    logDiscord.log_vida();
    //Serial.println("Programa antes da atualizacao OTA");
    contador_ms = 0;
  }

  //Serial.print(contador_ms);

  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();
  // Serial.println("SAIU");
  delay(1000);
  //
  //delay(1);
  //controller.environmentVariablesContinuousValidation();
}