// LCD library and configuration
#include <LiquidCrystal.h>
#include <SD.h>
#include <DHT.h>
#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include "RTClib.h"
LiquidCrystal lcd(12,11,5,4,3,2);

// Initializing Termometer constants
const int sensorPin = A0;

// Init File
File myFile;
RTC_DS1307 RTC;
void setup() {
  // put your setup code here, to run once:

  lcd.begin(16,2);
  lcd.print("Temperature:");
  lcd.setCursor(0,1);
  lcd.print("250");
  Serial.begin(9600);
  dht.begin();/*
  if (!SD.begin(6)) {
    Serial.println("No se pudo inicializar");
    return;
  }*/
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
  Serial.print("Sensor: ");
  //Serial.print(sensorVal);
  Serial.print(", Voltage: ");
  //Serial.print(voltage);
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
/*
  myFile = SD.open("datalog.txt", FILE_WRITE);//abrimos  el archivo
  if (myFile) { 
        Serial.print("Escribiendo SD: ");
        int sensor1 = analogRead(0);
        int sensor2 = analogRead(1);
        int sensor3 = analogRead(2);
        myFile.print("Datetime=");
        myFile.print(RTC.now().unixtime());
        myFile.print(", Tiempo(ms)=");
        myFile.print(millis());
        myFile.print(", Temp=");
        myFile.println(temperature);
        
        myFile.close(); //cerramos el archivo  
  } else {
    Serial.println("Error al abrir el archivo");
  }*/
  // Down the frequency to proper readings
  delay(1000);
}
