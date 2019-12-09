//使用Arduino UNO的连线方式
#define DIN 11
#define CS  10
#define CLK 13

//使用ESP8266的连线方式
//#define DIN 13 //D7
//#define CS  15 //D8
//#define CLK 14 //D5

#define uchar unsigned char
#include "LedControl.h"

LedControl lc=LedControl(DIN,CLK,CS,4); //4个MAX7219数码管模块级联
unsigned long delaytime=500;
uchar led_rev[10] = {0x7e, 0x06, 0x6d, 0x4f, 0x17, 0x5b, 0x7b, 0x0e, 0x7f, 0x5f}; //反向共阴七段数码管码表
uchar C_rev = 0x78; //反向字母码表
uchar P_rev = 0x3D;
uchar U_rev = 0x76;
uchar G_rev = 0x7B;
uchar S_rev = 0x5B;
uchar D_rev = 0x7E;
uchar F_rev = 0x39;
uchar A_rev = 0x3F;
uchar N_rev = 0x3E;
uchar Cel_rev = 0xF8; //摄氏度符号

void setRevNum(int addr, int digit, char num)
{ //用于反向显示数字(第几个数码管模块, 第几位, 显示什么数字)
  if(num!=' ')
    lc.setRow(addr, digit, led_rev[num-48]);
  else
    lc.setRow(addr, digit, 0x00);
}

void setup() {
  Serial.begin(9600);
  while(!Serial)
    continue;
  Serial.println("Serial Port Ready");
  int devices=lc.getDeviceCount();
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* 数码管亮度调至最低，防止功率过高开发板过热，亮度过高会有安全隐患 */
    /* 注意：实测亮度调至15，数码管全亮时，滤波电容温度会高于100度 */
    lc.setIntensity(address,1);
    /* and clear the display */
    lc.clearDisplay(address);
  }
  while(Serial.available()==0)
  {
    ledConnectionTest();
  }
}

char frame[40] = {}; //用于存储AIDA64从串口发来的信息
byte inByte;
int num1,num2,num3,num4;
char temp[3];

