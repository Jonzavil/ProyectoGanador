/*
  Proyecto de Sistemas embebidos
  Medidor de resistencias 1/2W y 1/4W
*/

//librerias a usar del proyecto
#include <Arduino.h>
#include <Wire.h>      
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

//variables de la LCD I2C
#define COLUMS 16
#define ROWS   2
#define PAGE   ((COLUMS) * (ROWS))
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

// inicializacion de la conexion hacia la base de datos en Firebase
#define WIFI_SSID "MOVISTAR WIFI5026"
#define WIFI_PASSWORD "sjiv5595"
#define API_KEY "AIzaSyBsUDmoqRCqq600MhwRt0SMo1qAdPZ36vo"
#define FIREBASE_PROJECT_ID "ecuares-7bda5"
#define FIREBASE_CLIENT_EMAIL "AIzaSyBsUDmoqRCqq600MhwRt0SMo1qAdPZ36vo"
#define FIREBASE_HOST "https://console.firebase.google.com/project/ecuares-7bda5/database/ecuares-7bda5-default-rtdb/data/~2F"
#define FIREBASE_AUTH "BNPiLp2vfzcQfmtZIzGGgMVFDTpIT3jL72kl4dGo"
#define USER_EMAIL "cepaquin@epol.edu.ec"
#define USER_PASSWORD "1234656to"

// Matriz de factores comerciales de resistencias
float resCom [14] ={1,1.2,1.5,1.8,2.2,2.4,2.7,3.3,3.9,4.7,5.1,5.6,6.8,8.2};

// Perifericos de salida
int led_1_4w = 4;
int led_1_2w = 5;
int buzzer = 2;

// Botones (Perifericos de entrada)
int bt_modo = 26;
int bt_aceptar = 27;
int bt_mostrar = 12;

// inicializacion de modos del sistema
bool modo = true;
bool aceptar = true;
bool mostrar = true;


FirebaseData firebaseData;
FirebaseJson json;
String path = "/esp32";

// Inicializacion del primer ADC (1/4W)
const int vsensorA = 35;

// Inicializacion del segundo ADC (1/2W)
const int vsensorB = 32;

// Elegir el ADC 
bool adc_select;

void selec_mode(){
  lcd.clear();
  if(digitalRead(led_1_4w)== HIGH){
    digitalWrite(led_1_2w,HIGH);
    digitalWrite(led_1_4w,LOW);
    lcd.setCursor(0,0);
    lcd.print("Select mode:");
    lcd.setCursor(3,1);
    lcd.print("1/2W");
    adc_select = true;
  }else
  {
    digitalWrite(led_1_2w,LOW);
    digitalWrite(led_1_4w,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Select mode:");
    lcd.setCursor(3,1);
    lcd.print("1/4W");
    adc_select = false;
  }
  
}

float escogerRes(float r){
  int i=0;
  for(i=0;i<14;i++){
    float valorA=resCom[i];
  
  
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

float calculoRes(){
  float voltage = 0;
  int muestras = 100;

  for (int i=0;i<muestras;i++){
    if (!adc_select)
    {
      voltage = voltage + analogRead(vsensorA)*(3.313/4095.0);
    }
    else
    {
      voltage = voltage + analogRead(vsensorB)*(3.313/4095.0);
    }
    
  }
  
  voltage = (voltage/muestras)+0.2;

  float current = (3.313-voltage)/1000;

  float res = voltage/current;

  return escogerRes(res);

}


void tono(){
  digitalWrite(buzzer,HIGH);
  delay(50);
  digitalWrite(buzzer,LOW);
}


void setup() {
  lcd.begin(COLUMS,ROWS);
  
  pinMode(bt_modo,INPUT_PULLDOWN);
  pinMode(bt_aceptar,INPUT_PULLDOWN);
  pinMode(bt_mostrar,INPUT_PULLDOWN);
  pinMode(vsensorA,INPUT);
  pinMode(vsensorB,INPUT);

  pinMode(led_1_2w,OUTPUT);
  pinMode(led_1_4w,OUTPUT);
  pinMode(buzzer,OUTPUT);

  lcd.setCursor(2,0);
  lcd.print("**Ecuares**");
  delay(3000);
  lcd.clear();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0,0);
  lcd.print("Connecting Wi-Fi");
  lcd.setCursor(0,1);
  while (WiFi.status() != WL_CONNECTED)

  {
    lcd.print(".");

    delay(300);

  }
  lcd.clear();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
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
          tono();
          delay(20);
          if(digitalRead(bt_modo)==HIGH){
            selec_mode();
            while (digitalRead(bt_modo)==HIGH){
              delay(20);
            }
          }
        }
        if ((digitalRead(led_1_2w)==HIGH | digitalRead(led_1_4w)== HIGH) & digitalRead(bt_aceptar)==HIGH){
          tono();
          modo = false;
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("*Realizando*");
          lcd.setCursor(2,1);
          lcd.print("medicion");
          delay(2000);
        }
      }
      lcd.clear();


      float valorRes = calculoRes();
      lcd.setCursor(2,0);
      lcd.printf("R= %.2f",valorRes);      
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
  
