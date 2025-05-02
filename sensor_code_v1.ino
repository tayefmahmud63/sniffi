#include <DHT.h>
#include <Wire.h>
#include "Adafruit_CCS811.h"

// DHT22
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// CCS811
Adafruit_CCS811 ccs;
bool ccs_ok = false;

// Analog sensor pins
#define MQ4_PIN    A0  // CH₄ (also used for debugging)
#define MQ136_PIN  A1  // H₂S
#define MQ131L_PIN A2  // O₃ Low
#define MQ2_PIN    A3  // Propane
#define MQ6_PIN    A4  // Butane
#define MQ8_PIN    A5  // H₂
#define MQ131H_PIN A6  // O₃ High
#define ZP07_PIN   A7  // VOCs

// Placeholder values for unimplemented I2C/UART sensors
float co = 0, nh3 = 0, so2 = 0, ch2o = 0, no2 = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Try CCS811 init
  if (ccs.begin()) {
    ccs_ok = true;
  }

  // Print CSV header
  Serial.println("temp,humd,co,nh3,h2s,ch2o,o3_low,so2,propane,butane,h2,voc,no2,eco2,tvoc,o3_high");
}

void loop() {
  // Read DHT22
  float temp = dht.readTemperature();
  float humd = dht.readHumidity();
  if (isnan(temp)) temp = 0;
  if (isnan(humd)) humd = 0;

  // Read analog sensors
  int ch4      = analogRead(MQ4_PIN);     // CH₄ (not printed directly)
  int h2s      = analogRead(MQ136_PIN);
  int o3_low   = analogRead(MQ131L_PIN);
  int propane  = analogRead(MQ2_PIN);
  int butane   = analogRead(MQ6_PIN);
  int h2       = analogRead(MQ8_PIN);
  int voc      = analogRead(ZP07_PIN);
  int o3_high  = analogRead(MQ131H_PIN);

  // Read CCS811
  float eco2 = 0, tvoc = 0;
  if (ccs_ok && ccs.available() && !ccs.readData()) {
    eco2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();
  }

  // Print CSV header every time
  //Serial.println("temp,humd,co,nh3,h2s,ch2o,o3_low,so2,propane,butane,h2,voc,no2,eco2,tvoc,o3_high");

  // Print CSV data row
  Serial.print(temp); Serial.print(",");
  Serial.print(humd); Serial.print(",");
  Serial.print(co); Serial.print(",");
  Serial.print(nh3); Serial.print(",");
  Serial.print(h2s); Serial.print(",");
  Serial.print(ch2o); Serial.print(",");
  Serial.print(o3_low); Serial.print(",");
  Serial.print(so2); Serial.print(",");
  Serial.print(propane); Serial.print(",");
  Serial.print(butane); Serial.print(",");
  Serial.print(h2); Serial.print(",");
  Serial.print(voc); Serial.print(",");
  Serial.print(no2); Serial.print(",");
  Serial.print(eco2); Serial.print(",");
  Serial.print(tvoc); Serial.print(",");
  Serial.print(o3_high); Serial.println();

  delay(1000); // Delay for 1 second
}