void refreshled()
{
  char a0,a1,a2,a3,b0,b1,b2,b3,c0,c1,c2,c3,d0,d1,d2,d3;
  a3 = num1/1000+48;
  a2 = (num1%1000)/100+48;
  a1 = ((num1%1000)%100)/10+48;
  a0 = (((num1%1000)%100)%10)+48;
  if(num1<1000) a3=' ';
  if(num1<100)  a2=' ';
  if(num1<10)   a1=' ';

//注释掉的内容用于正向使用数码管
/*lc.setChar(0,0,a0,false);
  lc.setChar(0,1,a1,false);
  lc.setChar(0,2,a2,false);
  lc.setChar(0,3,a3,false);
  lc.setRow(0,7,B01001110);
  lc.setRow(0,6,B01100111);
  lc.setRow(0,5,B00111110);*/
  setRevNum(0,6,a0);
  setRevNum(0,5,a1);
  setRevNum(0,4,a2);
  lc.setRow(0,0,C_rev);
  lc.setRow(0,1,P_rev);
  lc.setRow(0,2,U_rev);
  lc.setRow(0,7,Cel_rev);

  b3 = num2/1000+48;
  b2 = (num2%1000)/100+48;
  b1 = ((num2%1000)%100)/10+48;
  b0 = (((num2%1000)%100)%10)+48;
  if(num2<1000) b3=' ';
  if(num2<100)  b2=' ';
  if(num2<10)   b1=' ';
/*lc.setChar(1,0,b0,false);
  lc.setChar(1,1,b1,false);
  lc.setChar(1,2,b2,false);
  lc.setChar(1,3,b3,false);
  lc.setRow(1,7,B01011011);
  lc.setRow(1,6,B01011011);
  lc.setRow(1,5,B01111110);*/
  setRevNum(1,6,b0);
  setRevNum(1,5,b1);
  setRevNum(1,4,b2);
  lc.setRow(1,0,S_rev);
  lc.setRow(1,1,S_rev);
  lc.setRow(1,2,D_rev);
  lc.setRow(1,7,Cel_rev);
  
  c3 = num3/1000+48;
  c2 = (num3%1000)/100+48;
  c1 = ((num3%1000)%100)/10+48;
  c0 = (((num3%1000)%100)%10)+48;
  if(num3<1000) c3=' ';
  if(num3<100)  c2=' ';
  if(num3<10)   c1=' ';
/*lc.setChar(2,0,c0,false);
  lc.setChar(2,1,c1,false);
  lc.setChar(2,2,c2,false);
  lc.setChar(2,3,c3,false);
  lc.setRow(2,7,B01011111);
  lc.setRow(2,6,B01100111);
  lc.setRow(2,5,B00111110);*/
  setRevNum(2,6,c0);
  setRevNum(2,5,c1);
  setRevNum(2,4,c2);
  lc.setRow(2,0,G_rev);
  lc.setRow(2,1,P_rev);
  lc.setRow(2,2,U_rev);
  lc.setRow(2,7,Cel_rev);

  d3 = num4/1000+48;
  d2 = (num4%1000)/100+48;
  d1 = ((num4%1000)%100)/10+48;
  d0 = (((num4%1000)%100)%10)+48;
  if(num4<1000) d3=' ';
  if(num4<100)  d2=' ';
  if(num4<10)   d1=' ';
  lc.setChar(3,0,d0,false);
  lc.setChar(3,1,d1,false);
  lc.setChar(3,2,d2,false);
  lc.setChar(3,3,d3,false);
  lc.setRow(3,7,B01000111);
  lc.setRow(3,6,B01110111);
  lc.setRow(3,5,B01110110);
/*  setRevNum(3,7,d0);
  setRevNum(3,6,d1);
  setRevNum(3,5,d2);
  setRevNum(3,4,d3);
  lc.setRow(3,0,F_rev);
  lc.setRow(3,1,A_rev);
  lc.setRow(3,2,N_rev);*/
}

void ledConnectionTest()
{
  for(int i = 0; i < 4; i++){
    for(int j = 7; j >= 0; j--){
      lc.setRow(i, j, 0x01);
      delay(50);
      lc.setRow(i, j, 0x00);
    }  
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //注意：本段代码要求AIDA64设置必须和视频中完全相同才能解析 1.第一个标签以C开头，其后不能再出现字母C  2.标签只能为3个字母  3.取消勾选所有的单位显示
  //没办法，AIDA64发数据的格式我们也改不了...你们有没有更好的办法取代我这个沙雕方法...
  if(Serial.available()>0)
  {
      inByte = Serial.read();
      if(inByte==0x43)  //判断是不是字母C，找到数据帧起点
      { 
        frame[0] = inByte;
        int count = 1;
        for(int i = 1; i < 40; i++)
        {
          while(Serial.available()==0);
          frame[i] = Serial.read();
          count++;  
        }
        Serial.println(" ");
        //根据帧格式提取出四个数值
        num1 = 0;
        for(int i =4; i<8; i++)
        {
          if(frame[i]==0x20) break;
          num1 = num1*10+(frame[i]-48);
        }
        num2 = 0;
        for(int i =14; i<18; i++)
        {
          if(frame[i]==0x20) break;
          num2 = num2*10+(frame[i]-48);
        }
        num3 = 0;
        for(int i =24; i<28; i++)
        {
          if(frame[i]==0x20) break;
          num3 = num3*10+(frame[i]-48);
        }
        num4 = 0;
        for(int i =34; i<38; i++)
        {
          if(frame[i]==0x20) break;
          num4 = num4*10+(frame[i]-48);
        }
        Serial.println(num1);
        Serial.println(num2);
        Serial.println(num3);
        Serial.println(num4);
        refreshled();
      }
  }
}
