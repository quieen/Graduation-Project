#define BLYNK_TEMPLATE_ID "TMPL6a4UKzNb8"
#define BLYNK_TEMPLATE_NAME "DHT11Monitor"
#define BLYNK_AUTH_TOKEN "sWSCCfrRxGcpLU6zeW48BRtHbbmnAnem"

#include <WiFi.h>
#include <ESP32Servo360.h>
#include <ESP32Servo.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>


char auth[] = "sWSCCfrRxGcpLU6zeW48BRtHbbmnAnem"; // MÃ TOKEN
char ssid[] = "quyen"; // Your WiFi credentials.
char pass[] = "10107210"; // Set password to "" for open networks.

//lcd 21,22
//vantay 16,17
//servo_cua 27
//doam,t 15
//servo_mua 13
//quat 25 26
//led 35
//mua 14
//servo cua 32
#define Fan01 V2
int fan1 = 25;

#define Fan02 V3
int fan2 = 26;

#define LED V6
int led = 23;

Servo myservo1;
int servo1 = 13;

Servo myservo2;
int servo2 = 32;
int prev;
#define mySerial Serial2

bool mssg = false;
String name[128];


#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht11_sensor(DHTPIN,DHTTYPE);

Servo myservo;
int rain = 14;

float send_Temp;
int send_Humd;
String send_Status_Read_DHT11 = "";

String postData = "";
String payload = "";

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);
WidgetTerminal terminal(V5);



void get_DHT11_sensor_data(){
 
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
  delay(500);
  
}


void setup(){

  Serial.begin(115200);
  prev = 1;
  pinMode(rain,INPUT);
  pinMode(fan1,OUTPUT);
  pinMode(fan2,OUTPUT);
  pinMode(led,OUTPUT);
  Blynk.begin(auth, ssid, pass);



  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.println();
  Serial.println("-------------");
  Serial.print("Connecting");

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(800);
      ESP.restart();
    }
  }
  

  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.println("-------------");


  dht11_sensor.begin();
  delay(500);

  myservo1.attach(servo1);
  myservo2.attach(servo2);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  finger.begin(57600);

}

void loop(){
  HTTPClient http;
  int httpCode;    
    
  get_DHT11_sensor_data();
  
  
  postData = "id=esp32_01";
  postData += "&temperature=" + String(send_Temp);
  postData += "&humidity=" + String(send_Humd);
  postData += "&status_read_sensor_dht11=" + send_Status_Read_DHT11;
    
  payload = "";
  
  Serial.println();
  Serial.println("---------------updateDHT11data_and_recordtable.php");
  
  http.begin("http://172.20.10.5/FINAL02/updateDHT11data_and_recordtable.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   
  httpCode = http.POST(postData); 
  payload = http.getString();  
  
  Serial.print("httpCode : ");
  Serial.println(httpCode);
  Serial.print("payload  : ");
  Serial.println(payload);  
    
  http.end();  
  

//BLYNKRUN
  Blynk.run();

  //check nhiet do, do am
  int digital = digitalRead(rain);
  if(digital == 1){
    Serial.print("Digital: ");Serial.println(digital);

    myservo1.write(90);
    
  }if(digital == 0){
    Serial.print("Digital: ");Serial.println(digital);
    
    myservo1.write(0);
    
  }
  Blynk.virtualWrite(V0,send_Temp); // chân đặt, tên
  Blynk.virtualWrite(V1,send_Humd);  

  //Kiểm tra tình trạng hoạt động của vân tay và xác thực mở cửa vân tay. 
  if (finger.verifyPassword()) 
  {
    lcd.setCursor(0,0);
    lcd.print("Fingerprint?????");
  } 
  else 
  {
    lcd.setCursor(0,0);
    lcd.print("Sensor Broken");
  }
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK)
  {
    p = finger.image2Tz();
    lcd.setCursor(0,0);
    lcd.print("Wait for it...");
    if (p == FINGERPRINT_OK)
    {
      p = finger.fingerFastSearch();
      if (p == FINGERPRINT_OK)
      {
        myservo2.write(90);
        lcd.setCursor(0, 1);
        lcd.print("Unlocked");
        lcd.setCursor(10, 1);
        lcd.print(name[finger.fingerID]);
        delay(3000);
        myservo2.write(0);
        lcd.setCursor(0, 1);
        lcd.print("Locked");
        delay(1000);
        lcd.clear();

        postData = "id=esp32_01";
        postData += "&finger=" + String(name[finger.fingerID]);
      
        payload = "";
  
        Serial.println();
        Serial.println("---------------updateFinger.php");
  
        http.begin("http://172.20.10.5/FINAL02/updateFINGER.php");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   
        httpCode = http.POST(postData); 
        payload = http.getString();  
  
        Serial.print("httpCode : ");
        Serial.println(httpCode);
        Serial.print("payload  : ");
        Serial.println(payload);  
    
        http.end();  
      }
      else 
      {
        lcd.setCursor(0, 1);
        lcd.print("Wrong finger");
        delay(1500);
        lcd.clear();
      }     
    } 
  }  
}







