/*****************************************************************************
* | Author      :   Waveshare team
* | Function    :   1.5inch OLED Module Drive function
# -----------------------------------------------------------------------------
# Adapted by Borys Kuzmenko
# -----------------------------------------------------------------------------
******************************************************************************/

#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H		

#include "daisy.h"


#if __has_include("../DEV_Config.h")
    #include "../DEV_Config.h"     // користувацький файл є - береж його
#else
    #include "DEV_Config_template.h"  // немає - береж шаблон
#endif

using namespace daisy;
extern SpiHandle spi_display;
extern GPIO pin_dc;
extern GPIO pin_reset;

struct DisplayTraits1327 {
    static constexpr uint8_t kPixelsPerRamColumn = 1;
    static constexpr uint8_t kPixelsPerRamRow    = 2;
    static constexpr uint8_t kColumnOffset       = 0;
    static constexpr uint8_t kBytesPerRamColumn  = 1;
    static constexpr uint8_t kPixelsPerByte      = 2;
    static constexpr bool kReversedNibbles      = false;
};

struct DisplayTraits1363 {
    static constexpr uint8_t kPixelsPerRamColumn = 4;
    static constexpr uint8_t kPixelsPerRamRow    = 1;
    static constexpr uint8_t kColumnOffset       = 8;
    static constexpr uint8_t kBytesPerRamColumn  = 2;
    static constexpr uint8_t kPixelsPerByte      = 2;
    static constexpr bool kReversedNibbles      = true;
};

#if DISPLAY_CONTROLLER == SSD1327
    #include "SSD1327.h"
    using DisplayTraits = DisplayTraits1327;
#elif DISPLAY_CONTROLLER == SSD1363
    #include "SSD1363.h"
    using DisplayTraits = DisplayTraits1363;
#else
    #error "Unknown DISPLAY_CONTROLLER"
#endif

struct ImageData
{
    UBYTE* data;
    UWORD size;
};

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Transmit(const UBYTE *Image);
void OLED_Transmit_Part(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
void OLED_Clear_DMA(ImageData *Imagedata);
void OLED_Transmit_DMA(ImageData* Imagedata);
void OLED_Transmit_DMA_Part(ImageData *Imagedata, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);


inline constexpr uint8_t RamColumn(uint16_t x)
{
    return static_cast<uint8_t>(x / DisplayTraits::kPixelsPerRamColumn
                                + DisplayTraits::kColumnOffset);
}

inline constexpr uint8_t RamRow(uint16_t y)
{
    return static_cast<uint8_t>(y / DisplayTraits::kPixelsPerRamRow);
}

inline constexpr uint16_t BytesPerRow()
{
    return (DISPLAY_WIDTH + DisplayTraits::kPixelsPerByte - 1)
           / DisplayTraits::kPixelsPerByte;
}

inline constexpr uint16_t BufferByteOffset(uint16_t x, uint16_t y)
{
    return y * BytesPerRow() + x / DisplayTraits::kPixelsPerByte;
}

inline constexpr uint16_t BytesPerDisplay()
{
    return DISPLAY_WIDTH * DISPLAY_HEIGHT / DisplayTraits::kPixelsPerByte;
}

inline constexpr uint16_t BytesPerRowPart(uint16_t x_start, uint16_t x_end)
{
    const uint16_t span = x_end - x_start;
    return (span + DisplayTraits::kPixelsPerByte - 1) / DisplayTraits::kPixelsPerByte;
}

inline constexpr uint16_t BytesForPart(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end)
{
    return BytesPerRowPart(x_start, x_end) * (y_end - y_start);
}

inline constexpr uint16_t BytesForWidth(uint16_t width)
{
    return (width + DisplayTraits::kPixelsPerByte - 1)
           / DisplayTraits::kPixelsPerByte;
}

#endif  