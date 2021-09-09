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

#include "ssd1306ExtendedLib.h"

/*
 * ssd1306 constants configurations
 */
static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const SSD1306Config ssd1306cfg = {
  &I2CD1,
  &i2ccfg,
  SSD1306_SAD_0X78,
};

static SSD1306Driver SSD1306D1;

/*
 * ssd1306 Extended Library Functions
 */

/*
 * Function: oledClearScreen
 * Features: clears the screen, setting all pixels to black and updating
 * Usage: oledClearScreen();
 * Parameters: none
 */
void oledClearScreen(void){
  ssd1306FillScreen(&SSD1306D1, 0x00);
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Function: oledColorScreen
 * Features: fills the screen with color and updates it
 * Usage: oledColorScreen();
 * Parameters: none
 */
void oledColorScreen(void){
  ssd1306FillScreen(&SSD1306D1, 0x01);
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Function: oledDrawImage
 * Features: Draws an image on screen, animation from top to down
 * Usage: oledDrawImage(imageData);
 * Parameters: see imagesData.c for details on how to insert the image
 * Notes: it was included in NISC 2021 examples
 */
void oledDrawImage(const ssd1306_color_t image_data[]){
  int x = 0;
  int y = 0;
  uint16_t c = 0;
  oledColorScreen();
  for(y = 0; y<SSD1306_HEIGHT; y++){
    for(x = 0; x<SSD1306_WIDTH; x++){
      ssd1306DrawPixel(&SSD1306D1, x, y, *(image_data + c));
      c++;
      }
    ssd1306UpdateScreen(&SSD1306D1);
  }
}

/*
 * Function: oledDrawWindrose
 * Features: Draw a windrose symbol (circle with four spikes)
 * Usage: oledDrawWindrose(x, y, r, color);
 * Parameters: x, y is circle center in pixels; r is circle radius (the overall geometry is built on that), color can be "BLACK" or "COLOR"
 */
void oledDrawWindrose(int16_t x, int16_t y, int16_t r, char *color){
  // Variables
  int j;
  int k;
  int ssd1306Color;
  j = round( ((1 + sqrt(3)) / sqrt(2)) * r ); // half side of the inscribed square plus the height of the equilateral triangle with two vertex in the inscribed square ones
  k = round(r / (sqrt(2)) ); // half side of the inscribed square, from the radius of the circumscribed circle

  // Black/Color as defined by ssd1306 library
  if(strcmp(color, "BLACK") == 0)
  {
    ssd1306Color = SSD1306_COLOR_BLACK;
  }
  else if(strcmp(color, "COLOR") == 0)
  {
    ssd1306Color = SSD1306_COLOR_WHITE;
  }
  else
  {
    return;
  }

  // Drawing
  ssd1306DrawCircleFill(&SSD1306D1, x, y, r, ssd1306Color); //Center circle
  ssd1306DrawTriangleFill(&SSD1306D1, x - k, y - k, x + k, y - k, x, y - j, ssd1306Color); //Upper triangle
  ssd1306DrawTriangleFill(&SSD1306D1, x - k, y + k, x + k, y + k, x, y + j, ssd1306Color); //Lower triangle
  ssd1306DrawTriangleFill(&SSD1306D1, x + k, y - k, x + k, y + k, x + j, y, ssd1306Color); //Right triangle
  ssd1306DrawTriangleFill(&SSD1306D1, x - k, y - k, x - k, y + k, x - j, y, ssd1306Color); //Left triangle
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Function: oledFillLowerScreen
 * Features: fills with color the lower part (blue) of the screen
 * Usage: oledFillLowerScreen();
 * Parameters: None
 */
void oledFillLowerScreen(void){
  ssd1306DrawRectangleFill(&SSD1306D1, 0, 15, 128, 49, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Function: oledFillUpperScreen
 * Features: fills with color the upper part (yellow) of the screen
 * Usage: oledFillUpperScreen();
 * Parameters: None
 */
void oledFillUpperScreen(void){
  ssd1306DrawRectangleFill(&SSD1306D1, 0, 0, 128, 15, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Function: oledPinSetup
 * Features: setups the STM-32 I2C pins, for details see the board example/docs
 * Usage: oledPinSetup();
 * Parameters: None
 */
void oledPinSetup(void){
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
}

/*
 * Function: oledSetupInitStart
 * Features: OLED Setup and init, for details see the board example/docs
 * Usage: oledSetupInitStart();
 * Parameters: None
 */
void oledSetupInitStart(void){
  oledPinSetup();
  ssd1306ObjectInit(&SSD1306D1);
  ssd1306Start(&SSD1306D1, &ssd1306cfg);
}

/*
 * Function: oledWriteString
 * Features: writes a string in a certain position
 * Usage: oledWriteString(x, y, string, font, color);
 * Parameters: x, y is the upper left corner of text position in pixels, string is the string to be printes, font size (in pixels) can be chosen between "7x10" and "11x18", the color can be set as "BLACK" (black on colored background) or "COLOR" (blue/yellow according to position, on black background)
 */
void oledWriteString(uint8_t x, uint8_t y, char *str, char *font, char *color){

  int ssd1306Color; // to set the color as required by the ssd1306 color

  ssd1306GotoXy(&SSD1306D1, x, y); // move the cursor to the desired positions

  // Black/Color choice
  if(strcmp(color, "BLACK") == 0)
  {
    ssd1306Color = SSD1306_COLOR_BLACK;
  }
  else if(strcmp(color, "COLOR") == 0)
  {
    ssd1306Color = SSD1306_COLOR_WHITE;
  }
  else
  {
    return;
  }

  // Font size choice and print on screen
  if(strcmp(font, "7x10") == 0)
  {
    ssd1306Puts(&SSD1306D1, str, &ssd1306_font_7x10, ssd1306Color);
  }
  else if(strcmp(font, "11x18") == 0)
  {
    ssd1306Puts(&SSD1306D1, str, &ssd1306_font_11x18, ssd1306Color);
  }
  else
  {
    return;
  }
  ssd1306UpdateScreen(&SSD1306D1);
}
