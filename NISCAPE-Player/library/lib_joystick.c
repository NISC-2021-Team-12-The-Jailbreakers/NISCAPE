/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#include <library/lib_joystick.h>

 static adcsample_t samples[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];

 static thread_reference_t trp = NULL;

 extern int risp_selezionata;
 extern int disp_lettere[4];

static void adccallback(ADCDriver *adcp) {

  if (adcIsBufferComplete(adcp)) {
    chSysLockFromISR();
    chThdResumeI(&trp, (msg_t) MSG_ADC_OK );  /* Resuming the thread.*/
    chSysUnlockFromISR();
  }
}

void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {
  (void)adcp;
  (void)err;
  chSysLockFromISR();
  chThdResumeI(&trp, (msg_t) MSG_ADC_KO );  /* Resuming the thread with Error Message.*/
  chSysUnlockFromISR();
}


static const ADCConversionGroup adcgrpcfg = {
  FALSE,
  ADC_GRP_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN10(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3) , /* SMPR1 */
  0,                        /* SMPR2 */
  0,                        /* HTR */
  0,                        /* LTR */
  ADC_SQR1_NUM_CH(ADC_GRP_NUM_CHANNELS),  /* SQR1 */
  0,                        /* SQR2 */
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11) | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10) /* SQR3 */
};



static THD_WORKING_AREA( waJoystickManager, 128 + sizeof(float) * ADC_GRP_NUM_CHANNELS );
static THD_FUNCTION( joystickManager, arg ) {
  (void) arg;
  chRegSetThreadName("ADC");
  bool flag = true;

  /*
   * Setting as analog input:
   *    PORTC PIN 0 -> ADC1_CH10
   *    PORTC PIN 1 -> ADC1_CH11
   */
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1),
                  0, PAL_MODE_INPUT_ANALOG);

  adcStart(&ADCD1, NULL);

  while( true ) {

    float converted[ADC_GRP_NUM_CHANNELS] = { 0.0f };
    msg_t msg;
    int i;

    adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_GRP_BUF_DEPTH);

    chSysLock();
    msg = chThdSuspendS(&trp);
    chSysUnlock();
    /*
     * Check if acquisition is OK or KO
     */
    if( (uint32_t) msg == MSG_ADC_KO ) {
      continue;
    }

    /*
     * Clean the buffer and then calculate the average value.
     */
    for( i = 0; i < ADC_GRP_NUM_CHANNELS; i++ ) {
      converted[i] = 0.0f;
    }

    for( i = 0; i < ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH; i++ ) {
      converted[ i % ADC_GRP_NUM_CHANNELS] += (float) samples[i] * 100 / 4096;
    }

    for( i = 0; i < ADC_GRP_NUM_CHANNELS; i++ ) {
      converted[i] /= ADC_GRP_BUF_DEPTH;
    }

    /*
     * Se il bottone rimane fermo al centro non stampo i risultati, altrimenti si
     * converted[0] = asse X
     * converted[1] = asse Y
     */
    if(!((converted[1]<= (50 + RANGE)) && (converted[1]>= (50 - RANGE)) && (converted[0]<= (50 + RANGE)) && (converted[0]>= (50 - RANGE))))
    {
      if((converted[0]<= 50 + RANGE) && (converted[0] >= 50 - RANGE) && (converted[1] <= 0 + RANGE) && (flag == true))
      {
       /* chprintf( chp, "X channel = %f %\n\r", converted[0] );
        chprintf( chp, "Y channel = %f %\n\r", converted[1] );*/
        //risp_selezionata = SU;
        risp_selezionata = disp_lettere[0];
        flag = false;
        //chprintf(&SD2, "%d\r\n",risp_selezionata);

      }
      else if((converted[0]<= 50 + RANGE) && (converted[0] >= 50 - RANGE) && (converted[1] >= 100 - RANGE)&& (flag == true))
      {
        //risp_selezionata = GIU;
        risp_selezionata = disp_lettere[2];
        flag = false;
        //chprintf(&SD2, "%d\r\n",risp_selezionata);
      }
      else if((converted[0]<= 0 +RANGE) && (converted[1]<= (50 + RANGE)) && (converted[1]>= (50 - RANGE)) && (flag == true))
      {
        //risp_selezionata = DX;
        risp_selezionata = disp_lettere[1];
        flag = false;
        //chprintf(&SD2, "%d\r\n",risp_selezionata);
      }
      else if((converted[0]>= 100 - RANGE) && (converted[1]<= (50 + RANGE)) && (converted[1]>= (50 - RANGE))&& (flag == true) )
      {
        //risp_selezionata = SX;
        risp_selezionata = disp_lettere[3];
        flag = false;
        //chprintf(&SD2, "%d\r\n",risp_selezionata);
      }


    }
    else
    {
      flag = true;
    }
                /* occhio qua eh*/
    chThdSleepMilliseconds(30);
  }

  adcStop(&ADCD1);

}

void launch_ADC_Thread(void)
{
  chThdCreateStatic( waJoystickManager, sizeof( waJoystickManager), NORMALPRIO +1, joystickManager ,NULL);
}


