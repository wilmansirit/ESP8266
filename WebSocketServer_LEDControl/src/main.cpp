#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED_RED   16   // D0
#define LED_GREEN 12   // D6
#define LED_BLUE  13   //


const char *ssid = "CabudareWireless";
const char *password = "";
int countConnection = 0;

String indexPage = "<html>"
                "<head>"
                "<script>"
                "var connection = new WebSocket('ws://'+location.hostname+':81/', ['esp8266']);"
                "connection.onopen = function ()       { connection.send('From Client: Connected ' + new Date()); };"
                "connection.onerror = function (error) { console.log('WebSocket Error ', error);};"
                "connection.onmessage = function (e)   { console.log('Server: ', e.data);};"
                "function sendRGB() {"
                " var r = parseInt(document.getElementById('r').value).toString(16);"
                " var g = parseInt(document.getElementById('g').value).toString(16);"
                " var b = parseInt(document.getElementById('b').value).toString(16);"
                " if(r.length < 2) { r = '0' + r; }"
                " if(g.length < 2) { g = '0' + g; }"
                " if(b.length < 2) { b = '0' + b; }"
                " var rgb = '#'+r+g+b;"
                " connection.send(rgb);"
                "}"
                "</script>"
                "</head>"
                "<body>"
                "LED Control:<br/><br/>"
                "R: <input id='r' type='range' min='0' max='255' step='1' value='0' oninput='sendRGB();'/><br/>"
                "G: <input id='g' type='range' min='0' max='255' step='1' value='0' oninput='sendRGB();'/><br/>"
                "B: <input id='b' type='range' min='0' max='255' step='1' value='0'oninput='sendRGB();'/><br/>"
                "</body>"
                "</html>";



ESP8266WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81, "", "esp8266");

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] From Server: Disconnected!\n", num);
            break;

        case WStype_CONNECTED:
        {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] From Server: Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "From Server: Connected");
        }
            break;

        case WStype_TEXT:
            Serial.printf("[%u] Get Text: %s\n", num, payload);

            // send data to all connected clients (BROADCASTING)
            // webSocket.broadcastTXT(payload, length);

            if (payload[0] == '#')
            {
                // we get RGB data

                // decode rgb data
                uint32_t rgb = (uint32_t)strtol((const char *)&payload[1], NULL, 16);

                analogWrite(LED_RED, (rgb >> 16) & 0xFF);
                analogWrite(LED_GREEN, (rgb >> 8) & 0xFF);
                analogWrite(LED_BLUE, (rgb >> 0) & 0xFF);
            }
            break;
    }
}

void setup()
{

    Serial.begin(9600);
    Serial.println();

    WiFi.mode(WIFI_STA); //para que no inicie el SoftAP en el modo normal
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED and countConnection < 50) { 
        //Cuenta hasta 50 si no se puede conectar lo cancela
        ++countConnection;
        delay(500);
        Serial.print(".");
    }

    if (countConnection < 50) {

        //para usar con ip fija
        IPAddress Ip(10, 0, 0, 102);
        IPAddress Gateway(10, 0, 0, 1);
        IPAddress Subnet(255, 255, 255, 0);
        WiFi.config(Ip, Gateway, Subnet);

        Serial.println("");
        Serial.println("WiFi conectado");
        Serial.println(WiFi.localIP());

    } else {
        Serial.println("");
        Serial.println("Error de conexion");
    }

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);




    if (MDNS.begin("esp8266"))
    {
        Serial.println("MDNS responder started");
    }


    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

    digitalWrite(LED_RED, 0); 
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 0);

    analogWriteRange(0);

    // Start webSocket and webServer
    webSocket.begin();
    webServer.begin();

    // Handle webServer and webSocket event
    webServer.on("/", []() {
        webServer.send(200, "text/html", indexPage);
    });

    webSocket.onEvent(webSocketEvent);

}

void loop()
{
    webSocket.loop();
    webServer.handleClient();
}