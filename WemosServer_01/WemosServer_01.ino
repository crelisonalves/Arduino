#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

// Rótulo e variáveis globais

#define home "/index.html"
#define logo "/logo.ico"
#define json "/json.txt"


const char* ssid = "BBC_SIBELLE";
const char* password = "13579864";
bool    spiffsActive = false; 
ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {   // página inicial
  digitalWrite(led, 1);
  if (spiffsActive) {
    if (SPIFFS.exists(home)) {
      File f = SPIFFS.open(home, "r");
      if (!f) {
              
        server.send(200, "text/plain", "hello from esp8266!");
      } else {
        server.send(200, "text/html", f.readString());
        f.close();
      }
     }
  }
     digitalWrite(led, 0);
}


void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){

  Serial.begin(9600);
  delay(1000);
  
  // Start filing subsystem
  if (SPIFFS.begin()) {
      Serial.println("Sistema de arquivo SPIFFS ativado");
      Serial.println();
      spiffsActive = true;
  } else {
      Serial.println("Falha na leitura de sistema de arquivo SPIFFS");
  }
  delay(20);

  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/logo.ico",[](){
      if (spiffsActive) {
    if (SPIFFS.exists(logo)) {
      File f = SPIFFS.open(logo, "r");
      if (!f) {
              
        server.send(200, "image/x-icon", " ");
      } else {
        server.send(200, "image/x-icon", f.readString());
        f.close();
      }
     }
  }
  });
  server.on("/json.txt", [](){
   
    
      if (spiffsActive) {
    if (SPIFFS.exists(json)) {
      File f = SPIFFS.open(json, "r");
      if (!f) {
              
        server.send(200, "image/x-icon", " ");
      } else {
        server.send(200, "application/json", f.readString());
        f.close();
      }
     }
  }
 
  
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
