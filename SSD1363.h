#ifndef __SSD1363_H
#define __SSD1363_H

#include "daisy.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define REG_SET_ROW 0x75
#define REG_SET_COLUMN 0x15
#define REG_WRITE_DATA_ENABLE 0x5C
#define REG_READ_DATA_ENABLE 0x5D
#define REG_REMAP 0xA0
#define REG_SCROLL_START_LINE 0xA1
#define REG_SCROLL_OFFSET 0xA2
#define REG_ALL_PIXELS_OFF 0xA4
#define REG_ALL_PIXELS_ON 0xA5
#define REG_DISPLAY_NORMAL 0xA6
#define REG_DISPLAY_INVERTED 0xA7
#define REG_DISPLAY_OFF 0xAE
#define REG_DISPLAY_ON 0xAF
#define REG_INTERNAL_IREF 0xAD
#define REG_PHASE_LENGTH 0xB1
#define REG_SET_CLOCK 0xB3
#define REG_SECOND_PRECHARGE_PERIOD 0xB6
#define REG_SET_GRAY_SCALE_TABLE_CURVE 0xB8
#define REG_SET_GRAY_SCALE_INDEX 0xB9
#define REG_PRECHARGE_VOLTAGE_CONFIG 0xBA
#define REG_PRECHARGE_VOLTAGE 0xBB
#define REG_VCOMH 0xBE
#define REG_SET_MUX_RATIO 0xCA
#define REG_COMMAND_LOCK 0xFD
#define REG_CONTRAST_CURRENT 0xC1
#define REG_MASTER_CONTRAST_CURRENT 0xC7

#define DISPLAY_WIDTH  256 
#define DISPLAY_HEIGHT 128

using namespace daisy;
extern SpiHandle spi_display;
extern GPIO pin_dc;
extern GPIO pin_reset;


void OLED_WriteReg(UBYTE Reg);
void OLED_WriteData(UBYTE Data);
void OLED_Clear(void);
void OLED_Reset(void);

inline void OLED_WriteDataEnable()
{
    OLED_WriteReg(REG_WRITE_DATA_ENABLE);
}

inline void OLED_ReadDataEnable()
{
    OLED_WriteReg(REG_READ_DATA_ENABLE);
}

inline void OLED_AllPixelsOff()
{
    OLED_WriteReg(REG_ALL_PIXELS_OFF);
}

inline void OLED_AllPixelsOn()
{
    OLED_WriteReg(REG_ALL_PIXELS_ON);
}

inline void OLED_SetDisplayNormal()
{
    OLED_WriteReg(REG_DISPLAY_NORMAL);
}

inline void OLED_SetDisplayInverted()
{
    OLED_WriteReg(REG_DISPLAY_INVERTED);
}

inline void OLED_DisplayOff() 
{
    OLED_WriteReg(REG_DISPLAY_OFF);
}

inline void OLED_DisplayOn()
{
    OLED_WriteReg(REG_DISPLAY_ON);
}

// U8g2
inline void OLED_InitReg(void)
{
    // Hardware reset
    Delay_ms(1);
    pin_reset.Write(false);
    Delay_ms(50);
    pin_reset.Write(true);
    Delay_ms(1);

    // 1. Unlock commands
    OLED_WriteReg(0xFD);  // REG_COMMAND_LOCK
    OLED_WriteData(0x12);
    
    // 2. Display OFF
    OLED_WriteReg(0xAE);  // REG_DISPLAY_OFF
    
    // 3. Set Clock (рано, як у u8g2!)
    OLED_WriteReg(0xB3);  // REG_SET_CLOCK
    OLED_WriteData(0x90);
    
    // 4. Set MUX Ratio (128 lines)
    OLED_WriteReg(0xCA);  // REG_SET_MUX_RATIO
    OLED_WriteData(0x7F); // 127+1 = 128 lines
    
    // 5. Display Offset (вертикальний зсув)
    OLED_WriteReg(0xA2);  // REG_SCROLL_OFFSET
    OLED_WriteData(0x20); // 32 рядки offset
    
    // 6. Display Start Line
    OLED_WriteReg(0xA1);  // REG_SCROLL_START_LINE
    OLED_WriteData(0x00);
    
    // 7. Set Re-map & Dual COM mode
    OLED_WriteReg(0xA0);  // REG_REMAP
    OLED_WriteData(0x32); // 0b00110010
    OLED_WriteData(0x00); // Dual COM mode disabled (для 256x128)
    
    // 8. Display Enhancement A
    OLED_WriteReg(0xB4);
    OLED_WriteData(0x32);
    OLED_WriteData(0x0C);
    
    // 9. Set Contrast Current
    OLED_WriteReg(0xC1);  // REG_CONTRAST_CURRENT
    OLED_WriteData(0xFF);
    
    // 10. Precharge voltage config
    OLED_WriteReg(0xBA);  // REG_PRECHARGE_VOLTAGE_CONFIG
    OLED_WriteData(0x03);
    
    // 11. Linear grayscale
    OLED_WriteReg(0xB9);  // REG_SET_GRAY_SCALE_INDEX
    
    // 12. Internal IREF
    OLED_WriteReg(0xAD);  // REG_INTERNAL_IREF
    OLED_WriteData(0x90); // Internal: 0x90, External: 0x80
    
    // 13. Phase Length
    OLED_WriteReg(0xB1);  // REG_PHASE_LENGTH
    OLED_WriteData(0x74); // Phase1 & Phase2
    
    // 14. Precharge Voltage
    OLED_WriteReg(0xBB);  // REG_PRECHARGE_VOLTAGE
    OLED_WriteData(0x0C);
    
    // 15. Second Precharge Period
    OLED_WriteReg(0xB6);  // REG_SECOND_PRECHARGE_PERIOD
    OLED_WriteData(0xC8);
    
    // 16. VCOMH
    OLED_WriteReg(0xBE);  // REG_VCOMH
    OLED_WriteData(0x04);
    
    // 17. Normal display (не inverted)
    OLED_WriteReg(0xA6);  // REG_DISPLAY_NORMAL
    
    Delay_ms(1);
    
    // 19. Display ON
    OLED_WriteReg(0xAF);  // REG_DISPLAY_ON
    Delay_ms(10);
}


