/*
 * File:	emon.ino
 * Author:	Uncle RedBeard
 * Date:    02-MARCH-2015
 * Desc:    Read temp and humidity from DHT22 sensor and use LED
 *          to alert if temp / humidity is out of range
 * ToDo:    add logging function
 *			add sensor and logic for soil / substrate temperature and moisture levels
 * Note:    swingLED() based on Adam N Outler's Arduino based EMF Meter code
 *          http://www.hyundaiaftermarket.org/forum/index.php?/blog/3/entry-26-arduino-emf-meter/
 */
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 2
#define DHTTYPE DHT22

//=== Global Vars ===
//DHT22 sensor
DHT dht(DHTPIN, DHTTYPE);

//LCD
const int LCD_PIN = 3;
SoftwareSerial LCD = SoftwareSerial(255, LCD_PIN);

//LED
int LED[] = {4, 5, 6, 7};  //4 = green; 5 = red; 6 = blue; 7 = yellow
int LED_COUNT = 4;

//Temperature thresholds
const float TEMP_LOW = 78.00;  //low temp warning
const float RH_LOW = 85.00;  //low humidity warning

//Delay intervals
unsigned long READING_INTERVAL = 30000;
unsigned long LCD_WRITE_WAIT = 5;
unsigned long PREV_MILLIS = 0;

//=== Functions ===
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

void get_temp_rh(){
  float rh = dht.readHumidity();
  float tempF = dht.readTemperature(true);
  
  if(isnan(rh)||isnan(tempF)){
    LCD.write(12);
    if(((unsigned long)(millis() - PREV_MILLIS) >= LCD_WRITE_WAIT)){
      PREV_MILLIS = millis();
      LCD.print("Failed to read");
      LCD.write(13);
      LCD.print("DHT22 sensor");
    } else {
      LCD.write(12);
      if(((unsigned long)(millis() - PREV_MILLIS) >= LCD_WRITE_WAIT)){
        PREV_MILLIS = millis();
        //write to LCD
        LCD.print("Temp:    ");
        LCD.print(tempF);
        LCD.print("%");
        LCD.write(13);
        LCD.print("RH:      ");
        LCD.print(rh);
        LCD.print("%");        
      }
    }
    //turn on|off LEDs
    if (tempF > TEMP_LOW) {        //turn on|off temperarute leds
      digitalWrite(LED[0], HIGH);  //turn on green led
      digitalWrite(LED[1], LOW);   //turn off red led
    } else {
      digitalWrite(LED[0], LOW);   //turn off green led
      digitalWrite(LED[1], HIGH);  //turn on red led
    }
    if (rh > RH_LOW) {
      digitalWrite(LED[2], HIGH);  //turn on blue led
      digitalWrite(LED[3], LOW);   //turn off yellow led
    } else {
      digitalWrite(LED[2], LOW);   //turn off blue led
      digitalWrite(LED[3], HIGH);  //turn on yellow led
    }
  }
}

//==============
void setup() {
  
  //-+-+-+-Initialize LCD-+-+-+-
  LCD.begin(9600);
  LCD.write(17);
  LCD.write(12);
  LCD.write(22);
  delay(5);
  LCD.print("Hello there");
  LCD.write(13);
  LCD.print("Uncle Red");
  delay(3000);
  LCD.write(12);
  delay(5);
  LCD.print("System Starting");
  LCD.write(13);
  LCD.print("Please Wait");
  delay(5000);
  
  //-+-+-+-Initialize LEDs-+-+-+-
  LCD.write(12);
  delay(5);
  LCD.print("Init LEDs");
  delay(500);
  
  for (int pin = 0; pin < LED_COUNT; pin ++) {
    pinMode(LED[pin], OUTPUT);
    digitalWrite(LED[pin], HIGH);
    delay(50);
    digitalWrite(LED[pin], LOW);
  }
  
  delay(500);
  LCD.write(12);
  delay(1000);
  LCD.print("oooOOO pretty");
  swingLED();
  swingLED();
  delay(250);
  
  //-+-+-+-Initialize DHT22-+-+-+-
  LCD.write(12);
  delay(500);
  LCD.print("Initializing");
  LCD.write(13);
  LCD.print("DHT22 sensor");
  delay(250);
  swingLED();
  dht.begin();
  delay(750);
  
  LCD.write(12);
  delay(5);
  LCD.print("System Ready");
  
}

void loop(){
  get_temp_rh;
}
