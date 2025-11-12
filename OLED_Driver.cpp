/*****************************************************************************
* | File      	:   OLED_Driver.cpp
* | Author      :   Waveshare team
* | Function    :   1.5inch OLED Module Drive function
* | Info        :

******************************************************************************/
#include "OLED_Driver.h"
#include "stdio.h"
#include "core_cm7.h"
#include <system.h>
#include "GUI_Paint.h"

#if DISPLAY_CONTROLLER == SSD1327
#include "SSD1327.h"
#elif DISPLAY_CONTROLLER == SSD1363
#include "SSD1363.h"
#endif

using namespace daisy;

SpiHandle spi_display;
GPIO pin_dc;
GPIO pin_reset;

static volatile bool dmaReady = true;
static UWORD cachedX1=0x00, cachedY1=0x00, cachedX2=0x00, cachedY2=0x00;

void SPI_Config()
{
    SpiHandle::Config spi_config;
    // SPI peripheral config
    spi_config.periph = SpiHandle::Config::Peripheral::SPI_1;
    spi_config.mode   = SpiHandle::Config::Mode::MASTER;
    spi_config.direction
        = SpiHandle::Config::Direction::TWO_LINES_TX_ONLY;
    spi_config.datasize       = 8;
    spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
    spi_config.clock_phase    = SpiHandle::Config::ClockPhase::ONE_EDGE;
    spi_config.nss            = SpiHandle::Config::NSS::HARD_OUTPUT;
    spi_config.baud_prescaler = SpiHandle::Config::BaudPrescaler::PS_2;
    // SPI pin config
    spi_config.pin_config.sclk = Pin(SPI_CLK_PIN);
    spi_config.pin_config.mosi = Pin(SPI_MOSI_PIN);
    spi_config.pin_config.nss  = Pin(SPI_NSS_PIN);
    spi_display.Init(spi_config);
}

void SPI_Init()
{
    SPI_Config();
    // SSD1327 control pin config
    pin_dc.Init(Pin(SPI_DC_PIN), GPIO::Mode::OUTPUT);
    pin_reset.Init(Pin(SPI_RESET_PIN), GPIO::Mode::OUTPUT);
}

void Delay_ms(unsigned long xms)
{
  System::Delay(xms);
}

/*******************************************************************************
function:
            Hardware reset
*******************************************************************************/
void OLED_Reset(void)
{
    pin_reset.Write(true);
    Delay_ms(50);
    pin_reset.Write(false);
    Delay_ms(50);
    pin_reset.Write(true);
    Delay_ms(10);
}

/*******************************************************************************
function:
            Write register address and data
*******************************************************************************/

void OLED_WriteReg(UBYTE Reg)
{
    pin_dc.Write(false);
    spi_display.BlockingTransmit(&Reg, sizeof(Reg));
}

void OLED_WriteData(UBYTE Data)
{
    pin_dc.Write(true);
    spi_display.BlockingTransmit(&Data, sizeof(Data));
}

/********************************************************************************
Initialization
********************************************************************************/
void OLED_Init(void)
{
    SPI_Init();
    // Hardware reset
    OLED_Reset();

    OLED_InitReg();
    Delay_ms(100);
}

inline void OLED_SetColumn(UWORD Xstart, UWORD Xend) 
{
    if (cachedX1 == Xstart && cachedX2 == Xend) return;

    OLED_WriteReg(0x15);  // REG_SET_COLUMN
    if (DisplayTraits::kSendCoordinatsAsAddress) {
        OLED_WriteReg(static_cast<UBYTE>(RamColumn(Xstart)));   
        OLED_WriteReg(static_cast<UBYTE>(RamColumn(Xend - 1))); 
    } else {
        OLED_WriteData(static_cast<UBYTE>(RamColumn(Xstart)));   
        OLED_WriteData(static_cast<UBYTE>(RamColumn(Xend - 1))); 
    }
    cachedX1 = Xstart;
    cachedX2 = Xend;
}

