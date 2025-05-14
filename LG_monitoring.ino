#include <Arduino.h>
#include <U8g2lib.h> /* Projekt â†’ Pridat knihovnu â†’ spravovat knihovny â†’ vyhledat u8g2 */

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

char line1[16];
char line2[16];
char line3[16];
char line4[16];
const char * EMPTY_LINE = "               ";
const int INTERVAL = 21600;

const int button = 2;

const int bilge5 = 9;


int total_time = 0;
int act_time = 0;
int prev_time = 0;
String convert;
long int clock = 0;
int alarm = 0;


/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void update_buffer(int line, char * text){

  switch (line){
      case 0:
        strcpy(line1,text);
        break;
      case 1:
        strcpy(line2,text);
        break;
      case 2:
        strcpy(line3,text);
        break;
      case 3:
        strcpy(line4,text);
        break;
      
  }

}
void draw(void){
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,12,line1);
    u8g2.drawStr(0,30,line2);
    u8g2.drawStr(0,46,line3);
    u8g2.drawStr(0,64,line4);
  } while ( u8g2.nextPage() );
}

void clear(void){
  update_buffer(0,EMPTY_LINE);
  update_buffer(1,EMPTY_LINE);
  update_buffer(2,EMPTY_LINE);
  update_buffer(3,EMPTY_LINE);
  draw();
}

void setup(void) {

  pinMode(bilge5, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);

  clear();
  u8g2.begin();  
  update_buffer(0, "==  La Grace  == ");
  update_buffer(3," ©cr3w 2025 ");
  draw();
  delay(5000);
  clear();
}

void loop(void) {

int optime = millis();

if (digitalRead(bilge5) == 1)   //reverse logic
{
  if(alarm)
  {
      update_buffer(0, "Bilge   ALARM");

  }
  else
  {
      update_buffer(0, "Bilge   OFF");
  }
}
else
{
  update_buffer(0, "Bilge   ON");
  total_time = total_time+1;
  act_time +=1;
}

if (clock >= INTERVAL)  // po dosazeni hranice intervalu srovnej cisla, a restartuj periodu
{
  clock = 0;
  prev_time = act_time;
  act_time = 0;


  if((prev_time != 0)&&(act_time > (2*prev_time)))
  {
      alarm = 1;
  }
  else {
      alarm = 0;
  }

}

  int h,m,s;
  
  h=act_time;

  s = h % 60;
  h /= 60;
  m = h % 60;
  h /= 60;

  convert = "Act: "; 
  convert += String(h);
  convert += ":";
  if(m<10)
    convert += "0";
  convert += String(m);
  convert += ":";
  if(s<10)
    convert += "0";
  convert += String(s);
  
  char convert_array[24];
  convert.toCharArray(convert_array,24);
  update_buffer(1,convert_array);


  h=prev_time;

  s = h % 60;
  h /= 60;
  m = h % 60;
  h /= 60;

  convert = "Prev: ";
  convert += String(h);
  convert += ":";
  if(m<10)
    convert += "0";
  convert += String(m);
  convert += ":";
  if(s<10)
    convert += "0";
  convert += String(s);
  
 // char convert_array[24];
  convert.toCharArray(convert_array,24);
  update_buffer(2,convert_array);


  h=total_time;

  s = h % 60;
  h /= 60;
  m = h % 60;
  h /= 60;

  convert = "Total: ";
  convert += String(h);
  convert += ":";
  if(m<10)
    convert += "0";
  convert += String(m);
  convert += ":";
  if(s<10)
    convert += "0";
  convert += String(s);
  
 // char convert_array[24];
  convert.toCharArray(convert_array,24);
  update_buffer(3,convert_array);
  draw();

  clock +=1;
  delay(1000-(millis()-optime));
}




