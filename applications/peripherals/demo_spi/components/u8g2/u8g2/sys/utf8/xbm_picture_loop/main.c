
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

static unsigned char b[] = {
   0xff, 0xff, 0x07, 0x01, 0x00, 0x04, 0x01, 0x00, 0x04, 0x01, 0x08, 0x04,
   0xe1, 0x1e, 0x04, 0xb1, 0x13, 0x04, 0x11, 0x11, 0x04, 0x11, 0x79, 0x04,
   0xf1, 0x4f, 0x04, 0x99, 0xc4, 0x04, 0x89, 0x7f, 0x04, 0xc9, 0x38, 0x04,
   0xf1, 0x08, 0x04, 0x81, 0x08, 0x04, 0x81, 0x0f, 0x04, 0x01, 0xe2, 0x04,
   0x01, 0xda, 0x04, 0x01, 0xba, 0x04, 0x01, 0x9e, 0x04, 0x01, 0x4e, 0x04,
   0x01, 0x7e, 0x04, 0x01, 0x1e, 0x04, 0x01, 0x06, 0x04, 0x01, 0x06, 0x04,
   0x01, 0x06, 0x04, 0x01, 0x02, 0x04, 0x01, 0x00, 0x04, 0x01, 0x00, 0x04,
   0xff, 0xff, 0x07 };



int main(void)
{

  u8g2_SetupBuffer_Utf8(&u8g2, U8G2_R0);
  
  
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_6x13_tf);
  u8g2_SetFontDirection(&u8g2, 0);
  
  u8g2_FirstPage(&u8g2);
  do
  {      
    u8g2_DrawXBM(&u8g2, 1, 1, 19, 29, b);
    u8g2_DrawStr(&u8g2, 30, 20, "XBM");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

