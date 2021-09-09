/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#ifndef LIBRARY_LIB_JOYSTICK_H_
#define LIBRARY_LIB_JOYSTICK_H_

#include "lib_question.h"

#define VOLTAGE_RES            ((float)3.3/4096)

#define MSG_ADC_OK               0x1337
#define MSG_ADC_KO               0x7331
#define RANGE  10  //mi identifica un range di valori in un intorno della posizione accettabili del joystick

#define ADC_GRP_NUM_CHANNELS        2
#define ADC_GRP_BUF_DEPTH           10

#define INSERT_RANDOM TRUE

void launch_ADC_Thread(void);


#endif /* LIBRARY_LIB_JOYSTICK_H_ */
