#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "IOTUCO";
const char* password = "LifeIsIoT";

ESP8266WebServer server(80);

int ledPin = D4;
int ledPin2 = D5;
bool ledState = LOW;
bool ledState2 = LOW;

void handleRoot() {
  String html = "<html><body><h1>Control de LED</h1>";
  html += "<label class=\"switch\">";
  html += "<input type=\"checkbox\" onclick=\"sendRequest(this.checked,'1')\">";
  if(ledState) {
    html += "<div class=\"slider round checked\"></div>";
  } else {
    html += "<div class=\"slider round\"></div>";
  }
  html += "</label>";
  html += "<h1>Control de LED 2</h1>";
  html += "<label class=\"switch\">";
  html += "<input type=\"checkbox\" onclick=\"sendRequest(this.checked,'2')\">";
  if(ledState2) {
    html += "<div class=\"slider round checked\"></div>";
  } else {
    html += "<div class=\"slider round\"></div>";
  }
  html += "</label>";
  html += "<script>";
  html += "function sendRequest(checked,led) {";
  html += "var xhttp = new XMLHttpRequest();";
  html += "xhttp.open('POST', '/', true);";
  html += "xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');";
  html += "xhttp.send('command=' + (checked ? 'on' : 'off') + led);";
  html += "}";
  html += "</script>";
  html += "<style>";
  html += ".switch {";
  html += "position: relative;";
  html += "display: inline-block;";
  html += "width: 60px;";
  html += "height: 34px;";
  html += "}";
  html += ".switch input {";
  html += "display: none;";
  html += "}";
  html += ".slider {";
  html += "position: absolute;";
  html += "cursor: pointer;";
  html += "top: 0;";
  html += "left: 0;";
  html += "right: 0;";
  html += "bottom: 0;";
  html += "background-color: #ccc;";
  html += "border-radius: 34px;";
  html += "transition: .4s;";
  html += "}";
  html += ".slider:before {";
  html += "position: absolute;";
  html += "content: \"\";";
  html += "height: 26px;";
  html += "width: 26px;";
  html += "left: 4px;";
  html += "bottom: 4px;";
  html += "background-color: white;";
  html += "border-radius: 50%;";
  html += "transition: .4s;";
  html += "}";
  html += "input:checked + .slider {";
  html += "background-color: #2196F3;";
  html += "}";
  html += "input:checked + .slider:before {";
  html += "transform: translateX(26px);";
  html += "}";
  html += ".slider.round {";
  html += "border-radius: 34px;";
  html += "}";
  html += ".slider.round:before {";
  html += "border-radius: 50%;";
  html += "}";
  html += ".slider.round.checked:before {";
  html += "transform: translateX(26px);";
  html += "}";
  html += "</style>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handlePost() {
  if(server.hasArg("command")) {
    String command = server.arg("command");
    Serial.println(command);
    if(command == "on1") {
      ledState = HIGH;
    } else if(command == "off1") {
      ledState = LOW;
    } else if(command == "on2") {
      ledState2 = HIGH;
    } else if(command == "off2") {
      ledState2 = LOW;
    }
    digitalWrite(ledPin, ledState);
    digitalWrite(ledPin2, ledState2);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin, ledState);
  digitalWrite(ledPin2, ledState2);

  Serial.begin(9200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión a WiFi establecida");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handlePost);

  server.begin();
  Serial.println("Servidor iniciado");

  Serial.print("Dirección IP del servidor: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}
