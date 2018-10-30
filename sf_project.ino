#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 
#define BACKLIGHT_PIN     3
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);
#include<dht.h>   //t & h sensor library

int sensorPin = A0;       //soil moisture sensor input pin
int sensorValue=0;        //variable to store the value coming from the sensor(sensor value)
int percent=0;            //convert to percent 
int greenLED = 6;          
int redLED = 7;
int motorpin=8;
int thresholdValue = 800; // you can adjust the threshold value
dht DHT;                  //temperature & humidity sensor variable

void setup() {

  pinMode(motorpin,OUTPUT);    //turn of the water pump just begin the system
  digitalWrite(motorpin,LOW);  

  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);  //welcome note
  lcd.setBacklight(HIGH);  
  lcd.begin(16, 2);  
  lcd.setCursor(6,0);  
  lcd.print("Plant");
  lcd.setCursor(0, 1);
  lcd.print("Watering  System."); 
  delay(4000);
   
  lcd.clear();                  //moisture level display
  lcd.begin(16, 2); 
  lcd.setCursor(0,0); 
  lcd.println("ML :");
  
  pinMode(sensorPin, INPUT); //serial
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  
  Serial.begin(9600);

}



void loop() {

  int readData = DHT.read11(5);   //T & h sensor pin
  int t = DHT.temperature;       //get values(temperature and humidity) to the variables
  int h = DHT.humidity;
  
  int  sensorValue = analogRead(sensorPin);  //display moisture level of soil 
  lcd.setCursor(4,0); 
  percent = convertToPercent(sensorValue);    
  lcd.print(percent);                        //lcd.print(analogRead(sensorPin)); 
  lcd.setCursor(6,0);
  lcd.print("%"); 

  lcd.setCursor(9,0);      //display temperature 
  lcd.print("| T:");                  
  lcd.setCursor(13,0);
  lcd.print(t);
  lcd.setCursor(15,0);
  lcd.print("c");


  
  lcd.setCursor(0,1);     //display status
  lcd.print("St :");
   
  if(sensorValue>thresholdValue){
    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);
    Serial.print("\t Percent: ");
    Serial.print(percent);
    Serial.print("%");
    Serial.println(" - Time to water your plant");
    
    lcd.setCursor(4,1);
    lcd.println("DRY  ");

    Serial.print("Temperature = ");    //serial printing temperature & humidity
    Serial.print(t);
    Serial.print(" C ");
    Serial.print("\t Humidity = ");
    Serial.print(h);
    Serial.println(" % \n"); 

    digitalWrite(motorpin,HIGH);
    digitalWrite(redLED, HIGH);  
    digitalWrite(greenLED, LOW);   
  }
 
  
   if(sensorValue<thresholdValue){       //if(analogRead(sensorPin)<thresholdValue)
    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);
    Serial.print("\t Percent: ");
    Serial.print(percent);
    Serial.print("%");
    Serial.println(" - Doesn't need watering");
          
    lcd.setCursor(4,1);
    lcd.println("MOIST");

    Serial.print("Temperature = ");         //serial printing temperature & humidity
    Serial.print(t);
    Serial.print(" C ");
    Serial.print("\t Humidity = ");
    Serial.print(h);
    Serial.println(" % \n");

    digitalWrite(motorpin,LOW);
    digitalWrite(redLED, LOW);  
    digitalWrite(greenLED, HIGH);
  }
    
    lcd.setCursor(9,1);    //display humidity   
    lcd.print("| H:");                        
    lcd.setCursor(13,1);
    lcd.print(h);
    lcd.setCursor(15,1);
    lcd.print("%");  


 delay(1050);
    
     }

 //*********************************************************************************************************************

 
  int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1022, 292, 00, 100);     //1023,292
  return percentValue;
}

/*void printValuesToSerial()
{
  Serial.print(sensorValue);
  Serial.print(percent);
  Serial.print("%");
}*/
