/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 7 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

/*
 * LCD RS pin na digital pin 12
 * LCD Enable pin na digital pin 11
 * LCD D4 pin na digital pin 5
 * LCD D5 pin na digital pin 4
 * LCD D6 pin na digital pin 3
 * LCD D7 pin na digital pin 2
 * LCD R/W pin na GND
 * LCD VSS pin na GND
 * LCD VCC pin na 5V
 * LCD V0 pin přes potenci na zem
 *  (kontrast)
 */

 
#include "HX711.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
HX711 scale(6, 7);
const int buttonPinMeasuring = 8;
const int buttonPinCalibration = 9;

float calibration_factor = 2230; // this calibration factor is adjusted according to my load cell
float units;
float ounces;
float measuring_water;
unsigned long startMillis;
unsigned long currentMillis;
int buttonStateMeasuring = 0;
int buttonStateCalibration = 0;

void setup() {
  lcd.begin(30, 4);
  lcd.setCursor ( 2, 1 );
  lcd.print("---------------!");
  delay(1000);
  
  pinMode(buttonPinMeasuring, INPUT);  
  pinMode(buttonPinCalibration, INPUT); 

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  startMillis = millis();  //save the start time
}

void loop() {

  currentMillis = millis();
  long elapsedTimeInMinutes = ((currentMillis - startMillis) * 1000) * 0.017;

  buttonStateMeasuring = digitalRead(buttonPinMeasuring);
  buttonStateCalibration = digitalRead(buttonPinCalibration);
   
  units = scale.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;
  }

  if (buttonStateCalibration == HIGH) {
    scale.set_scale(units);
  }

  if (buttonStateMeasuring == HIGH) {
    measuring_water += units;
  }
  
  lcd.setCursor(7, 1);
  lcd.print(units);
  lcd.print("grams on scale");
  lcd.setCursor(7, 2);  
  lcd.print(measuring_water);
  lcd.print("already drink in mililitres");  
  lcd.setCursor(7, 3);  
  lcd.print(elapsedTimeInMinutes);
  lcd.print("in time"); 
  
  if(elapsedTimeInMinutes > 1500 && measuring_water < 2000){
    lcd.setCursor(7, 4);
    lcd.print("You should drink more or you will be dehydrated!!");
  } else {
    lcd.setCursor(7, 4);
    lcd.print("Please put glass on scale and press button to measure");
  }

  if(elapsedTimeInMinutes = 1560){
    measuring_water = 0;
    startMillis = 0;
    elapsedTimeInMinutes = 0;
  }
  
}
