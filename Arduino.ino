#include <MAX3010x.h>
#include "filters.h"
#include <OneWire.h>
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS); 
int sensorPin=A2;
int sensorData;
float duptemp = 0;
MAX30105 sensor;
const auto kSamplingRate = sensor.SAMPLING_RATE_400SPS;
const float kSamplingFrequency = 400.0;
const unsigned long kFingerThreshold = 10000;
const unsigned int kFingerCooldownMs = 500;
const float kEdgeThreshold = -2000.0;
const float kLowPassCutoff = 5.0;
const float kHighPassCutoff = 0.5;

const bool kEnableAveraging = false;
const int kAveragingSamples = 5;
const int kSampleThreshold = 5;
long instance1=0, timer;
double hrv =0, hr = 72, interval = 0;
int value = 0, count = 0;  
bool flag = 0;
#define shutdown_pin 10 
#define threshold 100 
#define timer_value 10000 

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);  
  if(sensor.begin() && sensor.setSamplingRate(kSamplingRate)) { 
    //Serial.println("Sensor initialized");
  }
  else {
    Serial.println("Sensor not found");  
    while(1);
  }
  pinMode(8, INPUT); 

  pinMode(9, INPUT); 

}

LowPassFilter low_pass_filter_red(kLowPassCutoff, kSamplingFrequency);
LowPassFilter low_pass_filter_ir(kLowPassCutoff, kSamplingFrequency);
HighPassFilter high_pass_filter(kHighPassCutoff, kSamplingFrequency);
Differentiator differentiator(kSamplingFrequency);
MovingAverageFilter<kAveragingSamples> averager_bpm;
MovingAverageFilter<kAveragingSamples> averager_r;
MovingAverageFilter<kAveragingSamples> averager_spo2;

MinMaxAvgStatistic stat_red;
MinMaxAvgStatistic stat_ir;

float kSpO2_A = 1.5958422;
float kSpO2_B = -34.6596622;
float kSpO2_C = 112.6898759;
long last_heartbeat = 0;
long finger_timestamp = 0;
bool finger_detected = false;

float last_diff = NAN;
bool crossed = false;
long crossed_time = 0;

