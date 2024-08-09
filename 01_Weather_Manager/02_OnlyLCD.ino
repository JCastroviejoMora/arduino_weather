// LCD library and configuration
#include <LiquidCrystal.h>
#include <SD.h>
#include <DHT.h>
#include "RTClib.h"

#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(12,11,5,4,3,2);

// Initializing Termometer constants
const int sensorPin = A0;

// Init File
File myFile;

void setup() {
  // put your setup code here, to run once:

  lcd.begin(16,2);
  lcd.print("Temperature:");
  lcd.setCursor(0,1);
  lcd.print("250");
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Read Temperature
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0)*5.0;
  float temp2 = (voltage - .5) * 100;
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float hic = dht.computeHeatIndex(temperature,humidity,false);
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(", Heat Index: ");
  Serial.print(hic);
  Serial.print(", Temperature:");
  Serial.println(temperature);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print(",H:");
  lcd.print(humidity);
  lcd.setCursor(0,1);
  lcd.print("T2:");
  lcd.print(temp2);
  lcd.print(",hic:");
  lcd.print(hic);

  // Down the frequency to proper readings
  delay(2000);
}
