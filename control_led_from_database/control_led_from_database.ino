#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h> 
#include <Arduino_JSON.h>

const char* ssid = "";
const char* password = "";

const char* serverName = "";

const byte led_gpio = 32;

const long interval = 5000;
unsigned long previousMillis = 0;

String outputsState;
String led_state;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(led_gpio, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
      
  if (millis() - previousMillis > interval) {
    if (WiFi.status() == WL_CONNECTED ) {
      outputsState = httpGETRequest(serverName);
      //Serial.println(outputsState);
      JSONVar myObject = JSON.parse(outputsState);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      Serial.println(myObject);
      led_state = JSON.stringify(myObject[0]["led_state"]);
      Serial.println(led_state);
      if (led_state == "0") {
        digitalWrite(led_gpio, LOW);
      }
      if (led_state == "1") {
        digitalWrite(led_gpio, HIGH);
      }
    }
  }
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  //http.addHeader("Content-Type", "application/json");
  //http.addHeader("X-CMC_PRO_API_KEY",api_key);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
