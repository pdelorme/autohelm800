#include <Arduino.h>
#include <WiFi.h>
// #include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoOTA.h>

class AHServer {
  private:
    AsyncWebServer server = AsyncWebServer(80);
    AsyncWebSocket ws = AsyncWebSocket("/ws");

    // Replace with your network credentials
    const char* SP_ssid = "AUTO_PILOT";
    const char* SP_password = "1234567890";
    /**
     * initialize wifi Access Point.
     */
    void setupWifiAP(){
      Serial.println("SettingUp AccessPoint");
      WiFi.softAP(SP_ssid, SP_password);
      IPAddress IP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(IP);
    }

    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
              
    }
    void setupWebServer(){
      Serial.println("Setting up WebServer");
      // Launch SPIFFS file system | Démarre le système de fichier SPIFFS 
      if(!SPIFFS.begin()){ 
        Serial.println("An Error has occurred while mounting SPIFFS");  
      }
    
      // static file server
      server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    
      // Simple INFO
      server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "HELLO OCEAN");
      });


      // // init ws
      // ws.onEvent(onEvent);
      // server.addHandler(&ws);

      Serial.println("starting WebServer");
      // OTA server
      ArduinoOTA.begin();
      // Start server
      server.begin();
    }
  public:
    AHServer(){
      setupWifiAP();
      setupWebServer();
    }
  
  void loop(){
     ArduinoOTA.handle();
  }

};
