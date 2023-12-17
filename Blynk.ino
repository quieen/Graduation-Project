#define BLYNK_TEMPLATE_ID "TMPL6a4UKzNb8"
#define BLYNK_TEMPLATE_NAME "DHT11Monitor"
#define BLYNK_AUTH_TOKEN "sWSCCfrRxGcpLU6zeW48BRtHbbmnAnem"
// #define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6a4UKzNb8"
#define BLYNK_TEMPLATE_NAME "DHT11Monitor"
#define BLYNK_AUTH_TOKEN "sWSCCfrRxGcpLU6zeW48BRtHbbmnAnem"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>


// information
char auth[] = "sWSCCfrRxGcpLU6zeW48BRtHbbmnAnem"; // MÃ TOKEN
char ssid[] = "quyen"; // Your WiFi credentials.
char pass[] = "10107210"; // Set password to "" for open networks.

#define RELAY1 V2
int out_led = 26;


void setup() {
  Serial.begin(115200);
  pinMode(out_led,OUTPUT);
  Blynk.begin(auth, ssid, pass); // mã token, name wifi, password
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
     delay(100);
     Serial.print(".");
   }
  Serial.println("Connected!");

}



void loop() {
  Blynk.run(); 
}
BLYNK_WRITE(RELAY1){
  int p = param.asInt();
  digitalWrite(out_led,p);
}