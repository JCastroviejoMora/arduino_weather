// LCD library and configuration
#include <SD.h>
#include <DHT.h>
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Time.h>
#include <TimeLib.h>
#include "RTClib.h"
time_t fecha;
// Initializing Termometer constants
const int sensorPin = A0;
int warning = 8;
int ok = 6;
int bad = 9;
// Init File
File myFile;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ok, OUTPUT); // Ok
  pinMode(warning, OUTPUT); // Warning
  pinMode(bad, OUTPUT); // Bad
  
  setTime(19, 10, 00, 05, 8, 2024);
  fecha = now();
  dht.begin();
  if (!SD.begin(10)) {
    Serial.println("No se pudo inicializar");
    digitalWrite(warning, HIGH);
    digitalWrite(ok, LOW);
    digitalWrite(bad, LOW);
    return;
  }
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
  Serial.print(sensorVal);
  Serial.print(", Voltage: ");
  Serial.print(voltage);
  Serial.print(", Temperature:");
  Serial.println(temperature);

  Serial.print("Hora: ");
  Serial.print(hour(fecha));
  Serial.print(":");
  Serial.print(minute(fecha));
  Serial.print(":");
  Serial.print(second(fecha));
 
  Serial.print("  ");
  Serial.print(day(fecha));
  Serial.print("/");
  Serial.print(month(fecha));
  Serial.print("/");
  Serial.println(year(fecha));

  myFile = SD.open("datalog.txt", FILE_WRITE);//abrimos  el archivo
  if (myFile) { 
        Serial.print("Escribiendo SD: ");
        
        myFile.print(year(fecha));
        myFile.print("-");
        myFile.print(month(fecha));
        myFile.print("-");
        myFile.print(day(fecha));
        myFile.print("  ");
        myFile.print(hour(fecha));
        myFile.print(":");
        myFile.print(minute(fecha));
        myFile.print(":");
        myFile.print(second(fecha));
      
        myFile.print("");
        myFile.print(",");
        myFile.print(millis());
        myFile.print(",");
        myFile.print(temperature);
        myFile.print(",");
        myFile.print(humidity);
        myFile.print(",");
        myFile.print(hic);
        myFile.print(",");
        myFile.print(sensorVal);
        myFile.print(",");
        myFile.print(voltage);
        myFile.print(",");
        myFile.println(temp2);
        myFile.close(); //cerramos el archivo 
        Serial.print("SD escrita"); 
        digitalWrite(warning, LOW);
        digitalWrite(ok, HIGH);
        digitalWrite(bad, LOW);
  } else {
    Serial.println("Error al abrir el archivo");
    digitalWrite(warning, LOW);
    digitalWrite(ok, LOW);
    digitalWrite(bad, HIGH);
  }
  // Down the frequency to proper readings
  delay(5000);
}
