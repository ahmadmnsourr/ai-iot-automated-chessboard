#include "mWifi.h"
#include <WebServer.h>
#include <MQTT.h>
#include "Pages.h"
#include "Config.h"

String EspMove = "";
String ReceivedMove = "";

WebServer server(80);
WiFiClient net;
MQTTClient client;

// MQTT settings         
const int mqtt_port = 1883;

const char* esp_topic = "ESP-TOPIC";
const char* rpi_topic = "RPI-TOPIC";

void connectToMQTT() {
  Serial.print("Connecting to MQTT broker...");
  while (!client.connect("ESP", mqtt_username, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT connected!");

  if (!client.subscribe(rpi_topic, 1)) {
    Serial.println("Failed to subscribe to RPI-TOPIC");
  } else {
    Serial.println("Subscribed to RPI-TOPIC");
  }
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Message received on topic: " + topic);

  if (topic == rpi_topic) {
    ReceivedMove = payload;
    Serial.println("Received from RPI-TOPIC: " + ReceivedMove);
  }
}

void publishMove(const String &move) {
  if (client.connected()) {
    if (!client.publish(esp_topic, move, true, 1)) {
      Serial.println("Publish failed");
    } else {
      Serial.println("Move published to ESP-TOPIC: " + move);
    }
  } else {
    Serial.println("Publish failed - not connected to MQTT");
    connectToMQTT();
  }
}

String handleEspMove() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    return "";
  }
  String EspMove = server.arg("move");
  if (EspMove.length() > 0) {
    publishMove(EspMove);
    Serial.println("Move received from web: " + EspMove);
  }
  return EspMove;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ConnectToWifi(ssid, pass);

  server.on("/", []() {
    server.send(200, "text/html", indexPage);
  });

  server.on("/game", []() {
    server.send(200, "text/html", gamePage);
  });

  server.on("/move", HTTP_POST, handleEspMove);

  server.on("/getmove", HTTP_GET, []() {
    if (ReceivedMove != "") {
      server.send(200, "text/plain", ReceivedMove);
      ReceivedMove = "";
    } else {
      server.send(204, "text/plain", "");
    }
  });

  server.begin();

  client.begin(mqtt_broker, mqtt_port, net);
  client.setOptions(60, true, 1000);
  client.onMessage(messageReceived);

  connectToMQTT();
}

void loop() {
  server.handleClient();
  client.loop();

  if (!client.connected()) {
    Serial.println("MQTT disconnected, attempting to reconnect...");
    connectToMQTT();
  }

  if (Serial.available()) {
    String move = Serial.readStringUntil('\n');
    move.trim();
    if (move.length() > 0) {
      if (move.startsWith("pub:")) {
        String payload = move.substring(4);
        publishMove(payload);
      } else if (move.startsWith("sub:")) {
        String topic = move.substring(4);
        client.subscribe(topic);
        Serial.println("Subscribed to: " + topic);
      } else {
        client.publish(rpi_topic, move);
      }
    }
  }
  delay(10);
}
