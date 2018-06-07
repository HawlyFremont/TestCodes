/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：18b20温度显示（数码管）实验
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-7
*  晶振:11.0592MHZ
*  实验效果：接上18B20温度传感器数码管显示出当前温度
******************************************************************/
#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define dula P0		//段选信号的锁存器控制
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;
sbit DS=P2^2;           //define interface
uint temp;             // variable of temperature

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
  a=tmpread();//低八位
  b=tmpread();//高八位
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625; //算出来的是测到的温度，数值可到小数点后两位
  temp=tt*10+0.5; //为了显示温度后的小数点后一位并作出四舍五入，因为取值运算不能取小数点后的数
  return temp;
}

void display(uint temp)			//显示程序
{
   uchar bai,shi1,shi0,ge;
   bai=temp/100;//温度数值上为十位
   shi0=temp%100;//温度数值上为几点几
   shi1=shi0/10;//温度上为个位，并且显示时需要加小数点
   ge=shi0%10;//温度上为小数位，并已经四舍五入

   wei1=1;		//显示百位
   wei2=0;
   wei3=0;
   wei4=0;
   P0=table[bai];
   delay(2);

   wei1=0;		//显示十位
   wei2=1;
   wei3=0;
   wei4=0;
   P0=table1[shi1];
   delay(2);

   wei1=0;		//显示个位
   wei2=0;
   wei3=1;
   wei4=0;
   P0=table[ge];
   delay(2);
}


void main()
{
 uchar a;
  do
  {
    tmpchange();//让18b20开始转换温度

	for(a=100;a>0;a--)
  	{   
		display(tmp());
  	}
  }while(1);
}

