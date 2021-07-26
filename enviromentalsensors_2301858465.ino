// C++ code
#include <LiquidCrystal.h>

//set pins for the lcd
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//variables to detect air quality
int gasValue = 0;
int gasInput = A0;
int gasAlert = 13;

//variables to detect temperatures
int baselineTemp = 0;
int celsius = 0;
int tempInput = A1;

//pins for RGB LED light
int red = 10;
int green = 6;
int blue = 9;

void setup()
{
  pinMode(gasInput, INPUT);
  pinMode(tempInput, INPUT);
  
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(gasAlert, OUTPUT);
  
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  delay(100);
  
  baselineTemp = 40;
  celsius = map(((analogRead(tempInput) - 20) * 3.04), 0, 1023, -40, 125);
  //temp sensor debug
  Serial.print("Temp: ");
  Serial.print(celsius);
  Serial.println(" Celsius");
  
  gasValue = analogRead(gasInput);
  //gas sensor debug
  Serial.print("PPM: ");
  Serial.println(gasValue);
  
  //top LED for temperatures reading
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(celsius);
  lcd.print(" °C");
  
  //change RGB light according to the temperatures
  if (celsius < baselineTemp) {
    //blue
    analogWrite(red, 0);
    analogWrite(green, 153);
    analogWrite(blue, 209);
  }else if (celsius >= baselineTemp && celsius < baselineTemp + 10) {
    //green
    analogWrite(red,64);
    analogWrite(green,184);
    analogWrite(blue,66);
  }else if (celsius >= baselineTemp + 10 && celsius < baselineTemp + 20) {
    //yellow
    analogWrite(red,227);
    analogWrite(green,208);
    analogWrite(blue,93);
  }else if (celsius >= baselineTemp + 20 && celsius < baselineTemp + 30) {
    //red
    analogWrite(red,155);
    analogWrite(green,0);
    analogWrite(blue,0);
  }else if (celsius >= baselineTemp + 30) {
    //red
    analogWrite(red,155);
    analogWrite(green,0);
    analogWrite(blue,0);
  }
  
  //bottom LED for air quality reading
  lcd.setCursor(0,1);
  lcd.print("PPM: ");
  lcd.print(gasValue);
  
  if(gasValue <= 500) {
    lcd.print("|Good");
  }
  else if(gasValue >= 500 && gasValue <= 650) {
    lcd.print("|Poor");
  }
  else if(gasValue > 650) {
    lcd.print("|Danger");
  }
  
  //if air quality is dangerous, turn on Piezo
  if(gasValue > 650) {
    digitalWrite(gasAlert, HIGH);
  }else { //turn off Piezo if air quality is acceptable
    digitalWrite(gasAlert, LOW);
  }
}