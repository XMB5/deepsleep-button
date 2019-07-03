#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  bool ledOn = HIGH;
  Serial.printf("connect to wifi ssid %s password %s\n", WIFI_SSID, WIFI_PASSWORD);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, ledOn);
    Serial.print(".");
    ledOn ^= HIGH;
    delay(500);
  }
  digitalWrite(LED_PIN, HIGH);
  Serial.printf("\nconnected with IP address %s\n", WiFi.localIP().toString().c_str());
}

HTTPClient http;
bool requestError;

void onButtonPress() {
  Serial.println("button pressed, sending http request");
  digitalWrite(LED_PIN, LOW);
  http.begin(TRIGGER_URL);
  int resCode = http.GET();
  http.end();
  Serial.printf("http response code %d\n", resCode);
  requestError = resCode != 200;
}

void onButtonRelease() {
  Serial.println("button released");
  if (requestError) {
    bool ledOn = false;
    for (int i = 0; i < 6; i++) {
      delay(250);
      ledOn ^= true;
      digitalWrite(LED_PIN, ledOn);
    }
  }
  digitalWrite(LED_PIN, HIGH);
}

bool previouslyPressed = false;
void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    //button pressed
    if (!previouslyPressed) {
      onButtonPress();
      previouslyPressed = true;
    }
  } else {
    if (previouslyPressed) {
      onButtonRelease();
    }
    previouslyPressed = false;
  }
  delay(25);
}
