#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <SoftwareSerial.h>
SoftwareSerial WiFiSerial(0, 1);
void setup()
{
  Serial.begin(115200);
  WiFiSerial.begin(115200);
  Serial.println();

 WiFi.begin("имя", "пароль");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
