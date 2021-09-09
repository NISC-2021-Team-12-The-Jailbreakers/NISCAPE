/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ "Room" part ______________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#include "ch.h"
#include "hal.h"
#include "stdio.h"

#define LED_RX_PORT         GPIOA
#define LED_RX_PIN          8U
#define RX_LINE             PAL_LINE(LED_RX_PORT,LED_RX_PIN)  //D7
#define CLOSE_DOOR          750
#define OPEN_DOOR           250


#define DEBOUNCE_TIME       1000
/*
 * PWM Driver Configuration.
 */

int rx_door;
/*
 * PWM Driver Configuration.
 */
//One driver for two channels, 0 and 1
static PWMConfig pwmcfg = {
  10000,
  200,
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL}, //after consulting datasheet, PB5 and PB4 works on two channels with the same driver
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};



void open(void)
{
  if(rx_door == 1)
  {
    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, OPEN_DOOR));
    chThdSleepSeconds(1);
    //close door
    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, CLOSE_DOOR));
  }
  else if(rx_door == 2)
  {
    pwmEnableChannel(&PWMD3, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, OPEN_DOOR));
    chThdSleepSeconds(1);
    //close door
    pwmEnableChannel(&PWMD3, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, CLOSE_DOOR));
  }
}

int main(void) {
  halInit();
  chSysInit();
  rx_door = 0;
  // It configures PWM related PIN.
  //PB4 = D5  -> TIM3_CH1
  palSetPadMode(GPIOB, 4, PAL_MODE_ALTERNATE(2));
  palSetLineMode(RX_LINE,PAL_MODE_INPUT);

  //PB5 = D4 -> TIM3_CH2
  palSetPadMode(GPIOB, 5, PAL_MODE_ALTERNATE(2)); //sets alternative function 2 according to datasheet

  pwmStart(&PWMD3, &pwmcfg);
  // It stars PWM driver.
  pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, CLOSE_DOOR));
  pwmEnableChannel(&PWMD3, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, CLOSE_DOOR));


  while (true) {
    if(palReadLine(RX_LINE) == PAL_HIGH)
    {
      chThdSleepMilliseconds(DEBOUNCE_TIME);  //TODO: da aumentare (?)
      if(rx_door == 0)
      {
        rx_door = 1;
      }
      else if(rx_door == 1)
      {
        rx_door = 2;
      }
      else if(rx_door == 2)
      {
        rx_door = 0; //per sicurezza se eventualmente ci sono tre impulsi
      }
      open();
    }
    chThdSleepMilliseconds(50);
  }

}
