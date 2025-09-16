/*
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
*/

// THIS CODE IS CURRENTLY BUILT FOR: Kale (Dwarf Blue Curled)
// THIS CODE IS CURRENTLY TESTING: Moisture sensor reading

#define MOISTURE_PIN 13

int moisture = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOISTURE_PIN, INPUT);
  moisture = analogRead(MOISTURE_PIN);
  Serial.print("The moisture level of this pot is currently: ");
  Serial.println(moisture);
  Serial.print("\n\n How does this compare to the recommended mositure level for [Kale (Dwarf Blue Curled)]")

  
}

void loop() {

}
