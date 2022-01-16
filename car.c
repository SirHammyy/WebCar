#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <SoftwareSerial.h>

#define TIMEOUT 5000
#define DEBUG_SERIAL Serial;

WebSocketsClient webSocket;
const char *ssid = "StillNachoWifi";
const char *password = "ou812wifi";

unsigned long messageInterval = 5000;
bool connected = false;

int boardled = D0;

int rightpin1 = D5;
int rightpin2 = D6;
int rightspeedpin = D4;
int rightSpeed = 0;

int leftpin1 = D7;
int leftpin2 = D8;
int leftspeedpin = D0;
int leftSpeed = 0;


void straight() {
  analogWrite(rightspeedpin, 225);
  analogWrite(leftspeedpin, 225);
  digitalWrite(rightpin1, HIGH);
  digitalWrite(rightpin2, LOW);
  digitalWrite(leftpin1, HIGH);
  digitalWrite(leftpin2, LOW);
}

void turnLeft() {
  analogWrite(rightspeedpin, 225);
  digitalWrite(rightpin1, HIGH);
  digitalWrite(rightpin2, LOW);
  analogWrite(leftspeedpin, 225);
  digitalWrite(leftpin1, LOW);
  digitalWrite(leftpin2, HIGH);
}

void reverse() {
  analogWrite(rightspeedpin, 225);
  digitalWrite(rightpin1, LOW);
  digitalWrite(rightpin2, HIGH);
  analogWrite(leftspeedpin, 225);
  digitalWrite(leftpin1, LOW);
  digitalWrite(leftpin2, HIGH);
}

void turnRight() {
  analogWrite(rightspeedpin, 225);
  digitalWrite(rightpin1, LOW);
  digitalWrite(rightpin2, HIGH);
  analogWrite(leftspeedpin, 225);
  digitalWrite(leftpin1, HIGH);
  digitalWrite(leftpin2, LOW);
}

void stopCar() {
  analogWrite(rightspeedpin, 0);
  digitalWrite(rightpin1, LOW);
  digitalWrite(rightpin2, LOW);
  analogWrite(leftspeedpin, 0);
  digitalWrite(leftpin1, LOW);
  digitalWrite(leftpin2, LOW);
}


void handleInput(uint8_t * payload) {
  if (strcmp((char*)payload, "Forward") == 0) {
    Serial.printf("[RECV] Moving forward\n");
    straight();
  } else if (strcmp((char*)payload, "Left") == 0) {
    Serial.printf("[RECV] Moving left\n");
    turnLeft();
  } else if (strcmp((char*)payload, "Right") == 0) {
    Serial.printf("[RECV] Moving right\n");
    turnRight();
  } else if (strcmp((char*)payload, "Reverse") == 0) {
    Serial.printf("[RECV] Moving reverse\n");
    reverse();
  } else if (strcmp((char*)payload, "Stop") == 0) {
    Serial.printf("[RECV] Stopping\n");
    stopCar();
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            connected = false;
            break;
        case WStype_CONNECTED: {
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;
 
            // send message to server when Connected
            Serial.println("[WSc] SENT: Connected");
            webSocket.sendTXT("esp8266 Connected");
        }
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] RESPONSE: %s\n", payload);
            handleInput(payload);
            break;
        case WStype_BIN:
            Serial.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;
                case WStype_PING:
                        // pong will be send automatically
                        Serial.printf("[WSc] get ping\n");
                        break;
                case WStype_PONG:
                        // answer to a ping we send
                        Serial.printf("[WSc] get pong\n");
                        break;
    
  }
}

void setup() {
    Serial.begin(115200);

    pinMode(rightpin1, OUTPUT);
    pinMode(rightpin2, OUTPUT);
    pinMode(leftpin1, OUTPUT);
    pinMode(leftpin2, OUTPUT);

    pinMode(rightspeedpin, OUTPUT); 
    pinMode(leftspeedpin, OUTPUT); 
 
//  DEBUG_SERIAL.setDebugOutput(true);
 
    Serial.println();
    Serial.println();
    Serial.println();
 
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
 
    WiFi.begin(ssid, password);
 
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());
    // server address, port and URL
    webSocket.begin("hammywebserver.duckdns.org", 42422, "/");
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();
 
 
void loop() {
    webSocket.loop();
    //if (connected && lastUpdate+messageInterval<millis()){
    //    Serial.println("[WSc] SENT: Simple js client message!!");
    //    webSocket.sendTXT("esp8266 coming in");
    //    lastUpdate = millis();
    //}
}