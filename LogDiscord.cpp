#include "LogDiscord.h"

DiscordLogService::DiscordLogService(){};

void DiscordLogService::sendLog(String mensagem) {
  String webhookURL = "https://discord.com/api/webhooks/1113132271625568368/xOBmpzrNDQKsUUZQMf6AFes0WQWITPioPKK-NjkFGHSOeZc1Wt5qL7-id_v2pR5g2vrO";

  HTTPClient http;

  http.begin(webhookURL);
  http.addHeader("Content-Type", "application/json");
  String postData = "{\"content\":\"" + mensagem + "\"}";
  int httpResponseCode = http.POST(postData);

  // if (httpResponseCode < 400) {
  //     Serial.println("Mensagem enviada com sucesso");
  // } else {
  //     Serial.print("Erro ao enviar a mensagem. Código de resposta: ");
  //     Serial.println(httpResponseCode);
  // }

  http.end();
}

void DiscordLogService::log_vida(){
    time_t now = time(nullptr);
    struct tm *timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    char message[256];
    snprintf(message, sizeof(message), "```CONECTADO: %02d:%02d:%02d.```",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    String mensagem = message;
    
    // Enviar mensagem para o webhook do Discord
    sendLog(message);
}