// // Custom datasheet (not good)
// inline void OLED_InitReg(void)
// {
//     Delay_ms(1);
//     pin_reset.Write(false);
//     Delay_ms(50);
//     pin_reset.Write(true);
//     Delay_ms(1);

// 	// Unlock commands
//     OLED_WriteReg(REG_COMMAND_LOCK);
//     OLED_WriteData(0x12);  // 0x12 - command unlock

// 	// Display OFF
//     OLED_WriteReg(REG_DISPLAY_OFF);

//     // Set MUX Ratio (128 lines)
//     OLED_WriteReg(REG_SET_MUX_RATIO);
//     OLED_WriteData(0x7F); // 0x7F = 128 lines

//     // Set Re-map & Dual COM mode
//     OLED_WriteReg(REG_REMAP);
//     OLED_WriteData(0x32); // Horizontal + Alternative COM
//     OLED_WriteData(0x00); // Dual COM mode - обов'язково для 256x128!

//     OLED_WriteReg(REG_SCROLL_OFFSET); 
//     OLED_WriteData(0x20); // Offset:: 0~127	
//     OLED_WriteReg(REG_SCROLL_START_LINE);
//     OLED_WriteData(0x00);  // Set start line = 0
    
//     // Display Enhancement
//     OLED_WriteReg(0xB4);
//     OLED_WriteData(0x32);
//     OLED_WriteData(0x0C);

//     // Set Contrast Current
//     OLED_WriteReg(REG_CONTRAST_CURRENT);
//     OLED_WriteData(0xFF);

//     // Set Default Gamma
//     OLED_WriteReg(REG_SET_GRAY_SCALE_INDEX);

//     // Set Clock
//     OLED_WriteReg(REG_SET_CLOCK);
//     OLED_WriteData(0x30); 

//     OLED_WriteReg(REG_PRECHARGE_VOLTAGE_CONFIG);
//     OLED_WriteData(0x03);

// 	OLED_WriteReg(REG_INTERNAL_IREF);
//     OLED_WriteData(0x90); // Select:: Internal

// 	OLED_WriteReg(REG_PHASE_LENGTH);    
//     OLED_WriteData(0x74);  

//     OLED_WriteReg(REG_PRECHARGE_VOLTAGE);
//     OLED_WriteData(0x0C);

//     OLED_WriteReg(REG_SECOND_PRECHARGE_PERIOD);
//     OLED_WriteData(0xC8); // 0x00 - 0x0F

//     OLED_WriteReg(REG_VCOMH);
//     OLED_WriteData(0x04);

// 	// Set Display On
//     OLED_WriteReg(REG_DISPLAY_ON);
// }


// // AliExpress datasheet
// inline void OLED_InitReg(void)
// {
//     pin_reset.Write(true);
//     Delay_ms(1);
//     pin_reset.Write(false);
//     Delay_ms(50);
//     pin_reset.Write(true);
//     Delay_ms(1);

//     // Unlock commands
//     OLED_WriteReg(REG_COMMAND_LOCK);
//     OLED_WriteData(0x12);  

