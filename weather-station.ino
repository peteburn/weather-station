// Include the libraries
#include <dhtESP32-rmt.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

// Define the connections pins
#define DHTPIN 2

float pressure = 0.0;
float temperature = 0.0;
float humidity = 0.0;
float maximum = 0.0;
float minimum = 0.0;

const uint8_t blank[] = {};

// Create the pressure sensor object
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to. In this case we are only going to use 4 data pins (4-7).
// For simplicity we have used the same pin numbers as the Arduino. The only thing that matters is that we don't use pins 2 or 3 as these can be used for interupts.
const int rs = 4, en = 3, d4 = 5, d5 = 6, d6 = 7, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  bmp.begin();
  // Set the initial values.
  while(temperature == 0) {
    uint8_t readings=read_dht(temperature, humidity, 2, DHT22);
  }
  maximum = temperature;
  minimum = temperature;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t readings=read_dht(temperature, humidity, 2, DHT22);
  lcd.setCursor(0,0);
  lcd.print("Temperature:  ");
  lcd.print((int)temperature);
  lcd.print("C");
  //Calculate the maximum and minimum.
  if(temperature > maximum) {
    maximum = temperature;
  }
  if(temperature < minimum) {
    minimum = temperature;
  }
  lcd.setCursor(0,1);
  lcd.print("Max: ");
  lcd.print(int(maximum));
  lcd.print("C ");
  lcd.print("Min: ");
  lcd.print((int)minimum);
  lcd.print("C");
  // Now the humidity
  lcd.setCursor(0,2);
  lcd.print("Humidity:     ");
  lcd.print((int)humidity);
  lcd.print("%");
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
  pressure = (int)event.pressure;
  lcd.setCursor(0,3);
  lcd.print("Pressure:     ");
  lcd.print((int)pressure);
  lcd.print("mb");
  delay(5000);
}