inline void OLED_SetROW(UWORD Ystart, UWORD Yend)
{
    if (cachedY1 == Ystart && cachedY2 == Yend) return;
    
    OLED_WriteReg(0x75);  // REG_SET_ROW
    if (DisplayTraits::kSendCoordinatsAsAddress) {
        OLED_WriteReg(static_cast<UBYTE>(RamRow(Ystart)));
        OLED_WriteReg(static_cast<UBYTE>(RamRow(Yend - 1)));
    } else {
        OLED_WriteData(static_cast<UBYTE>(RamRow(Ystart)));
        OLED_WriteData(static_cast<UBYTE>(RamRow(Yend - 1)));
    }

    cachedY1 = Ystart;
    cachedY2 = Yend;
}

/********************************************************************************
function:   Set the display Window(Xstart, Ystart, Xend, Yend)
********************************************************************************/
static void OLED_SetWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    if((Xstart > DISPLAY_WIDTH) || (Ystart > DISPLAY_HEIGHT) ||
       (Xend > DISPLAY_WIDTH) || (Yend > DISPLAY_HEIGHT))
        return;

    OLED_SetColumn(Xstart, Xend);
    OLED_SetROW(Ystart, Yend);
    OLED_WriteReg(0x5C);  // REG_WRITE_DATA_ENABLE
}

/********************************************************************************
Blocking transmit functions
********************************************************************************/

void OLED_Clear(void)
{
    UWORD i;
    OLED_SetWindow(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    for(i=0; i<BytesPerDisplay(); i++){
        OLED_WriteData(0x00);
    }
}

void OLED_Transmit(const UBYTE *Image)
{
    UWORD i, j, temp;
    OLED_SetWindow(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    for(i=0; i<DISPLAY_HEIGHT; i++)
        for(j=0; j<BytesPerRow(); j++)
        {
            temp = Image[j + i*BytesPerRow()]; 
            OLED_WriteData(temp);
        }
}

void OLED_Transmit_Part(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    UWORD i, j, temp;
    OLED_SetWindow(Xstart, Ystart, Xend, Yend);
    for(i=0; i<Yend-Ystart; i++)
        for(j=0; j<BytesPerRowPart(Xstart, Xend); j++)
        {
            temp = Image[j + i*BytesPerRowPart(Xstart, Xend)];
            OLED_WriteData(temp);
        }
}

/********************************************************************************
DMA callback functions
********************************************************************************/
void OLED_Dma_Ready(void*, daisy::SpiHandle::Result result)
{
    dmaReady = (result == daisy::SpiHandle::Result::OK);
}

void OLED_Clear_DMA(ImageData *Imagedata)
{
    memset(Imagedata->data, 0x00, Imagedata->size);
    while (!dmaReady) {
        System::Delay(1); 
    }
    dmaReady = false;
    OLED_SetWindow(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    pin_dc.Write(true);
    
    SCB_CleanDCache_by_Addr((uint32_t*)Imagedata->data, Imagedata->size);
    spi_display.DmaTransmit((UBYTE*)Imagedata->data, Imagedata->size, NULL, OLED_Dma_Ready, NULL);
    
}

void OLED_Transmit_DMA(ImageData *Imagedata)
{
    while (!dmaReady) {
        System::Delay(1); 
    }
    dmaReady = false;
    OLED_SetWindow(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    pin_dc.Write(true);
    
    uint32_t dmaSize = BytesForPart(0, DISPLAY_WIDTH, 0, DISPLAY_HEIGHT); 
    SCB_CleanDCache_by_Addr((uint32_t*)Imagedata->data, dmaSize); 
    spi_display.DmaTransmit((UBYTE*)Imagedata->data, dmaSize, NULL, OLED_Dma_Ready, NULL);
}

void OLED_Transmit_DMA_Part(ImageData *Imagedata, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    while (!dmaReady) {
        System::Delay(1); 
    }
        dmaReady = false;
        OLED_SetWindow(Xstart, Ystart, Xend, Yend);
        pin_dc.Write(true);
        
        uint32_t dmaSize = BytesForPart(Xstart, Xend, Ystart, Yend); 
        SCB_CleanDCache_by_Addr((uint32_t*)Imagedata->data, dmaSize);    
        spi_display.DmaTransmit((UBYTE*)Imagedata->data, dmaSize, NULL, OLED_Dma_Ready, NULL);
        
}