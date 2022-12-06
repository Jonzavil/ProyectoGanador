#include <Arduino.h>
#include <Wire.h>              //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_I2C.h>
#include <math.h>

#define COLUMS 16
#define ROWS   2

#define PAGE   ((COLUMS) * (ROWS))


int led_1_4w = 4;
int led_1_2w = 5;
int buzzer = 2;

int bt_modo = 26;
int bt_aceptar = 27;
int bt_mostrar = 12;

const int vsensor = 36;
int valor = 0;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

bool modo = true;
bool aceptar = true;
bool mostrar = true;

void selec_mode(){
  lcd.clear();
  if(digitalRead(led_1_4w)== HIGH){
    digitalWrite(led_1_2w,HIGH);
    digitalWrite(led_1_4w,LOW);
    lcd.setCursor(0,0);
    lcd.print("Select mode:");
    lcd.setCursor(3,1);
    lcd.print("1/2W");
  }else
  {
    digitalWrite(led_1_2w,LOW);
    digitalWrite(led_1_4w,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Select mode:");
    lcd.setCursor(3,1);
    lcd.print("1/4W");
  }
  
}

void setup() {
  lcd.begin(COLUMS,ROWS);
  //Serial.begin(9600);
  pinMode(bt_modo,INPUT_PULLUP);
  pinMode(bt_aceptar,INPUT_PULLUP);
  pinMode(bt_mostrar,INPUT_PULLUP);
  pinMode(vsensor,INPUT);

  pinMode(led_1_2w,OUTPUT);
  pinMode(led_1_4w,OUTPUT);
  pinMode(buzzer,OUTPUT);
  delay(2000);
  lcd.print("Ecuares");
  delay(3000);
  lcd.clear();
}

void loop() {
  
  while (mostrar)
  {
    while (aceptar)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Iniciar medicion");
      while (modo)
      {
        
        if(digitalRead(bt_modo)==LOW){
          delay(20);
          if(digitalRead(bt_modo)==LOW){
            selec_mode();
            while (digitalRead(bt_modo)==LOW){
              delay(20);
            }
          }
        }
        if ((digitalRead(led_1_2w)==HIGH | digitalRead(led_1_4w)== HIGH) & digitalRead(bt_aceptar)==LOW){
          modo = false;
          lcd.clear();
          lcd.setCursor(5,0);
          lcd.print("*Modo*");
          lcd.setCursor(2,1);
          lcd.print("seleccionado");
          delay(2000);
        }
      }
      lcd.clear();
      valor = analogRead(vsensor);
      /*
        Operacion para pasar el valor ADC a resistencia

        */
     



}


      
      lcd.setCursor(2,0);
      int v=0;

      /*
        Metodo para pasar el valor a string
      */
      lcd.printf("%d",valor);      
      lcd.setCursor(2,1);
      if (digitalRead(led_1_2w)==HIGH){
        lcd.print("P=1/2W");
      }else{
        lcd.print("P=1/4W");
      }
      delay(1500);
      digitalWrite(led_1_2w,LOW);
      digitalWrite(led_1_4w,LOW);
      /*
        Almacenar resistncia y potencia
      */
      modo = true;
    }
    
}
  
