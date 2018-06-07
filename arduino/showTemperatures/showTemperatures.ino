#include <LiquidCrystal.h>       //调用Arduino自带的LiquidCrystal库
#include<OneWire.h>     //调用Arduino单总线库
#include<DallasTemperature.h> //调用ds18b20温度传感器库


#define ONE_WIRE_BUS 2    //设置单总线数据I/O口为2号引脚
OneWire temp(ONE_WIRE_BUS);  //初始化一个单总线类，以2号引脚作为输出口
DallasTemperature sensors(&temp); //初始化一个温度传感器类。
LiquidCrystal lcd(12,11,10,9,8,7,6); //4数据口模式连线声明
float temperature = 0;      //设置temperature为浮点变量
int i;

void showWelcome(){
    lcd.home();        //把光标移回左上角，即从头开始输出   
    lcd.print("Hello World"); //显示
    lcd.setCursor(0,1);   //把光标定位在第1行，第0列
    lcd.print("Welcome to BST-Arduino");       //显示
    delay(500);
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
    lcd.setCursor(0,0);        //把光标移回左上角，即从头开始输出   
    lcd.print("Hi,"); //显示
    lcd.setCursor(0,1);   //把光标定位在第1行，第0列
    lcd.print("Arduino is fun");       //显示
    delay(2000);
}

void showTemperature(){
  sensors.requestTemperatures();  //对传感器进行一个温度请求
  temperature=sensors.getTempCByIndex(0);  //读取传感器输出的数据，以摄氏度为单位赋值给temperature变量。
  delay(100);
  temperature = (temperature*10);   //把读取到的temperature转换为10倍
  lcd.clear();          //清屏
  lcd.print("temperature is ");   //使屏幕显示文字
  lcd.setCursor(0, 1) ;      //设置光标位置为第二行第一个位置
  lcd.print((long)temperature / 10);   //显示温度整数位
  lcd.print(".");          //显示小数点
  lcd.print( (long)temperature % 10);  //显示温度小数点后一位
  lcd.print((char)223);      //显示o符号
  lcd.print("C");          //显示字母C
  delay(1000);           //延时1秒，刷新速度。
}

void setup()
{
  Serial.begin(9600);       //初始化串口
  sensors.begin();        //初始化温度传感器
  lcd.begin(16, 2);       //初始化LCD  
}

void loop ()                     
{
 showWelcome();
 //showTemperature(); 
}
