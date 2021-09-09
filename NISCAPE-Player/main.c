/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
____________________________ "Player" part _____________________________
_____________________________ MIT License ______________________________
***********************************************************************/


#include "library/lib_joystick.h"
#include "library/lib_question.h"
#include "library/lib_oled.h"

#define TEMPO_ATTESA_INIT_GIOCO     3                               //seconds before starting the game
#define N_RISP_CORR_LIV_1           3                               //number of correct questions to open the first door
#define N_RISP_CORR_LIV_2           5                               //number of correct questions to open the second door
#define N_SEC_TICK                  1000                            // milliseconds for each time tick (see below)

#define TX_LINE                     PAL_LINE(GPIOB,10U)             //D6
#define LED_VERDE                   PAL_LINE(GPIOA,10U)             //D2
#define LED_BLU                     PAL_LINE(GPIOB,5U)              //D4
#define LED_ROSSO                   PAL_LINE(GPIOA,8U)              //D7


int risp_selezionata;                                               // answer selected by user
int disp_lettere[4] = {0,1,2,3};                                    // characters arrangement to show on screen, it's then shuffled by random_disp_lettere();
int difficulty = 0;                                                 // 0 for room 1, 1 for room 2
int timeTicks = 4;                                                  // number of time ticks
int n_vite = 3;                                                     //number of lives, note that
int n_risp_corrette = 0;                                            // initializes the number of correct answers, to check if a level has been solved
bool new_question = false;                                          // flag for a new question selection, for time management, resets the ticks on new question

/*
 * Thread that manage the IR communication between nucleo1 and nucleo2 (open the door)
 * if the user passes the first level, IR LED TX = 1 -> the door 1 opens
 * if the user passes the second level, IR LED TX = 1 -> the door 2 opens
 */
static THD_WORKING_AREA(waTxManager, 128);
static THD_FUNCTION(txManager, arg) {

  (void)arg;
    chRegSetThreadName("txManager");
    bool flag = false; //accendo solo una volta il led TX
    while(true)
    {
      if((n_risp_corrette == N_RISP_CORR_LIV_1 || n_risp_corrette == N_RISP_CORR_LIV_2) && flag == false)
      {
          //TX = 1 open the door 1
          flag = true;
          if(n_risp_corrette == N_RISP_CORR_LIV_1)
          {
            difficulty = 1;
            chprintf((BaseSequentialStream*)&SD2,"Opening door 1\r\n");
          }
          else
          {
            chprintf((BaseSequentialStream*)&SD2,"Opening door 2\r\nYOU WON YOU WON YOU WON YOU WON");
            winnerOLED();
          }

          palSetLine(TX_LINE);
          chThdSleepMilliseconds(1000);
          palClearLine(TX_LINE);
      }
      else if(!(n_risp_corrette == N_RISP_CORR_LIV_1 || n_risp_corrette == N_RISP_CORR_LIV_2))
      {
        flag = false;
      }

      chThdSleepMilliseconds(50);
    }
}




/*
 * time ticks manager thread
 * if the user is in the first level (difficulty = 0) there is no time to answer the question
 * if the user is in the second level (difficulty = 1) there are (timeTicks * N_SEC_TICK) milliseconds to answer the question
 * timeTicks defined the number of windroses that we see on the OLED. This variable is read by the OLED functions in order to visualize
 * the correct number of windroses at each refresh.
 */
static THD_WORKING_AREA(waTickManager, 128);
static THD_FUNCTION(tickManager, arg) {

  (void)arg;
  chRegSetThreadName("tickManager");
  while (true) {
    if((difficulty == 1) && (timeTicks>=0) && (new_question == true))
    {
      answersScreen();
      timeTicks--;

    }
    if((difficulty == 1)  && (new_question == true) )
    {
      chThdSleepMilliseconds(N_SEC_TICK);
    }
    else
    {
      chThdSleepMilliseconds(10);
    }

  }
}


/*
 * disp_lettere is the array that define the order of characters on the OLED
 * In order to randomize the order of the characters on the OLED, we randomize this array.
 * This array is read by the joystick in order to send a value associated with the choosen character and
 * by the OLED at each refresh
 */
static void random_disp_lettere(void)
{
   int temp_arr[4] = {-1, -1 , -1 , -1};                //array that stores the past random value
   int temp_var;
   bool flag = false;


   disp_lettere[0] = rand() % 4;                        //generate a random value from 0 to 3
   temp_arr[0] = disp_lettere[0];                       //we assign it to the first index of disp_lettere

   /*
    * disp_lettere[1] and disp_lettere[2] are assigned with a random value
    */
   for(int i = 1;i <3;i++)
   {
     do
     {
       temp_var = rand() % 4;
       flag = false;
            for(int j = 0;j<4 && (flag == false);j++)
            {
              if((temp_var == temp_arr[j]))
              {
                flag = true;
              }
            }
     }while(flag == true);

     temp_arr[i] = temp_var;
     disp_lettere[i] = temp_var;

   }

   /*
    * to speed up the creation of disp_lettere, the last value is calculated
    */
   disp_lettere[3] = 6-disp_lettere[0] - disp_lettere[1] - disp_lettere[2];


}

