#ifndef __SSD1327_H
#define __SSD1327_H

#include "daisy.h"

#define DISPLAY_WIDTH  128//OLED width
#define DISPLAY_HEIGHT 128 //OLED height

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

void OLED_WriteReg(UBYTE Reg);
void OLED_WriteData(UBYTE Data);
void OLED_Clear(void);
void Delay_ms(unsigned long xms);

/*******************************************************************************
function:
        Common register initialization
*******************************************************************************/
inline void OLED_InitReg(void)
{
    OLED_WriteReg(0xae);//--turn off oled panel

    OLED_WriteReg(0x15);    //   set column address
    OLED_WriteReg(0x00);    //  start column   0
    OLED_WriteReg(0x7f);    //  end column   127

    OLED_WriteReg(0x75);    //   set row address
    OLED_WriteReg(0x00);    //  start row   0
    OLED_WriteReg(0x7f);    //  end row   127

    OLED_WriteReg(0x81);  // set contrast control
    OLED_WriteReg(0x80);

    OLED_WriteReg(0xa0);    // gment remap
    OLED_WriteReg(0x51);   //51

    OLED_WriteReg(0xa1);  // start line
    OLED_WriteReg(0x00);

    OLED_WriteReg(0xa2);  // display offset
    OLED_WriteReg(0x00);

    OLED_WriteReg(0xa4);    // rmal display
    OLED_WriteReg(0xa8);    // set multiplex ratio
    OLED_WriteReg(0x7f);

    OLED_WriteReg(0xb1);  // set phase leghth
    OLED_WriteReg(0xf1);

    OLED_WriteReg(0xb3);  // set dclk
    OLED_WriteReg(0x00);  //80Hz:0xc1 90Hz:0xe1   100Hz:0x00   110Hz:0x30 120Hz:0x50   130Hz:0x70     01

    OLED_WriteReg(0xab);  //
    OLED_WriteReg(0x01);  //

    OLED_WriteReg(0xb6);  // set phase leghth
    OLED_WriteReg(0x0f);

    OLED_WriteReg(0xbe);
    OLED_WriteReg(0x0f);

    OLED_WriteReg(0xbc);
    OLED_WriteReg(0x08);

    OLED_WriteReg(0xd5);
    OLED_WriteReg(0x62);

    OLED_WriteReg(0xfd);
    OLED_WriteReg(0x12);

    Delay_ms(1);
    OLED_WriteReg(0xaf);
}

// inline void OLED_SetROW(UBYTE Ystart, UBYTE Yend)
// {
//     OLED_WriteReg(0x15);
//     OLED_WriteReg(Ystart/2);
//     OLED_WriteReg(Yend/2 - 1);
// }

// inline void OLED_SetColumn(UBYTE Xstart, UBYTE Xend)
// {
//     OLED_WriteReg(0x75);
//     OLED_WriteReg(Xstart);
//     OLED_WriteReg(Xend - 1);
// }


#endif