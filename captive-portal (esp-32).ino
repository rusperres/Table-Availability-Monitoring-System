#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#include <WebSocketsServer.h> // Include WebSocket library

DNSServer dnsServer;
AsyncWebServer server(80);
WebSocketsServer webSocket(81); // Create WebSocket server instance on port 81

const int LED_BUILTIN = 2;


bool webRequestReceived = false;

char webpage[] PROGMEM = R"=====(

  <!DOCTYPE html>
  <html lang='en'>

  <head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>CIT-U Table</title>
    <style>
      body {
        background-color: #d4f0fc;
      }

      .header {
        width: 100%;
        height: 100px;
        background-color: #02577a;
        border: 2px solid #01303f;
        position: fixed;
        top: 0;
        left: 0;
      }

      .logo {
        width: 50%;
        height: 80px;
        border-radius: 10px;
        position: absolute;
        background-color: #01303f;
        left: 25%;
        top: 15px;
      }

      h1 {
        text-align: center;
        color: #89d6fb;
        font-size: xx-large;
        font-family: 'Courier New', Courier, monospace;
      }

      .container {
        width: 400px;
        height: 400px;
        position: relative;
        left: 0%;
        margin: auto;
        top: 150px;
      }

      div {
        width: 350px;
        height: 200px;
        background-color: #02a9f7;
        border: 2px solid #053B50;
        position: absolute;
        top: 100px;
        left: 6%;
      }

      .chair {
        background-color: #02577a;
        border: 2px solid #053B50;
        width: 80px;
        height: 60px;
      }

      .back {
        background-color: #02a9f7;
        border: 2px solid #053B50;
        width: 80px;
        height: 15px;
      }

      .top {
        border-left: 0px;
        border-top: 0px;
      }

      .bottom {
        position: absolute;
        border-bottom: 0px;
        border-left: 0px;
        bottom: 0px;
      }

      .chair-top-left {
        position: absolute;
        left: 75px;
        top: 40px;
      }

      .chair-top-right {
        position: absolute;
        right: 75px;
        top: 40px;
      }

      .chair-bottom-left {
        position: absolute;
        left: 75px;
        bottom: 40px;
      }

      .chair-bottom-right {
        position: absolute;
        right: 75px;
        bottom: 40px;
      }

      @media screen and (max-width:1080px) {
        h1 {
          margin-top: 30px;
          font-size: x-large;
        }
      }
    </style>
    <script>
      var websocket;

      function connectWebSocket() {
        websocket = new WebSocket('ws://' + window.location.hostname + ':81/');
        websocket.onopen = function (evt) {
          console.log('WebSocket connected');
        };
        websocket.onmessage = function (evt) {
          console.log('Message received: ' + evt.data);
          toggleColorDiv(evt.data);
        };
      }

      function toggleColorDiv(command) {
        var colorDiv = document.getElementById('colorDiv');
        if (command === 'toggleRed') {
          colorDiv.style.backgroundColor = '#ff6464';
        } else {
          colorDiv.style.backgroundColor = '#64ff64';
        }
      }

      window.onload = function () {
        connectWebSocket();
      };
    </script>
  </head>

  <body>
    <section class="header">
      <section class="logo">
        <h1> Seats2Sit</h1>
      </section>
    </section>
    <section class="container">
      <article class="chair chair-top-left">
        <article class="back top"></article>
      </article>
      <article class="chair chair-top-right">
        <article class="back top"></article>
      </article>
      <article class="chair chair-bottom-left">
        <article class="back bottom"></article>
      </article>
      <article class="chair chair-bottom-right">
        <article class="back bottom"></article>
      </article>
      <div id='colorDiv'></div>

    </section>
  </body>

  </html>)=====";

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    webRequestReceived = true;
    request->send_P(200, "text/html", webpage);
  }
};

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_CONNECTED:
            break;
        case WStype_DISCONNECTED:
            Serial.printf("[%u] WebSocket client disconnected\n", num);
            break;
        case WStype_TEXT:
          if (strcmp((char *)payload, "toggleRed") == 0) {
              delay(1000);
              webSocket.sendTXT(num, "toggleRed");
          } else if (strcmp((char *)payload, "toggleGreen") == 0) {
              delay(1000);
              webSocket.sendTXT(num, "toggleGreen");
          }
          break;
        default:
            break;
    }
}

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.softAP("Seats2Sit");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.begin();

  webSocket.begin(); // Start WebSocket server
  webSocket.onEvent(onWebSocketEvent); // Define WebSocket event handler
}

void loop() {
  if (webRequestReceived) {
    if (Serial.available() > 0) {
        String receivedString = "";

        while (Serial.available() > 0) {
            receivedString += char(Serial.read());
        }

        for (uint8_t i = 0; i < webSocket.connectedClients(); i++) {
            if (receivedString == "1") {
                webSocket.sendTXT(i, "toggleRed", strlen("toggleRed"));
            } else if (receivedString == "2") {
                webSocket.sendTXT(i, "toggleGreen", strlen("toggleGreen"));
            }
        }
    }
  }
  dnsServer.processNextRequest();
  webSocket.loop();
}



