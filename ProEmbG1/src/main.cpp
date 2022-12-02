#include <Arduino.h>
#include <Wire.h>              //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_I2C.h>

#define COLUMS 16
#define ROWS   2

#define PAGE   ((COLUMS) * (ROWS))

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

bool modo = true;
bool aceptar = true;
bool mostrar = true;

void setup() {
  lcd.begin(COLUMS,ROWS);
  lcd.print("Ecuares");
  delay(3000);
  lcd.clear();
}

void loop() {
  while (mostrar)
  {
    while (aceptar)
    {
      while (modo)
      {
        /* Programacion del modo */
      }
      
    }
    
  }
  
}