#include <Arduino.h>
#include <Wire.h>              //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_I2C.h>
#include <math.h>

#define COLUMS 16
#define ROWS   2

#define PAGE   ((COLUMS) * (ROWS))
float resCom [13] ={1,1.2,1.5,1.8,2.2,2.7,3.3,3.9,4.7,5.1,5.6,6.8,8.2};

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

float escogerRes(float r){
  int i=0;
  for(i=0;i<13;i++){
    float valor=resCom[i];
  
  
      if(r>(resCom[i]-(resCom[i]*0.091) ) && r<(resCom[i]+(resCom[i]*0.09) )){
        return(resCom[i]);
      }
      if(r>((resCom[i]-(resCom[i]*0.091) )*10) && r<((resCom[i]+(resCom[i]*0.09) )*10)){
        return(resCom[i]*10);
      }
    
      if(r>((resCom[i]-(resCom[i]*0.091) )*100) && r<((resCom[i]+(resCom[i]*0.09) )*100)){
        return(resCom[i]*100);
      }
      if(r>((resCom[i]-(resCom[i]*0.091) )*1000) && r<((resCom[i]+(resCom[i]*0.09) )*1000)){
        return(resCom[i]*1000);
      }
    if(r>((resCom[i]-(resCom[i]*0.091) )*10000) && r<((resCom[i]+(resCom[i]*0.09) )*10000)){
        return(resCom[i]*10000);
      }   
  }  
}

void setup() {
  lcd.begin(COLUMS,ROWS);
  //Serial.begin(9600);
  pinMode(bt_modo,INPUT_PULLDOWN);
  pinMode(bt_aceptar,INPUT_PULLDOWN);
  pinMode(bt_mostrar,INPUT_PULLDOWN);
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
        
        if(digitalRead(bt_modo)==HIGH){
          delay(20);
          if(digitalRead(bt_modo)==HIGH){
            selec_mode();
            while (digitalRead(bt_modo)==HIGH){
              delay(20);
            }
          }
        }
        if ((digitalRead(led_1_2w)==HIGH | digitalRead(led_1_4w)== HIGH) & digitalRead(bt_aceptar)==HIGH){
          modo = false;
          lcd.clear();
          lcd.setCursor(5,0);
          lcd.print("*Realizando*");
          lcd.setCursor(2,1);
          lcd.print("medicion");
          delay(2000);
        }
      }
      lcd.clear();
      valor = analogRead(vsensor);
      /*
        Operacion para pasar el valor ADC a resistencia

        */
      
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
}
  