//     // Display OFF
//     OLED_WriteReg(REG_DISPLAY_OFF);

//     // Set Clock
//     OLED_WriteReg(REG_SET_CLOCK);
//     OLED_WriteData(0x90); 

//     // Set MUX Ratio (128 lines)
//     OLED_WriteReg(REG_SET_MUX_RATIO);
//     OLED_WriteData(0x7F); 

//     // Set Display Offset
//     OLED_SetScrollOffset(0x00);
//     OLED_SetScrollStartLine(0x00);

//     // Set Re-map & Dual COM mode
//     OLED_WriteReg(REG_REMAP);
//     OLED_WriteData(0x36); 
//     OLED_WriteData(0x01); 

//     // GPIO Configuration
//     OLED_WriteReg(0xB5);
//     OLED_WriteData(0x00);
    
//     // Internal VDD Regulator 
//     OLED_WriteReg(0xAB);
//     OLED_WriteData(0x01);

//     // Display Enhancement
//     OLED_WriteReg(0xB4);
//     OLED_WriteData(0xA0);
//     OLED_WriteData(0xDD);

//     // Set Contrast Current
//     OLED_WriteReg(REG_CONTRAST_CURRENT);
//     OLED_WriteData(0xCF);

//     // Master Contrast Current
//     OLED_WriteReg(REG_MASTER_CONTRAST_CURRENT);
//     OLED_WriteData(0x0F);

//     // Set Gray Scale Default
//     OLED_WriteReg(REG_SET_GRAY_SCALE_INDEX);

//     OLED_WriteReg(REG_PHASE_LENGTH);    
//     OLED_WriteData(0xE2);  

//     // Enhance Driving Scheme Capability
//     OLED_WriteReg(0xD1);
//     OLED_WriteData(0x82);
//     OLED_WriteData(0x20);

//     // Set Column
//     OLED_WriteReg(REG_SET_COLUMN);   
//     OLED_WriteReg(0x1C);    
//     OLED_WriteReg(0x5B);    

//     // Set Row
//     OLED_WriteReg(REG_SET_ROW);   
//     OLED_WriteReg(0x00);    
//     OLED_WriteReg(0x7F);  

//     // Precharge Voltage
//     OLED_WriteReg(REG_PRECHARGE_VOLTAGE);
//     OLED_WriteData(0x1F); 

//     // Second Precharge Period
//     OLED_WriteReg(REG_SECOND_PRECHARGE_PERIOD);
//     OLED_WriteData(0x08); 

//     // VCOMH
//     OLED_WriteReg(REG_VCOMH);
//     OLED_WriteData(0x07);

//     OLED_WriteReg(REG_DISPLAY_NORMAL);

//     Delay_ms(10);

//     OLED_WriteReg(REG_DISPLAY_ON); 

//     Delay_ms(100);

// }


// // ChatGPT
// inline void OLED_InitReg(void)
// {
//     OLED_Reset();
//     // Режим команд
//     OLED_WriteReg(0xFD);  // Command Lock
//     OLED_WriteData(0x12);  // Unlock

//     OLED_WriteReg(0xAE);  // Display OFF

//     // Настройки
//     OLED_WriteReg(0xA1); OLED_WriteData(0x00);  // Set start line = 0
//     OLED_WriteReg(0xA2); OLED_WriteData(0x20);  // Display offset

//     // Remap / Dual COM, сегмент-ремап:
//     OLED_WriteReg(0xA0);
//     OLED_WriteData(0x36);  // варіант remap — можливо треба змінити
//     OLED_WriteData(0x01);

//     // Multiplex ratio (MUX)
//     OLED_WriteReg(0xCA);
//     OLED_WriteData(0x7F);  // 127 → 128 рядків

//     // Display Clock
//     OLED_WriteReg(0xB3);
//     OLED_WriteData(0xF1);  // або інше значення

//     // Phase Length / Pre-charge / others
//     OLED_WriteReg(0xB1);
//     OLED_WriteData(0x32);

//      // Precharge 2nd
//      OLED_WriteReg(0xB6);
//      OLED_WriteData(0x08);

//     // OLED_WriteReg(0xB4);
//     // OLED_WriteData(0xA0);
//     // OLED_WriteData(0xB5);
//     // OLED_WriteData(0x55);

//     // Contrast
//     OLED_WriteReg(0xC1);
//     OLED_WriteData(0xFF);

//     OLED_WriteReg(0xC7);
//     OLED_WriteData(0x0F);

//     // // Enable internal regulator / current reference
//     // OLED_WriteReg(0xAB);
//     // OLED_WriteData(0x01);

//     OLED_WriteReg(0xAF);  // Display ON
// }

#endif