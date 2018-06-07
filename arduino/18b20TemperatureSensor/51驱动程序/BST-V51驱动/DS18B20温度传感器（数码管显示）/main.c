
/**********************BST-V51实验开发板例程************************
*  平台：BST-V51 + Keil U3 + STC89C52
*  名称：18B20温度传感器模块测试
*  公司：深圳市亚博软件开发有限公司
*  淘宝：bstmcu.taobao.com       
*  日期：2013-11
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析.
******************************************************************/
//接上18B20温度传感器（另购）数码管显示出当前温度
#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit DS=P2^2;           //define interface
uint temp;             // variable of temperature
uchar flag1;            // sign of the result positive or negative
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};

void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}


void dsreset(void)       //send reset and initialization command
{
  uint i;
  DS=0;		              
  i=103;				   //将总线拉低480us~960us
  while(i>0)i--;
  DS=1;					   //然后拉高总线，若DS18B20做出反应会将在15us~60us后将总线拉低
  i=4;					   //15us~60us等待
  while(i>0)i--;
  //while(DS);
}

bit tmpreadbit(void)       //read a bit
{
   uint i;
   bit dat;
   DS=0;i++;          //i++ for delay
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}

uchar tmpread(void)   //read a byte date
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //读出的数据最低位在最前面，这样刚好一个字节在DAT里
  }
  return(dat);
}

void tmpwritebyte(uchar dat)   //write a byte to ds18b20
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     //write 1
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS=0;       //write 0
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }

  }
}

void tmpchange(void)  //DS18B20 begin change
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);  // address all drivers on bus
  tmpwritebyte(0x44);  //  initiates a single temperature conversion
  //delay(100);
}

uint tmp()               //get the temperature
{
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread();
  b=tmpread();
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;
  return temp;
}

void readrom()          //read the serial
{
  uchar sn1,sn2;
  dsreset();
  delay(1);
  tmpwritebyte(0x33);
  sn1=tmpread();
  sn2=tmpread();
}


void delay10ms()            //delay
  {
    uchar a,b;
    for(a=10;a>0;a--)
      for(b=60;b>0;b--);
   }

void display(uint temp)			//显示程序
{
   uchar A1,A2,A2t,A3,ser;
   ser=temp/10;
   SBUF=ser;
   A1=temp/100;
   A2t=temp%100;
   A2=A2t/10;
   A3=A2t%10;
   dula=0;
   P0=table[A1];		//显示百位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(1);

   dula=0;
   P0=table1[A2];		//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(1);

   P0=table[A3];		//显示个位
   dula=1;
   dula=0;

   P0=0xfb;
   wela=1;
   wela=0;
   delay(1);
}


void main()
{
 uchar a;
  do
  {
    tmpchange();
    //delay(200);

for(a=100;a>0;a--)
  {   display(tmp());
  }
  }                while(1);
}

