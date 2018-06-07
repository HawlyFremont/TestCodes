#include <LiquidCrystal.h> //申明1602液晶的函数库
#include<OneWire.h>      //调用Arduino单总线库
#include<DallasTemperature.h> //调用ds18b20温度传感器库


#define ONE_WIRE_BUS 2    //设置单总线数据I/O口为2号引脚

OneWire temp(ONE_WIRE_BUS);  //初始化一个单总线类，以2号引脚作为输出口
DallasTemperature sensors(&temp); //初始化一个温度传感器类。DS18B20

//申明1602液晶的引脚所连接的Arduino数字端口，8线或4线数据模式，任选其一
//LiquidCrystal lcd(12,11,10,9,8,7,6,5,4,3,2);   //8数据口模式连线声明
LiquidCrystal lcd(12,11,10,6,7,8,9); //4数据口模式连线声明，主板上的线顺序要和显示器上的一致, LiquidCrystal(rs, rw, enable, d, d, d, d)
//LiquidCrystal lcd(12,11,5,4,3,2); //4数据口模式连线声明，主板上的线顺序要和显示器上的一致, LiquidCrystal(rs, rw, enable, d, d, d, d)

int i;
int pirPin = 3; // Input for HC-S501
int laserPin = 4; //定义激光感应器输出
int pirValue; // Place to store read PIR Value
float temperature = 0;       //设置temperature为浮点变量

void flashingAndScrolling(){
   for(i=0;i<3;i++)
    {
      lcd.noDisplay();
      delay(500);
      lcd.display();
      delay(500);
    }
    for(i=0;i<24;i++)
    {
      lcd.scrollDisplayLeft();
      delay(500);
    }
    lcd.clear();       
    delay(500);
}

void showHello()
{  
  lcd.home();        //把光标移回左上角   
  lcd.clear();
  lcd.print("Hello World!");
  lcd.setCursor(0,1);   //把光标定位在第1行，第0列
  lcd.print("Arduino is fun.");    
}

void showTemperature()                     
{ 
  sensors.requestTemperatures();  //对传感器进行一个温度请求
  temperature=sensors.getTempCByIndex(0);  //读取传感器输出的数据，以摄氏度为单位赋值给temperature变量。
  delay(100);  
  lcd.clear();          //清屏
  delay(200);
  lcd.home();
  lcd.print("Temperature is:");   //使屏幕显示文字
  lcd.setCursor(0, 1) ;      //设置光标位置为第二行第一个位置
  lcd.print(temperature);   
  lcd.print((char)223);      //显示o符号
  lcd.print("C");          //显示字母C 
}

void showPirInfo()
{  
  lcd.home();        //把光标移回左上角   
  lcd.clear();
  lcd.print("Attention:");
  lcd.setCursor(0,1);   //把光标定位在第1行，第0列
  lcd.print("Detect somebody!");    
}

void setup()
{
  pinMode(pirPin, INPUT);
  pinMode(laserPin, OUTPUT);
  delay(200);   
  //Serial.begin(9600);       //初始化串口
  lcd.begin(16,2);     
  sensors.begin();        //初始化温度传感器   
  delay(200); 
}
void loop()
{
  pirValue = LOW;
  pirValue = digitalRead(pirPin);
  if(pirValue==HIGH){
    digitalWrite(laserPin,pirValue);
    delay(2000);
    showPirInfo();
    digitalWrite(laserPin,LOW);
    delay(1000);
    flashingAndScrolling(); 
  }else{
//    digitalWrite(laserPin,LOW);
//    showHello();
//    delay(1000);           //延时1秒，刷新速度。
//    flashingAndScrolling();
    
    showTemperature();
    delay(1000);           //延时1秒，刷新速度。
    //flashingAndScrolling();
  }
}
