#include <DHT.h>

#define DHTPIN 15 
#define DHTTYPE DHT11 
DHT dht11_sensor(DHTPIN, DHTTYPE); 



#define ON_Board_LED 2 
#define LED_01 13 
#define LED_02 12 

float send_Temp;
int send_Humd;
String send_Status_Read_DHT11 = "";


//  Subroutine to read and get data from the DHT11 sensor.
void get_DHT11_sensor_data() {
  Serial.println();
  Serial.println("-------------get_DHT11_sensor_data()");

  send_Temp = dht11_sensor.readTemperature();
  send_Humd = dht11_sensor.readHumidity();

  if (isnan(send_Temp) || isnan(send_Humd)) {
    Serial.println("Failed to read from DHT sensor!");
    send_Temp = 0.00;
    send_Humd = 0;
    send_Status_Read_DHT11 = "FAILED";
  } else {
    send_Status_Read_DHT11 = "SUCCEED";
  }
  
  Serial.printf("Temperature : %.2f °C\n", send_Temp);
  Serial.printf("Humidity : %d %%\n", send_Humd);
  Serial.printf("Status Read DHT11 Sensor : %s\n", send_Status_Read_DHT11);
  Serial.println("-------------");
}


void setup() {

  
  Serial.begin(115200); 

  dht11_sensor.begin();
  delay(2000);
}



void loop() {

  get_DHT11_sensor_data();  
  delay(2000);
}
