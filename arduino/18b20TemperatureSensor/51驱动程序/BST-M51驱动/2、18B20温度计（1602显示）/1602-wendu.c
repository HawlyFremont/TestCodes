/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：18b20温度显示（1602液晶屏）实验
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-7
*  晶振:11.0592MHZ
*  实验效果：接上18B20温度传感器1602液晶屏显示出当前温度
******************************************************************/
#include <reg52.H>
#include <intrins.H>
#include <math.H>

#define uchar unsigned char
  #define uint unsigned int
 sbit dula = P2^6;
 sbit wela = P2^7;
 sbit rw = P1^1; 
 sbit RS = P1^0;  
  
 sbit LCDEN = P2^5; 

void delayUs()
{
    _nop_();
}

 void delayMs(uint a)
{
    uint i, j;
    for(i = a; i > 0; i--)
        for(j = 100; j > 0; j--);
 }

/*****************************1602*******************************/
void writeComm(uchar comm)
{
     RS = 0;    
    P0 = comm;
    LCDEN = 1;
     delayUs();
    LCDEN = 0;
    delayMs(1);
}

//写数据:RS=1, RW=0;
void writeData(uchar dat)
{
     RS = 1;
     P0 = dat;
     LCDEN = 1;
    delayUs();
    LCDEN = 0;
    delayMs(1);
 }


 void init()
 {
     rw = 0; 
     dula = wela = 0;
    writeComm(0x38);
   writeComm(0x0c); 
    writeComm(0x06);
    writeComm(0x01); 
}

void writeString(uchar * str, uchar length)
{
     uchar i;
    for(i = 0; i < length; i++)
    {
         writeData(str[i]);
     }
 }
 
/*****************************DS18B20*******************************/
 sbit ds = P2^2;
void dsInit()
 {
    
    unsigned int i;  
    ds = 0;
    i = 100;  
     while(i>0) i--;
    ds = 1;   
    i = 4;
     while(i>0) i--;
 }
 
void dsWait()
 {
      unsigned int i;
      while(ds);  
      while(~ds);
      i = 4;
      while(i > 0) i--;
}


bit readBit()
{
    unsigned int i;
    bit b;
    ds = 0;
    i++;   
    ds = 1; 
   i++; i++;  
    b = ds;
    i = 8; 
    while(i>0) i--;
    return b;
}

unsigned char readByte()
{
    unsigned int i;
    unsigned char j, dat;
   dat = 0;
    for(i=0; i<8; i++)
    {
        j = readBit();
      
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}


void writeByte(unsigned char dat)
{
    unsigned int i;
    unsigned char j;
    bit b;
    for(j = 0; j < 8; j++)
    {
        b = dat & 0x01;
        dat >>= 1;
    
        if(b)   
        {
           ds = 0;          i++; i++;  
            ds = 1;    
            i = 8; while(i>0) i--;  
        }
        else  
        {
            ds = 0;
          i = 8; while(i>0) i--;  
            ds = 1;
           i++; i++;
        }
   }
}


void sendChangeCmd()
{
    dsInit();    
    dsWait();   
    delayMs(1);    
    writeByte(0xcc);
    writeByte(0x44);
}

void sendReadCmd()
{
    dsInit();
    dsWait();
    delayMs(1);
    writeByte(0xcc); 
    writeByte(0xbe); 
}


int getTmpValue()
{
    unsigned int tmpvalue;
    int value; 
    float t;
    unsigned char low, high;
    sendReadCmd();
    
    low = readByte(); 
    high = readByte();
   
    tmpvalue = high;
    tmpvalue <<= 8;
    tmpvalue |= low;
    value = tmpvalue;
    
  \
    t = value * 0.0625;
    \
    value = t * 100 + (value > 0 ? 0.5 : -0.5); //大于0加0.5, 小于0减0.5
    return value;
}

void display(int v) 
{
    unsigned char count;
    unsigned char datas[] = {0, 0, 0, 0, 0};
    unsigned int tmp = abs(v);
    datas[0] = tmp / 10000;
    datas[1] = tmp % 10000 / 1000;
    datas[2] = tmp % 1000 / 100;
    datas[3] = tmp % 100 / 10;
    datas[4] = tmp % 10;
    writeComm(0xc0+3);
    if(v < 0)
    {
        writeString("- ", 2);
   }
    else
    {
       writeString("+ ", 2);
    }
    if(datas[0] != 0)
    {
        writeData('0'+datas[0]);
    }
    for(count = 1; count != 5; count++)
    {
        writeData('0'+datas[count]);
        if(count == 2)
        {
            writeData('.');
        }
    }
}
/**//*****************************主函数*******************************/

void main()
{
    uchar table[] = "  xianzaiwendu: ";
    sendChangeCmd();
    init();
    writeComm(0x80);
    writeString(table, 16);
    while(1)
    {
        delayMs(1000); //温度转换时间需要750ms以上
        writeComm(0xc0);
        display(getTmpValue());
        sendChangeCmd();
    }
}
