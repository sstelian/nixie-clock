//Nixie clock with RTC
//Written by Stelian Saracut for Arduino 1.0.4
#include <Wire.h>
#include "RTClib.h"
#include <Time.h> 
 
 
 
RTC_DS1307 rtc;
 
int k;
int del = 200; //wait between digits
int del2 = 400; //wait
boolean cs=HIGH,ps=HIGH,disp=LOW;
 
void setup() {
  //initializing
  DDRD = B1111; //port D outputs to cathode decoder
  DDRB = B1110; //port B outputs to anode decoder
  PORTC = B110111; //enable pullups for I2C (RTC) and buttons on port C
  Wire.begin();
  rtc.begin();
  if (! rtc.isrunning())
    adjTime(); //manually set time
    else
    getRTCtime(); //get time from RTC
  startup(400); //just because it looks nice
   
}
 
 
void loop() {
  if ((hour() == 0)&&(second()==0)) getRTCtime(); //update time once a day
   cs = debounce(ps,A0);
   if ((cs != ps) && (cs == LOW)) 
      adjTime(); //manually adjust time
   ps = cs;
  if (second() % 20 == 0) disp = HIGH;
   if (disp == HIGH) {
     //time shifts out and date shifts in
      if (k<=100) nixDisp(hour() % 10,minute()/10,minute() % 10,second()/10,second() % 10,11);
      if ((k>100) && (k<=200)) nixDisp(minute()/10,minute() % 10,second()/10,second() % 10,11,11); 
      if ((k>200) && (k<=300)) nixDisp(minute() % 10,second()/10,second() % 10,11,11,11); 
      if ((k>300) && (k<=400)) nixDisp(second()/10,second() % 10,11,11,11,11);    
      if ((k>400) && (k<=500)) nixDisp(second() % 10,11,11,11,11,11);     
      if ((k>500) && (k<=600)) nixDisp(11,11,11,11,11,11); //blank display   
      if ((k>600) && (k<=700)) nixDisp(11,11,11,11,11,day()/10);     
      if ((k>700) && (k<=800)) nixDisp(11,11,11,11,day()/10,day() % 10); 
      if ((k>800) && (k<=900)) nixDisp(11,11,11,day()/10,day() % 10,month()/10); 
      if ((k>900) && (k<=1000)) nixDisp(11,11,day()/10,day() % 10,month()/10,month() % 10); 
      if ((k>1000) && (k<=1100)) nixDisp(11,day()/10,day() % 10,month()/10,month() % 10,year() % 100 /10); 
      if ((k>1100) && (k<=1500)) nixDisp(day()/10,day() % 10,month()/10,month() % 10,year() % 100 /10,year() % 10); 
      if (k>1500) {
       disp = LOW; //back to normal operation
       k = 0;
     } 
    k++; 
   }
   else
     
      
       
   nixDisp(hour()/10,hour() % 10,minute()/10,minute() % 10,second()/10,second() % 10); //display time under normal operation
        
}
 
void getRTCtime() {
  DateTime now = rtc.now();
  setTime(now.hour(),now.minute(),now.second(),now.day(),now.month(),now.year());
   
} 
 
