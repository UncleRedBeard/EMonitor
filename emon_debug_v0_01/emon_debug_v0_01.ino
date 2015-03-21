/*
 * File:       emon_debug_v0.01
 * Author:  Uncle RedBeard
 * Date:      02-MARCH-2015
 * Desc:      Read temp and humidity from DHT22 sensor and use LED
 *               to alert if temp / humidity is out of range
 * ToDo:     add logging function
 *               add LCD and write temp & humidty to display
 *               add millis() based delay logic for system init and each function
 * Debug:  09-MARCH-2015
 *              - removed all lcd calls
 *              - replaced with Serial.print()'s
 *              - this appears to have resolved the issue with printing temp and rh
 *              - need to add LCD.xxx calls and make sure they work properly
 *                LCD.write(12);
 *         11-MARCH-2015
 *              - added LCD updates to setup() and get_temp_rh(); code change only, currently unable to test with hardware
 *         20-MARCH-2015
 *              - testing with previous updates did not work properly
 *              - removed LCD from get_tmp_rh() and that appears to have resolved one issue
 *              - need to add LCD back into get_temp_rh()
 *         21-MARCH-2015
 *              - fixed; issue was how if was using millis() instead of delay()
 *              - need to clean up all serail references before merging back in to master
 */
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 2
#define DHTTYPE DHT22

//===Global Vars===
DHT dht(DHTPIN, DHTTYPE);

//LCD
const int LCD_PIN = 3;
SoftwareSerial LCD = SoftwareSerial(255, LCD_PIN);

//LED
int LED[] = {
  4, 5, 6, 7};  //4 = green; 5 = red; 6 = blue; 7 = yellow
int LED_COUNT = 4;

//Temperature thresholds
const float LOW_TEMP = 85.00;  //low temp warning
const float LOW_RH = 95.00;  //low humidity warning

//Delay Intervals
unsigned long READING_INTERVAL = 30000;
unsigned long LCD_WRITE_WAIT = 5;
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
  
  //LCD
  if(isnan(rh)||isnan(tempF)) {
    LCD.write(12);
    if(((unsigned long)(millis() - PREV_MILLIS) >= LCD_WRITE_WAIT)) {
      PREV_MILLIS = millis();
      LCD.print("ERROR:  Unable to");
      LCD.write(13);
      LCD.print("read DHT22");
    }
  } else {
    LCD.write(12);
    if(((unsigned long)(millis() - PREV_MILLIS) >= LCD_WRITE_WAIT)) {
      LCD.print("Temp:    ");
      LCD.print(tempF);
      LCD.print("F");
      LCD.write(13);
      LCD.print("RH:      ");
      LCD.print(rh);
      LCD.print("%");
    }
  }
    
  //LEDs
  if (tempF > LOW_TEMP) {
    digitalWrite(LED[0], HIGH);
    digitalWrite(LED[1], LOW);
  } 
  else {
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
  
  /**** Initialize LCD ****/
  LCD.begin(9600);
  LCD.write(17);
  LCD.write(22);
  LCD.write(12);
  delay(LCD_WRITE_WAIT);
  LCD.print("     Hello");
  LCD.write(13);
  LCD.print("   Uncle Red");
  delay(3000);
  LCD.write(12);
  delay(LCD_WRITE_WAIT);
  LCD.print("System Starting");
  LCD.write(13);
  LCD.print("Please Wait");
  delay(2000);

  //LCD
  LCD.write(12);
  delay(LCD_WRITE_WAIT);
  LCD.print("Initializing");
  LCD.write(13);
  LCD.print("LEDs");

  for (int pin = 0; pin < LED_COUNT; pin++) {
    pinMode(LED[pin], OUTPUT);
    digitalWrite(LED[pin], HIGH);
    delay(50);
    digitalWrite(LED[pin], LOW);
  }

  delay(1000);
  LCD.write(12);
  delay(LCD_WRITE_WAIT);
  LCD.print("oooOOO pretty");
  delay(1000);
  swingLED();
  swingLED();
  delay(250);

  /**** Initialize DHT22 *****/
  LCD.write(12);
  delay(LCD_WRITE_WAIT);
  LCD.print("Initializing");
  LCD.write(13);
  LCD.print("DHT22 sensor");
  delay(250);
  swingLED();
  dht.begin();
  delay(750);

  LCD.write(12);
  delay(5);
  LCD.print("System Running");
  
}

void loop() {
  if(((unsigned long)(millis() - PREV_MILLIS) >= READING_INTERVAL)) {
    PREV_MILLIS = millis();
    get_temp_rh();
  }
}