void loop() {
  //Serial.println('1');
  sensorData = analogRead(sensorPin); 
  auto sample = sensor.readSample(1000);
  float current_value_red = sample.red;
  float current_value_ir = sample.ir;
  if(sample.red > kFingerThreshold) {
    if(millis() - finger_timestamp > kFingerCooldownMs) {
      finger_detected = true;
    }
  }
  else {
    //Serial.println('2');
    differentiator.reset();
    averager_bpm.reset();
    averager_r.reset();
    averager_spo2.reset();
    low_pass_filter_red.reset();
    low_pass_filter_ir.reset();
    high_pass_filter.reset();
    stat_red.reset();
    stat_ir.reset();
    finger_detected = false;
    finger_timestamp = millis();
  }

  if(finger_detected) {
    //Serial.println('3');
    current_value_red = low_pass_filter_red.process(current_value_red);
    current_value_ir = low_pass_filter_ir.process(current_value_ir);

    stat_red.process(current_value_red);
    stat_ir.process(current_value_ir);

    float current_value = high_pass_filter.process(current_value_red);
    float current_diff = differentiator.process(current_value);

    if(!isnan(current_diff) && !isnan(last_diff)) {
      
      if(last_diff > 0 && current_diff < 0) {
        crossed = true;
        crossed_time = millis();
      }
      
      if(current_diff > 0) {
        crossed = false;
      }
  
      if(crossed && current_diff < kEdgeThreshold) {
        if(last_heartbeat != 0 && crossed_time - last_heartbeat > 300) {
          int bpm = 60000/(crossed_time - last_heartbeat);
          float rred = (stat_red.maximum()-stat_red.minimum())/stat_red.average();
          float rir = (stat_ir.maximum()-stat_ir.minimum())/stat_ir.average();
          float r = rred/rir;
          float spo2 = kSpO2_A * r * r + kSpO2_B * r + kSpO2_C;
          
          if(bpm > 50 && bpm < 250) {
            if(kEnableAveraging) {
              int average_bpm = averager_bpm.process(bpm);
              int average_r = averager_r.process(r);
              int average_spo2 = averager_spo2.process(spo2);
  
              if(averager_bpm.count() >= kSampleThreshold) {
               //Serial.println('5');
             /* Serial.print("Time (ms): ");
              Serial.println(millis()); */
              //Serial.print("Heart Rate (current, bpm): ");
              Serial.print(bpm);  
              //Serial.print("R-Value (current): ");
              //Serial.println(r);
              //Serial.print("SpO2 (current, %): ");
              Serial.print(",");
              Serial.print(spo2);   
               // Serial.print("Air Quality:");
              Serial.print(",");
              Serial.print(sensorData, DEC);  
              Serial.print(",");             
  //Serial.println(" PPM");
              float temperatureC = getTemperature(); // Get temperature in Celsius
  /*if (temperatureC != -127) {
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");
  } */
  if (temperatureC != -127) {
    duptemp = temperatureC;
  }
  //Serial.print("Temperature: ");
  Serial.print(duptemp);
  Serial.print(",");
  //Serial.println("°C");
  getHRHRV();
              delay(1000);
              }
            }
            else {
              //Serial.println('5');
             /* Serial.print("Time (ms): ");
              Serial.println(millis()); */
              //Serial.print("Heart Rate (current, bpm): ");
              Serial.print(bpm);  
              //Serial.print("R-Value (current): ");
              //Serial.println(r);
              //Serial.print("SpO2 (current, %): ");
              Serial.print(",");
              Serial.print(spo2);   
               // Serial.print("Air Quality:");
    Serial.print(",");
  Serial.print(sensorData, DEC);  
  Serial.print(",");             
  //Serial.println(" PPM");
  float temperatureC = getTemperature(); // Get temperature in Celsius
  /*if (temperatureC != -127) {
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");
  } */
  if (temperatureC != -127) {
    duptemp = temperatureC;
  }
  //Serial.print("Temperature: ");
  Serial.print(duptemp);
  Serial.print(",");
  //Serial.println("°C");
  getHRHRV();
              delay(1000);
            }
          }
          stat_red.reset();
          stat_ir.reset();
        }
        crossed = false;
        last_heartbeat = crossed_time;
      }
    }
    last_diff = current_diff;
  }
  //delay(2000);
}
float getTemperature() {
  byte data[9];
  byte addr[8];

  if (!oneWire.search(addr)) {
    oneWire.reset_search();
    return -127;
  }
  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44, 1);

  delay(100); 

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); 

  for (int i = 0; i < 9; i++) {
    data[i] = oneWire.read();
  }

  int16_t rawTemperature = (data[1] << 8) | data[0];
  float temperatureC = (float)rawTemperature / 16.0;

  return temperatureC;
}

void getHRHRV() {
  if((digitalRead(8) == 1)||(digitalRead(9) == 1)){

    //Serial.println("leads off!");
    Serial.print(0);

    Serial.print(",");

    Serial.print(0);
    Serial.println();
    digitalWrite(shutdown_pin, LOW); 

    instance1 = micros();

    timer = millis();

  }

  else {

    digitalWrite(shutdown_pin, HIGH); 

    value = analogRead(A0);

    value = map(value, 250, 400, 0, 100); 

    if((value > threshold) && (!flag)) {
      count++;  
      flag = 1;
      interval = micros() - instance1;
      instance1 = micros(); 
    }
    else if((value < threshold)) {
      flag = 0;
    }

    if ((millis() - timer) > 10000) {
      hr = count*6;
      timer = millis();
      count = 0; 
    }

    hrv = hr/60 - interval/1000000;
    Serial.print(hr);
    Serial.print(",");
    Serial.print(hrv);
/*
    Serial.print(",");

    Serial.println(value);
*/
    Serial.println();
    delay(1);
  }
}