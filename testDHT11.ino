#include <DHT.h>

int DHTPIN = 2;
int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);

float temp;
float humd;

void get_DHT11(){
  temp = dht.readTemperature();
  humd = dht.readHumidity();

  Serial.printf("Temperature : %.f °C\n", temp);
  Serial.printf("Humidity : %d %%\n", humd);
  Serial.println("-------------");
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(2000);
}

void loop() {
  delay(2000);
  get_DHT11();

}
