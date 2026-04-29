#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = " ";
const char* password = " ";

WebServer server(80);

void handleRoot() {
  String json = "{\n";
  json += "  \"status\": \"online\",\n";
  json += "  \"device\": \"Qein X1A Pro S3\",\n";
  json += "  \"message\": \"System is operational!\"\n";
  json += "}";

  server.send(200, "application/json", json);
}

void handleSystemInfo() {
  uint32_t flash_size = ESP.getFlashChipSize() / (1024 * 1024);
  uint32_t psram_size = ESP.getPsramSize() / (1024 * 1024);
  uint32_t uptime = millis() / 1000;

  String json = "{\n";
  json += "  \"chip\": \"ESP32-S3\",\n";
  json += "  \"flash_mb\": " + String(flash_size) + ",\n";
  json += "  \"psram_mb\": " + String(psram_size) + ",\n";
  json += "  \"uptime_seconds\": " + String(uptime) + "\n";
  json += "}";

  server.send(200, "application/json", json);
}

void handleNotFound() {
  String json = "{ \"error\": \"404\", \"message\": \"Endpoint not found\" }";
  server.send(404, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(2000); 

  Serial.println("starting..");

  Serial.printf("Connecting to Wi-Fi: %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi Connected!");
  Serial.print("API is hosted at: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/system", HTTP_GET, handleSystemInfo);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP Server started.");
  Serial.println("=========================================\n");
}

void loop() {
  server.handleClient();
}
