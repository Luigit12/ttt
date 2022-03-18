#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleRoot() {
  if (server.method() == "GET") {
    server.send(200, "text/html", ""
"<html>"

"<head>"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"</head>"

"<body>"
"  <form name=\"send-command\" method=\"post\">"
"    <label for=\"command\">Vul hier het commando in</label>"
"    <input type=\"text\" id=\"command\" name=\"command\" />"
"    <input type=\"submit\" hidden />"
"  </form>"
"</body>"

"</html>"
);
  }
  else if (server.method() == "POST") {
    Serial.print("Arguments: ");
    Serial.println(server.args());
    server.send(200);
  }
}
