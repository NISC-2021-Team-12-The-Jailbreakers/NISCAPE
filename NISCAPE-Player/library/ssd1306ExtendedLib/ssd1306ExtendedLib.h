/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/
/*
 * ssd1306 Extended Library
 * This is an extension for the stm32-ssd1306 library
 * Put it in the same folder that collects libraries
 */

/*
 * Define the library if it has not been defined yet
 */
#ifndef LIBRARY_SSD1306_EXTENDED_H_
#define LIBRARY_SSD1306_EXTENDED_H_

/*
 * Standard libraries include (Ignore unresolved inclusion, ChibiStudio's bug, see https://forum.chibios.org/viewtopic.php?f=13&t=5922)
 */
#include <string.h> //needed for strcmp
#include <math.h> // needed for geometrical symbols

/*
 * Custom libraries include
 */
#include "..\ssd1306\ssd1306.h"
#include "imagesData.c"

/*
 * Functions prototypes. For function docs, see them in the .c file.
 */
void oledClearScreen(void);
void oledColorScreen(void);
void oledDrawImage(const ssd1306_color_t image_data[]);
void oledDrawWindrose(int16_t x, int16_t y, int16_t r, char *color);
void oledFillLowerScreen(void);
void oledFillUpperScreen(void);
void oledPinSetup(void);
void oledSetupInitStart(void);
void oledWriteString(uint8_t x, uint8_t y, char *str, char *font, char *color);

#endif // End of definition - LIBRARY_SSD1306_EXTENDED_H_
