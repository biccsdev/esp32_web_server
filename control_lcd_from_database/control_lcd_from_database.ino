#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h> 
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "";
const char* password = "";

const char* serverName = "";

const byte led_gpio = 32;

const long interval = 5000;
unsigned long previousMillis = 0;

String outputsState;
String msg;

LiquidCrystal_I2C lcd(0x27,16,2);

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

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Hello :D");

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
      msg = JSON.stringify(myObject[0]["msg"]);
      Serial.println(msg);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(msg);
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
