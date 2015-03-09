/*
 * File:       emon_debug_v0.01
 * Author:  Uncle RedBeard
 * Date:      02-MARCH-2015
 * Desc:      Read temp and humidity from DHT22 sensor and use LED
 *               to alert if temp / humidity is out of range
 * ToDo:     add logging function
 *               add LCD and write temp & humidty to display
 *               add millis() based delay logic for system init and each function
 */
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 2
#define DHTTYPE DHT22

//===Global Vars===
DHT dht(DHTPIN, DHTTYPE);

//LED
int LED[] = {4, 5, 6, 7};  //4 = green; 5 = red; 6 = blue; 7 = yellow
int LED_COUNT = 4;

//Temperature thresholds
const float LOW_TEMP = 78.00;  //low temp warning
const float LOW_RH = 85.00;  //low humidity warning

//Delay Intervals
unsigned long READING_INTERVAL = 30000;
unsigned long LCD_WRITE_WAIT = 5000;
unsigned long PREV_MILLIS = 0;

//===Functions===
void swingLED() {
  for (int pin = 0; pin < LED_COUNT; pin++) {
    digitalWrite(LED[pin], HIGH);
    delay(50);
  }
  for (int pin = 0; pin < LED_COUNT; pin++) {
    digitalWrite(LED[pin], LOW);
    delay(50);
  }
  for (int pin = LED_COUNT; pin > -1; pin--) {
    digitalWrite(LED[pin], HIGH);
    delay(50);
  }
  for (int pin = LED_COUNT; pin > -1; pin--) {
    digitalWrite(LED[pin], LOW);
    delay(25);
  }
}

void get_temp_rh() {
  float rh = dht.readHumidity();
  float tempF = dht.readTemperature(true);
  
  if (isnan(rh)||isnan(tempF)) {
    Serial.print("\nUnable to read DHT22 sensor\n");
  } else {
    Serial.print("\nTemperature:   ");
    Serial.print(tempF);
    Serial.print("F\n");
    Serial.print("Humidity:         ");
    Serial.print(rh);
    Serial.print("%\n");
  }
  //LEDs
  if (tempF > LOW_TEMP) {
    digitalWrite(LED[0], HIGH);
    digitalWrite(LED[1], LOW);
  } else {
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], HIGH);
  }
  if (rh > LOW_RH) {
    digitalWrite(LED[2], HIGH);
    digitalWrite(LED[3], LOW);
  } else {
    digitalWrite(LED[2], LOW);
    digitalWrite(LED[3], HIGH);
  }
}
//================
void setup() {
  /**** Initialize Serial Port ****/
  Serial.begin(9600);

  /***** Initialize LEDs *****/
  Serial.print("Initializing LEDs\n");
  delay(500);
  
  for (int pin = 0; pin < LED_COUNT; pin++) {
    pinMode(LED[pin], OUTPUT);
    digitalWrite(LED[pin], HIGH);
    delay(50);
    digitalWrite(LED[pin], LOW);
  }
  
  delay(500);
  Serial.print("oooOOOO pretty\n");
  delay(1000);
  swingLED();
  swingLED();
  delay(250);
  
  /**** Initialize DHT22 *****/
  delay(500);
  Serial.print("Initializing DHT22 sensor\n");
  delay(250);
  swingLED();
  dht.begin();
  delay(750);
  
  Serial.print("System Ready\n");
  
}

void loop() {
  if(((unsigned long)(millis() - PREV_MILLIS) >= READING_INTERVAL)) {
    PREV_MILLIS = millis();
    get_temp_rh();
  }
}