//function for adjusting time manually and button debouncing
void adjTime() {
  int H=0,M=0,D=1,Mo=1,Y=13;
  boolean cs1=HIGH,cs2=HIGH,ps1=HIGH,ps2=HIGH,cs=HIGH,ps=LOW;
   
  
  while(true) {
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=H/10;
  PORTB = B1010;
  delay(10);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=H%10;
  PORTB = B1000;
   
   
   cs = debounce(ps,A0);
   if ((cs != ps) && (cs == LOW))
      break;
   ps = cs;
    
   cs1 = debounce(ps1,A1); 
   cs2 = debounce(ps2,A2); 
   if ((cs1 != ps1)&&(cs1 == LOW))
         H--;
   if (H <0) H=23;    if ((cs2 != ps2)&&(cs2 == LOW))          H++;    if (H >23) H=0;
   ps1 = cs1;
   ps2 = cs2;
   
   delay(10);
  }
    
  
   
  cs1=HIGH; cs2=HIGH; ps1=HIGH; ps2=HIGH; cs=HIGH; ps=LOW;
  
  while(true) {
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=M/10;
  PORTB = B110;
  delay(10);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=M%10;
  PORTB = B100;
 
   cs = debounce(ps,A0);
   if ((cs != ps) && (cs == LOW))
      break;
   ps = cs;
    
   cs1 = debounce(ps1,A1); 
   cs2 = debounce(ps2,A2); 
   if ((cs1 != ps1)&&(cs1 == LOW))
         M--;
   if (M < 0) M = 59;    if ((cs2 != ps2)&&(cs2 == LOW))          M++;    if (M > 59) M = 0;
   ps1 = cs1;
   ps2 = cs2;
   delay(10);
  }
   
 
   
  cs1=HIGH; cs2=HIGH; ps1=HIGH; ps2=HIGH; cs=HIGH; ps=LOW;
   
  while(true) {
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=D/10;
  PORTB = B1010;
  delay(10);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=D%10;
  PORTB = B1000;
 
   cs = debounce(ps,A0);
   if ((cs != ps) && (cs == LOW))
      break;
   ps = cs;
    
   cs1 = debounce(ps1,A1); 
   cs2 = debounce(ps2,A2); 
   if ((cs1 != ps1)&&(cs1 == LOW))
         D--;
   if (D < 0) D = 31;    if ((cs2 != ps2)&&(cs2 == LOW))          D++;    if (D > 31) D = 0;
   ps1 = cs1;
   ps2 = cs2;
   delay(10);
  }
   
   
   
  cs1=HIGH; cs2=HIGH; ps1=HIGH; ps2=HIGH; cs=HIGH; ps=LOW;
   
  while(true) {
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=Mo/10;
  PORTB = B110;
  delay(10);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=Mo%10;
  PORTB = B100;
    
   cs = debounce(ps,A0);
   if ((cs != ps) && (cs == LOW))
      break;
   ps = cs;
    
   cs1 = debounce(ps1,A1); 
   cs2 = debounce(ps2,A2); 
   if ((cs1 != ps1)&&(cs1 == LOW))
         Mo--;
   if (Mo < 1) Mo = 12;    if ((cs2 != ps2)&&(cs2 == LOW))          Mo++;    if (Mo > 12) Mo = 1;
   ps1 = cs1;
   ps2 = cs2;
   delay(10);
  }
 
 
   
  cs1=HIGH; cs2=HIGH; ps1=HIGH; ps2=HIGH; cs=HIGH; ps=LOW;
   
  while(true) {
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=Y/10;
  PORTB = B10;
  delay(10);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=Y%10;
  PORTB = B0;
    
   cs = debounce(ps,A0);
   if ((cs != ps) && (cs == LOW))
      break;
   ps = cs;
    
   cs1 = debounce(ps1,A1); 
   cs2 = debounce(ps2,A2); 
   if ((cs1 != ps1)&&(cs1 == LOW))
         Y--;
   if (Y < 0) Y = 99;    if ((cs2 != ps2)&&(cs2 == LOW))          Y++;    ps1 = cs1;    ps2 = cs2;    delay(10);   }   rtc.adjust(DateTime(Y,Mo,D,H,M,0));   setTime(H,M,1,D,Mo,Y);             }  //button debouncing boolean debounce(boolean last, int pin) {  boolean current = digitalRead(pin);  if (last != current) {    delay(5);    current = digitalRead(pin);  }  return current; } void nixDisp(int a, int b, int c, int d, int e, int f) {   PORTB = B1110;    delayMicroseconds(del2);    PORTD=f;     if (f > 9) PORTB = B1110;
    else
    PORTB = B0; 
  delayMicroseconds(del); 
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=e;
  if (e > 9) PORTB = B1110;
    else
    PORTB = B10;
  delayMicroseconds(del);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=d;
  if (d > 9) PORTB = B1110;
    else
    PORTB = B100;
  delayMicroseconds(del);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=c;
  if (c > 9) PORTB = B1110;
    else
    PORTB = B110;
  delayMicroseconds(del);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD=b;
  if (b > 9) PORTB = B1110;
    else
    PORTB = B1000;
  delayMicroseconds(del);
  PORTB = B1110;
  delayMicroseconds(del2);
  PORTD =a;
  if (a > 9) PORTB = B1110;
    else
    PORTB = B1010;
  delayMicroseconds(del);
}
 
 
//some sort of counter that looks good
void startup(int t) {
  int a=0,b=1,c=2,d=3,e=4,f=5,i,j;
  for (j=1; j<=t; j++) {
    i++;
    if (i % 10 == 0) {
      a++; b++; c++;
      d++; e++; f++;
     }
    if (a == 10) a=0;
    if (b == 10) b=0;
    if (c == 10) c=0;
    if (d == 10) d=0;
    if (e == 10) e=0;
    if (f == 10) f=0;
   
    nixDisp(a,b,c,d,e,f); 
  } 
}
