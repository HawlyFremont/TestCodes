#include <OneWire.h>

OneWire  ds(8);  // 连接引脚8
 
void setup(void) {
  Serial.begin(9600);
}
 
void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
   
  if ( !ds.search(addr)) //搜索获取18b20的地址序列码存入addr
  {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
   
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);//读取其地址序列码并用串口显示出来
  }
 
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");//CR：循环冗余校验
      return;
  }
  Serial.println();
  
  //通过ROM的第一字节判断18b20是哪种型号
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  //  旧款DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 
 
  ds.reset();//初始化
  ds.select(addr);//选择18b20
  ds.write(0x44,1);         // 启动ds18b20进行温度转，结果存入内部9字节RAM中
   
  delay(1000);     //转换时间1000m，实际上可能750ms就足够，保证有足够的转换时间
  
  present = ds.reset();//初始化
  ds.select(addr);//选择18b20    
  ds.write(0xBE);         //读取内部RAM中9字节的内容
 
  Serial.print("  Data = ");
  Serial.print(present,HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {  // 逐一把其中9个字节的内容放进data里面
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);//串口打印CRC校验码
  Serial.println();
 
  // 9字节的data数据转换成温度度数
 
  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);//精度是低4位决定，因为低4位是代表的小数，低4位，从高到低的每一位2^(-n)次方
                               //所以数据左移即可改变精，所以精度分别是0.5,0.25,0.125,0.0625
    if (cfg == 0x00) raw = raw << 3;  // 9 bit的温度计分辨率, 最大转换时间为93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit的温度计分辨率, 最大转换时间为187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit的温度计分辨率, 最大转换时间为375 ms
    // 默认的是12 bit的温度计分辨率, 最大转换时间为750 ms
  }
  celsius = (float)raw / 16.0;//12位分辨，1位代表0.0625，即除以16得出摄氏度
  fahrenheit = celsius * 1.8 + 32.0;//摄氏度转华氏度
  Serial.print("  Temperature = ");//串口显示摄氏度
  Serial.print(celsius);
  Serial.print(" Celsius, "); //串口显示华氏度  
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}