//Blynk_WIRTE
BLYNK_WRITE(Fan01){
  int p = param.asInt();
  digitalWrite(fan1,p);
}

BLYNK_WRITE(Fan02){
  int p = param.asInt();
  digitalWrite(fan2,p);
}

BLYNK_WRITE(LED){
  int p = param.asInt();
  digitalWrite(led,p);
}

BLYNK_WRITE(V5){
  int id =0;
  if (mssg == false && (String("dang ky") == param.asStr())){
    lcd.clear();
    mssg = true;
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("ten : ");
    
  }else if (mssg == true && (String("huy bo") != param.asStr())){
    mssg = false;
    lcd.setCursor(8,1);
    lcd.print(param.asStr());
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("dat van tay");
    int p = -1;
    int id = (1,127);
    while (finger.loadModel(id) == FINGERPRINT_OK)
    {
      id = random(1,127);
    }
    while (p != FINGERPRINT_OK){
      p = finger.getImage();
      if (p == FINGERPRINT_OK){
        p = finger.image2Tz(1);
        if (p == FINGERPRINT_OK){
          p = finger.fingerFastSearch();
          if (p == FINGERPRINT_OK){
            lcd.clear();
            lcd.print("Van tay da co");
            delay(2000);
            break;
          } else
          lcd.clear();
          lcd.print("bo tay ra");
          p = finger.getImage();
          while (p == FINGERPRINT_OK){
            p = finger.getImage();
          }
          lcd.clear();
          lcd.print("xac thuc");
          p = finger.getImage();
          while (p != FINGERPRINT_OK){
            p = finger.getImage();
          }
          if (p == FINGERPRINT_OK){
            p = finger.image2Tz(2);
            if (p == FINGERPRINT_OK){
              p = finger.createModel();
              if (p == FINGERPRINT_OK) {
                p = finger.storeModel(id);
                if (p == FINGERPRINT_OK){
                  lcd.clear();
                  lcd.println("thanh cong");
                  delay(500);
                  lcd.clear();
                  lcd.print(id);
                  lcd.clear();
                  name[id] = param.asStr();
                  lcd.clear();
                  lcd.print(name[id]);
                  delay(2000);
                
                } else {
                  lcd.clear();
                  lcd.print("that bai");
                  delay(2000);
                  break; 
                } 
              } else {
                lcd.clear();
                lcd.print("Khong hop le");
                delay(2000);
                break;
              }
            }else break;
          } else break;
        } else break;
      }
      
    }
  }
    
  else if (String("xoa van tay") == param.asStr()) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("dat van tay");
    int p = -1;
    while (p != FINGERPRINT_OK){
      p = finger.getImage();
      if (p == FINGERPRINT_OK){
        p = finger.image2Tz();
        if (p == FINGERPRINT_OK){
          p = finger.fingerFastSearch();
          if (p == FINGERPRINT_OK){
            p = finger.deleteModel(finger.fingerID);
            if (p == FINGERPRINT_OK){
              lcd.clear();
              lcd.print("Xoa thanh cong");
              name[finger.fingerID] = "";
              delay(2000);
              
            }
          } else{
            lcd.clear();
            lcd.print("Khong ton tai");
            delay(2000);
            break;
          }
        } else break;
      }
      
    }
  } else if (String("kiem tra") == param.asStr()){
    int p = -1;
    p = finger.getTemplateCount();
    if (p == FINGERPRINT_OK){
      lcd.clear();
      lcd.print("Co ");
      lcd.setCursor(3,0);
      lcd.print(finger.templateCount);
      Serial.print(finger.templateCount);
      lcd.setCursor(6,0);
      lcd.print("van tay");
      delay(2000);
      lcd.clear();
    }
   }
   else if (String("xoa het") == param.asStr()){
    lcd.clear();
    int p = finger.emptyDatabase();
    if (p == FINGERPRINT_OK){
      lcd.print("hoan tat ");
      String name[128];
      delay(2000);
    }
   }
}