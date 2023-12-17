
#include <WiFi.h>
const char *ssid = "quyen";
const char *password = "10107210";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to WIFI...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WIFI connected.");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}
