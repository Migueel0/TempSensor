//Sensor setup
#include <DHT.h>
#define DHTTYPE DHT11

//Display setup
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



const int DHTPIN = 8; 
LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN, DHTTYPE);
float previousTemperature = -999;
float previousHumidity = -999;
bool error = false;
const int LED=13;
const int BOTON=7;
bool botonPresionado = false;
int estado = LOW;




void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  pinMode(BOTON,INPUT);
  digitalWrite(LED, LOW);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.print("Epic sensor");
  delay(1000);
}

void loop() {
  
  while(digitalRead(BOTON) == LOW){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.print("Error");
    if (!error) {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Error");
      lcd.setCursor(0, 1);
      lcd.print("Check the sensor");
      error = true;
    }
    previousHumidity = -999;
    previousTemperature = -999;
    return;
  }
  error = false;
  
  if(digitalRead(LED) == 0){
    previousHumidity = -999;
    displayTemperature(t);
  }else{
    previousTemperature = -999;
    displayHumidity(h);
  }
    
  }
  estado = digitalRead(LED);
  digitalWrite(LED,!estado);
  while(digitalRead(BOTON) == HIGH);

  

}

void displayTemperature(float t){
    if (abs(t - previousTemperature) > 0.1) {
      lcd.clear();
      lcd.print("Temperatura");
      lcd.setCursor(0,1);
      lcd.print("T:");
      lcd.print(t,2);
      lcd.print((char)223);
      lcd.print("C");
      previousTemperature = t;
  }
}


void displayHumidity(float h){
    if (abs(h - previousHumidity) > 0.1) {
      lcd.clear();
      lcd.print("Humedad");
      lcd.setCursor(0,1);
      lcd.print("H:");
      lcd.print(h,2);
      lcd.print("%");
      previousHumidity = h;
  }
}