/*
 * this function turn on/off the leds associated with the number of lives
 */
static void turnLedLifes(void)
{
  switch(n_vite)
  {
  case 3:
    palSetLine(LED_VERDE);
    palSetLine(LED_BLU);
    palSetLine(LED_ROSSO);

    break;
  case 2:
    palClearLine(LED_VERDE);
    palSetLine(LED_BLU);
    palSetLine(LED_ROSSO);

    break;
  case 1:
    palClearLine(LED_VERDE);
    palClearLine(LED_BLU);
    palSetLine(LED_ROSSO);
    break;
  case 0:
    palClearLine(LED_VERDE);
    palClearLine(LED_BLU);
    palClearLine(LED_ROSSO);
    break;
  default:
    palClearLine(LED_VERDE);
    palClearLine(LED_BLU);
    palClearLine(LED_ROSSO);
    break;


  }
}

static Question questions[N_QUESTIONS];            //structure to define the questions with the relatives answers

int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  //TODO: da rivedere
  risp_selezionata = -1;
  int actual_question = -1;
  bool res=false;                     //variable for GPIOs configurations

  sdStart(&SD2, NULL);

  //int random=-1;  //random var

  /*
   * Set of all the LED in the project
   */
  palSetLineMode(TX_LINE,PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LED_ROSSO,PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LED_BLU,PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LED_VERDE,PAL_MODE_OUTPUT_PUSHPULL);

  palClearLine(TX_LINE);                                            //set to 0 the TX IR LED
  inizializeStruct(questions);                                      //we load all the questions, the answers and the index of the correct answer
  initOLED();                                                       //init OLED()

  chThdSleepSeconds(TEMPO_ATTESA_INIT_GIOCO);                       //time to start the game


  chThdCreateStatic(waTickManager, sizeof(waTickManager), NORMALPRIO + 2, tickManager, NULL);   //thread  tickManager
  chThdCreateStatic(waTxManager, sizeof(waTxManager), NORMALPRIO -1 , txManager, NULL);         //thread txManager
  launch_ADC_Thread();                                                                          //thread ADC

  int k=0;
  bool flag2 = true;

  /*
   * Here is the start of the game.
   * We select a random question from our set and see it on terminal
   */
  while ((k<N_QUESTIONS) && (n_vite > 0) && (n_risp_corrette != N_RISP_CORR_LIV_2))
  {

        turnLedLifes();                                                      //based on the variable n_vite we switch on/off the LEDs
        risp_selezionata = -1;                                               //init of the variable

        if(risp_selezionata == -1 && flag2 == true)
        {


          actual_question = generateRandomQuestion();                       //generation of a random index
          printSingleQuestionOnTerminal(&questions[actual_question]);       //print the random question and relative answers
          new_question = true;
          random_disp_lettere();                                            //generation of the random pattern of characters on OLED

          /*
           * based on the difficulty we update the screen
           * if the user is at the second level we update timeTicks by using thread tickManager
           */
          if(difficulty == 1)
          {
              timeTicks = 4;
              chThdSleepMilliseconds(20);
          }
          else
          {
              answersScreen();
          }

        }

         /*
          * wait for answer by the user. The anser is stored into "risp_selezionata"
          */

          chThdSleepMilliseconds(100);


         if(risp_selezionata != -1)
         {
             //if the user replied
             new_question = false;
             res = checkRightAnswer(&questions[actual_question]);                       //check the user answer
             if(res == 0)
             {
               //wrong answer
               n_vite--;
               wrongAnswerOLED();                                                       //OLED shows the wrong answer image
               chThdSleepSeconds(2);

             }
             else
             {
               //right answer
               n_risp_corrette++;
               correctAnswerOLED();                                                     //OLED shows the correct answer image
               chThdSleepSeconds(2);

             }
             k++;
             flag2 = true;

         }
         else
         {
           flag2 = false;
         }

         /*
          * if the user is at the second level but all the time elapsed without an answer
          * the answer is considered wrong
          */
         if(timeTicks == -1)
         {
             n_vite--;
             turnLedLifes();
             flag2 = true;
             chprintf((BaseSequentialStream*)&SD2,"Timeout\r\n");
             wrongAnswerOLED();
             chThdSleepSeconds(2);
         }



     }

    /*
     * if n_vite = 0, the user died
     */
    if(n_vite == 0)
    {
      turnLedLifes();
      gameOverOLED();
      chprintf((BaseSequentialStream*)&SD2,"RIP RIP RIP RIP RIP RIP\r\n");

      chThdSleepSeconds(1);
    }


}
