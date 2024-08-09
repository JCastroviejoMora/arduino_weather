// LCD library and configuration
#include <SD.h>
#include <DHT.h>
#define DHTPIN_1 7
#define DHTPIN_2 4
#define DHTTYPE DHT11
DHT dht_1(DHTPIN_1, DHTTYPE);
DHT dht_2(DHTPIN_2, DHTTYPE);

// Initializing 3d Termometer constants
const int sensorPin = A0;
// Initializing Led system
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
  
  dht_1.begin();
  dht_2.begin();
  if (!SD.begin(10)) {
    Serial.println("Error reaching the SC component.");
    digitalWrite(warning, HIGH);
    digitalWrite(ok, LOW);
    digitalWrite(bad, LOW);
    return;
  }
}

void loop() {
  // Measure Running time
  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  int runHours= allSeconds/3600;
  int secsRemaining=allSeconds%3600;
  int runMinutes=secsRemaining/60;
  int runSeconds=secsRemaining%60;
  char buf[14];
  sprintf(buf,"%02d:%02d:%02d",runHours,runMinutes,runSeconds);
  Serial.println(buf);
  // Read Temperature DHT 1
  float temp_1 = dht_1.readTemperature();
  float hum_1 = dht_1.readHumidity();
  float hic_1 = dht_1.computeHeatIndex(temp_1,hum_1,false);
  // Read Temperature DHT 2
  float temp_2 = dht_2.readTemperature();
  float hum_2 = dht_2.readHumidity();
  float hic_2 = dht_2.computeHeatIndex(temp_2,hum_2,false);
  // Read Temperature third sensor
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0)*5.0;
  float temp_3 = (voltage - .5) * 100;
  // Printing the results
  Serial.print("Sensor #1 \t Temperature: ");
  Serial.print(temp_1);
  Serial.print(", Humidity: ");
  Serial.print(hum_1);
  Serial.print(", HIC: ");
  Serial.println(hic_1);
  Serial.print("Sensor #2 \t Temperature: ");
  Serial.print(temp_2);
  Serial.print(", Humidity: ");
  Serial.print(hum_2);
  Serial.print(", HIC: ");
  Serial.println(hic_2);
  Serial.print("Sensor #3 \t Temperature: ");
  Serial.print(temp_3);
  Serial.print(", Voltage: ");
  Serial.println(voltage);
  Serial.println("----------------------------");

  myFile = SD.open("datalog.txt", FILE_WRITE);//abrimos  el archivo
  if (myFile) { 
        Serial.print("Escribiendo SD: ");
        
        myFile.print(buf);
        myFile.print(",");
        myFile.print(millis());
        myFile.print(",");
        myFile.print(temp_1);
        myFile.print(",");
        myFile.print(hum_1);
        myFile.print(",");
        myFile.print(hic_1);
        myFile.print(",");
        myFile.print(temp_2);
        myFile.print(",");
        myFile.print(hum_2);
        myFile.print(",");
        myFile.print(hic_2);
        myFile.print(",");
        myFile.print(temp_3);
        myFile.print(",");
        myFile.print(sensorVal);
        myFile.print(",");
        myFile.println(voltage);
        myFile.close(); //cerramos el archivo 
        Serial.print("SD escrita"); 
        digitalWrite(warning, LOW);
        digitalWrite(ok, HIGH);
        digitalWrite(bad, LOW);
  } else {
    Serial.println("Error opening the output File");
    digitalWrite(warning, LOW);
    digitalWrite(ok, LOW);
    digitalWrite(bad, HIGH);
  }
  // Down the frequency to proper readings
  delay(5000);
}
