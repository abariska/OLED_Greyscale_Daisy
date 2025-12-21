/**
  ******************************************************************************
  * @file    fonts.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   Header for fonts.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYFONTS_H
#define __MYFONTS_H


/*最大字体微软雅黑24 (32x41) */
#define MAX_HEIGHT_FONT         41
#define MAX_WIDTH_FONT          32
#define OFFSET_BITMAP           

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

typedef struct {
  const uint16_t *data;
  uint16_t width;
  uint16_t height;
  uint8_t dataSize;
  } tImage;

typedef struct {
  long int code;
  const tImage *image;
  } tChar;
  
typedef struct {
  int length;
  const tChar *chars;
  } tFont;

extern const tFont Font8Bold;
extern const tFont Font12Bold;
extern const tFont Font16Bold;
extern const tFont Font20Bold;
extern const tFont Font24Bold;
extern const tFont Regular_8;
extern const tFont Regular_12;
extern const tFont Regular_16;
extern const tFont Regular_20;
extern const tFont Regular_24;
extern const tFont Bohnshcrift_B16;
extern const tFont Calibri_B16;
extern const tFont Roboto_B_12;
extern const tFont Roboto_B_14;
extern const tFont Roboto_B_16;
extern const tFont Roboto_B_18;
extern const tFont Roboto_B_20;
extern const tFont Roboto_B_24;
extern const tFont Roboto_L_12;
extern const tFont Roboto_L_14;
extern const tFont Roboto_L_16;
extern const tFont Roboto_L_18;
extern const tFont Roboto_L_20;
extern const tFont Roboto_L_24;
extern const tFont Roboto_M_12;
extern const tFont Roboto_M_14;
extern const tFont Roboto_M_16;
extern const tFont Roboto_M_18;
extern const tFont Roboto_M_20;
extern const tFont Roboto_M_24;

#ifdef __cplusplus
}
#endif
  
#endif /* __MYFONTS_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
