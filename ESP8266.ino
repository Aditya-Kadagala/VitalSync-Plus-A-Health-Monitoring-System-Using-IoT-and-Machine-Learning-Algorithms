#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "ssid";
const char* password = "pass";
String URL = "http://ip address/directory/php_file.php";

long instance1 = 0, timer;
float hrvv = 0, hrr = 72, interval = 0;
int value = 0, count = 0;
bool flag = 0;
#define shutdown_pin 10
#define threshold 100
#define timer_value 10000
float shr = 0;
float spo2 = 0;
float co2 = 0;
float temp = 0;
float hr = 0;
float hrv = 0;
void setup() {
  Serial.begin(9600);
  pinMode(14, INPUT);
  pinMode(12, INPUT);
  WiFi.begin(ssid, password);
}

void loop() {
  if ((digitalRead(8) == 1) || (digitalRead(9) == 1)) {
    Serial.println("leads off!");
    digitalWrite(shutdown_pin, LOW);
    instance1 = micros();
    timer = millis();
  } else {
    digitalWrite(shutdown_pin, HIGH);
    value = analogRead(A0);
    value = map(value, 250, 400, 0, 100);
    if ((value > threshold) && (!flag)) {
      count++;
      Serial.println("in");
      flag = 1;
      interval = micros() - instance1;
      instance1 = micros();
    } else if ((value < threshold)) {
      flag = 0;
    }
    if ((millis() - timer) > 10000) {
      hrr = count * 6;
      timer = millis();
      count = 0;
    }
    hrvv = hrr / 60 - interval / 1000000;
    Serial.print(hrr);
    Serial.print(",");
    Serial.print(hrvv);
    Serial.print(",");
    Serial.println(value);
    delay(10);
  }
  
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    sscanf(data.c_str(), "%f,%f,%f,%f,%f,%f", &shr, &spo2, &co2, &temp, &hr, &hrv);
    Serial.println("------------");
    Serial.println(shr);
    Serial.println(spo2);
    Serial.println(co2);
    Serial.println(temp);
    Serial.println(hr);
    Serial.println(hrv);
    Serial.println("-------------");
  }
  hr = hrr;
  hrv = hrvv;
  
  /*hrr = 100;
  hrvv = 100;*/
  /*temp = 44;
  co2 = 10;
  spo2 = 22;
  shr = 4;*/
  String postData = "temp=" + String(temp) + "&co2=" + String(co2) + "&hr=" + String(hr) + "&hrv=" + String(hrv) + "&spo2=" + String(spo2) + "&shr=" + String(shr);
  Serial.println(postData);
  HTTPClient http;
  WiFiClient client;
  http.begin(client, URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  String payload = http.getString();
  http.end(); 
  delay(1000);
}
