//Preasure sensor setup
#include <Adafruit_BMP085.h>

//Temp sensor setup
#include <DHT.h>
#define DHTTYPE DHT11

//Display setup
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int DHTPIN = 8;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
const float MIN_FLOAT_VALUE = -3.4028235E+38;
float previousTemperature, previousHumidity, previousPressure = MIN_FLOAT_VALUE;
bool error = false;
const int led1 = 13;
const int led2 = 12;
const int led3 = 11;
const int BUTTON = 7;
bool botonPresionado = false;
int state = 0;

float humidity;
float temperature;
float pressure;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.print("Epic sensor");
  delay(1000);
}

void loop() {
  readValue();
  while (digitalRead(BUTTON) == LOW) {

    //error = checkErrors();
    //if (error)
    //There has been an error, break while loop
    //return;

    //displayValue();
    turnLeds();
  }
  nextState();
  Serial.print("\n");
  while (digitalRead(BUTTON) == HIGH)
    ;
}

void displayTemperature() {

  if (abs(temperature - previousTemperature) > 0.2) {
    Serial.print("t");
    lcd.clear();
    lcd.print("Temperatura");
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(temperature, 2);
    lcd.print((char)223);
    lcd.print("C");
    previousTemperature = temperature;
  }
}

void displayPressure() {

  if (abs(pressure - previousPressure) > 100) {
    Serial.print("p");
    lcd.clear();
    lcd.print("Pressure");
    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print(round(pressure));
    lcd.print(" Pa");
    previousPressure = pressure;
  }
}

void displayHumidity() {
  if (abs(humidity - previousHumidity) > 0.2) {
    Serial.print("h");
    lcd.clear();
    lcd.print("Humedad");
    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(humidity, 2);
    lcd.print("%");
    previousHumidity = humidity;
  }
}

void turnLeds() {
  if (state == 0) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    //previousTemperature, previousPressure = MIN_FLOAT_VALUE;
    displayHumidity();
  } else if (state == 1) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    previousHumidity, previousPressure = MIN_FLOAT_VALUE;
    displayTemperature();
  } else if (state == 2) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    previousTemperature, previousHumidity = MIN_FLOAT_VALUE;
    displayPressure();
  }
}

/*void displayValue() {
  if (state == 0) {

  } else if (state == 1) {

  } else if (state == 2) {

  }
}
*/
void readValue() {
  if (state == 0) {
    humidity = dht.readHumidity();
    previousTemperature, previousPressure = MIN_FLOAT_VALUE;
    temperature = NULL;
    pressure = NULL;
  } else if (state == 1) {
    temperature = dht.readTemperature();
    previousHumidity, previousPressure = MIN_FLOAT_VALUE;
    pressure = NULL;
    humidity = NULL;
  } else if (state == 2) {
    pressure = bmp.readPressure();
    previousTemperature, previousHumidity = MIN_FLOAT_VALUE;
    temperature = NULL;
    humidity = NULL;
  }
}

boolean checkErrors() {
  if (isnan(humidity) && isnan(temperature) && isnan(pressure)) {
    Serial.print("Error");
    if (!error) {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Error");
      lcd.setCursor(0, 1);
      lcd.print("Check the sensor");
      error = true;
    }
    previousHumidity = MIN_FLOAT_VALUE;
    previousTemperature = MIN_FLOAT_VALUE;
    previousPressure = MIN_FLOAT_VALUE;
    return true;
  }
  return false;
}

void nextState() {
  if (state < 2)
    state++;
  else
    state = 0;
}