#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <string.h>

#define TEMPERATURE_PIN A0
#define LED_STATUS LED_BUILTIN

const char *ssid = "CabudareWireless";
const char *password = "";
int countConnection = 0;

unsigned long last = 0;
String temperature = "";

// Create a WebSocketServer Object
WebSocketsServer webSocket = WebSocketsServer(8080);

// *********************************** FUNCTIONS ***********************************

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        Serial.printf("[%u] Disconnected..!\n", num);
    }
    break;

    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected..!");
    }
    break;

    case WStype_TEXT:
    {
        Serial.printf("[%u] From Client: %s\n", num, payload);
    }
    break;
    }
}

void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    // Initialize serial port
    Serial.begin(9600);

    // Connected to the WIFI
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED and countConnection < 50)
    {
        ++countConnection;
        delay(500);
        Serial.print(".");
    }

    //If connected, then
    if (WiFi.status() == WL_CONNECTED)
    {
        IPAddress Ip(10, 0, 0, 102);
        IPAddress Gateway(10, 0, 0, 1);
        IPAddress Subnet(255, 255, 255, 0);
        WiFi.config(Ip, Gateway, Subnet);

        Serial.print("");
        Serial.print("WiFi Connected with IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.print("Connection Error...!!");
    }

    // Initialize the WebSocket and waits for events
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

}

// the loop function runs over and over again forever
void loop()
{
    // WebSocket loop
    webSocket.loop();

    if (millis() - last >= 1000)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        float millivolts = (analogRead(TEMPERATURE_PIN) / 1024.0) * 3300;

        temperature = String((millivolts / 10.0), 2);

        Serial.print("Tempertaure: ");
        Serial.print(temperature);
        Serial.print("\r");

        webSocket.broadcastTXT(temperature);
        last = millis();
    }
}
