/*
 * File:	emon.ino
 * Author:	Uncle RedBeard
 * Date:	28-FEB-2015
 * Desc:	Read temp and humidity from DHT22 sensor and use LED
 *	to alert if temp / humidity is out of range
 * ToDo:	add LCD and write temp & humidty to display
 *	add logging function
 */
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

// Global Vars
// DHT22 sensor declaration
DHT dht(DHTPIN, DHTTYPE);

//LED Vars
int ROW1[] = {4, 5, 6};  // 4 = Green; 5 = Red; 6 = Blue
int LEDCount = 3;

//Temperature constants
const float tempLOW = 62.00;
const float rhLOW = 40.00;

//delay params
unsigned long interval = 15000;
unsigned long prevMIllis = 0;

//functions
void swingLEDs(){
  for (int Pin = 0; Pin < LEDCount; Pin++) {
    digitalWrite(ROW1[Pin], HIGH);
    delay(50);
  }
  for (int Pin = 0; Pin < LEDCount; Pin++) {
    digitalWrite(ROW1[Pin], LOW);
    delay(50);
  }
  for (int Pin = LEDCount; Pin > -1 ; Pin--) {
    digitalWrite(ROW1[Pin], HIGH);
    delay(50);
  }
  for (int Pin = LEDCount; Pin > -1; Pin--) {
    digitalWrite(ROW1[Pin], LOW);
    delay(50);
  }
}

void tempRH(){
  float humidity = dht.readHumidity();
  float tempF = dht.readTemperature(true);
  
  if (isnan(humidity) || isnan(tempF)) {
    Serial.println("\nFailed to read DHT22 sensor.");
  } else {
    Serial.print("Temp: ");
    Serial.print(tempF);
    Serial.println("*F");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%\n");
  }
  
  if (tempF > tempLOW) {
    digitalWrite(ROW1[0], HIGH);
    digitalWrite(ROW1[1], LOW);
  } else if (tempF < tempLOW) {
    digitalWrite(ROW1[1], HIGH);
    digitalWrite(ROW1[0], LOW);
  }
  
  if (humidity > rhLOW) {
    digitalWrite(ROW1[2], HIGH);
  } else {
    digitalWrite(ROW1[2], LOW);
  }
    
}


//===================
void setup(){
  Serial.begin(9600);
  
  Serial.println("\nSystem Starting");
  Serial.println("Please wait.\n");
  
  Serial.println("Initializing LEDs");
  
  for (int Pin = 0; Pin < LEDCount; Pin++) {
    pinMode(ROW1[Pin], OUTPUT);
    digitalWrite(ROW1[Pin], HIGH);
    delay(50);
    digitalWrite(ROW1[Pin], LOW);
    delay(50);
  }
  
  delay(2000);
  swingLEDs();
  swingLEDs();
  
  Serial.println("\nInitializing DHT22 sensor...\n");
  dht.begin();
}

//+=+=+=+=+=+=+=+=+=+=+=+
void loop(){
  if(((unsigned long)(millis() - prevMIllis)) >= interval){
    prevMIllis = millis();
    
    tempRH();
  }
  
}
