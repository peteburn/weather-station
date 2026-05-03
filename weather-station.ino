// Include the libraries
#include <dhtESP32-rmt.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

// Create some variables to store the readings. They are all initialised to 0.0
float pressure, temperature, humidity, maximum, minimum = 0.0;
// Create the pressure sensor object
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
// Create a constant for the temperature and humidity input
const int DHTPIN = 2;
// Initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to. In this case we are only going to use 4 data pins (4-7).
// For simplicity we have used the same pin numbers as the Arduino. The only thing that matters is that we don't use pins 2 or 3 as these can be used for interupts.
const int rs = 4, en = 3, d4 = 5, d5 = 6, d6 = 7, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String formatTempForLCD(int temperature) {
  String formattedTemp = (String)temperature;
  if(temperature <10) {
    formattedTemp = " " + formattedTemp;
  }
  return formattedTemp;
}

void setup() {
  // Start the display. We specify 20 columns by 4 rows as the library also supports other sizes of diaply
  lcd.begin(20, 4);
  // Start the pressure sensor
  bmp.begin();
  // Set the initial values. It can take a few seconds for the first reading to be sent by the DHT22 therefore loop until we get one.
  while(temperature == 0) {
    uint8_t readings=read_dht(temperature, humidity, DHTPIN, DHT22);
  }
  maximum = temperature;
  minimum = temperature;
}

void loop() {
  // put your main code here, to run repeatedly:
  // Get the temperature from the DHT22
  uint8_t readings=read_dht(temperature, humidity, DHTPIN, DHT22);
  lcd.setCursor(0,0);
  lcd.print("Temperature:  ");
  lcd.print(formatTempForLCD((int)temperature));
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
  lcd.print(formatTempForLCD((int)maximum));
  lcd.print("C ");
  lcd.print("Min: ");
  lcd.print(formatTempForLCD((int)minimum));
  lcd.print("C");
  // Now the humidity
  lcd.setCursor(0,2);
  lcd.print("Humidity:     ");
  lcd.print((int)humidity);
  lcd.print("%");
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
  // 56 is a contant to get the barometric pressure. This is created by comparing the reading to the 
  // barometric pressure at the nearest place where it is recorded, in this case, Munich Airport.
  // It's certainly a hack but is close enough for our purposes. We only need an approximation for this project.
  pressure = (int)event.pressure +56;
  lcd.setCursor(0,3);
  lcd.print("Pressure:     ");
  lcd.print((int)pressure);
  lcd.print("mb");
  delay(5000);
}