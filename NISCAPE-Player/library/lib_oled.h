/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#ifndef LIBRARY_LIB_OLED_H_
#define LIBRARY_LIB_OLED_H_


#include "ch.h"
#include "hal.h"
#include "library/ssd1306ExtendedLib/ssd1306ExtendedLib.h"


/* non static function*/
void initOLED(void);
void winnerOLED(void);
void correctAnswerOLED(void);
void wrongAnswerOLED(void);
void gameOverOLED(void);
void answersScreen(void);


#endif /* LIBRARY_LIB_OLED_H_ */
