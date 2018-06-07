#include <Wire.h>
#include<OneWire.h>      //调用Arduino单总线库
#include <LCD.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include<DallasTemperature.h> //调用ds18b20温度传感器库

#define ONE_WIRE_BUS 2    //设置单总线数据I/O口为2号引脚

int pirPin = 3; // Input for HC-S501
int laserPin = 4; //定义激光感应器输出
int pirValue = LOW; // Place to store read PIR Value
float temperature = 0;   
char charbuf[6];

OneWire temp(ONE_WIRE_BUS);  //初始化一个单总线类，以2号引脚作为输出口
DallasTemperature tSensors(&temp); //初始化一个温度传感器类。DS18B20
LiquidCrystal_I2C mLcd(0x27, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the I2C bus address for an unmodified backpack

int lcdBlink(LiquidCrystal_I2C lcd){
  int i = 0;
  for(i=0;i<3;i++)
  {
    lcd.noDisplay();
    delay(500);
    lcd.display();
    delay(500);
  }
  return 0;
}

int lcdScroll(LiquidCrystal_I2C lcd){
  int i = 0;
  for(i=0;i<16;i++)
  {
    lcd.scrollDisplayLeft();
    delay(500);
  }
  return 0;
}


int lcdShowContents(LiquidCrystal_I2C lcd, String firstLineStr, String secondLineStr, bool needBlink, bool needStroll) {
  lcd.clear();
  lcd.home ();
  delay(100);
  if(firstLineStr!=NULL && firstLineStr.length() > 0){
      lcd.print(firstLineStr);
  }
  
  if(secondLineStr!=NULL && secondLineStr.length() > 0){
      lcd.setCursor(0,1); 
      lcd.print(secondLineStr);
  }
  delay(1000);

  if(needBlink){
    lcdBlink(lcd);
  }

  if(needStroll){
    lcdScroll(lcd);
  }

  return 0;
}

float getTemperature(DallasTemperature sensors)                     
{ 
  float f = 0.0;       //设置temperature为浮点变量  
  sensors.requestTemperatures();  //对传感器进行一个温度请求
  f=sensors.getTempCByIndex(0);  //读取传感器输出的数据，以摄氏度为单位赋值给temperature变量。
  return f;   
}

void setup() { 
  pinMode(pirPin, INPUT);
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin,LOW);
  tSensors.begin();        //初始化温度传感器
  delay(200);
  mLcd.begin (16, 2); // for 16 x 2 LCD module
  mLcd.setBacklightPin(3, POSITIVE);
  mLcd.setBacklight(HIGH);
  //mLcd.setBacklight(LOW); //backlight off 
}
void loop() {
  
   pirValue = LOW;
   pirValue = digitalRead(pirPin);   
   if(pirValue==HIGH){
    digitalWrite(laserPin,HIGH);
    //delay(1000);
    lcdShowContents(mLcd, "Attention:", "Somebody moving!", true, true);    
    digitalWrite(laserPin,LOW); 
  }else{
    digitalWrite(laserPin,LOW);         
  }
  temperature = getTemperature(tSensors);
  delay(100);
  dtostrf(temperature, 5, 2, charbuf);
  //lcdShowContents(mLcd, "Temperature is:", charbuf + (char)223 + "C", true, true); 
  lcdShowContents(mLcd, "Temperature is:", charbuf, true, true); 
}

