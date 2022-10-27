#include "heltec.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "DJ WiFi";
const char *password = "secrets!";

WiFiServer server(80);

void printToScreen(String s) {
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, s);
  Heltec.display->display();
}

void setup() {
  Heltec.begin(true /*display*/, false /*LoRa*/, true /*Serial*/);
  printToScreen("READY");

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  printToScreen("My IP: " + myIP.toString());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String fromClient = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (fromClient.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println();
            break;
          } else {
            fromClient = "";
          }
        } else if (c != '\r') {
          fromClient += c;
        }

        if (fromClient.endsWith("GET /ledon")) {
          digitalWrite(25, HIGH);
        }
        if (fromClient.endsWith("GET /ledoff")) {
          digitalWrite(25, LOW);
        }
      }
    }
    client.stop();
  }
}
