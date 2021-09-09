/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#include "lib_question.h"

int used_indexes[N_QUESTIONS];

int generateRandomQuestion(void)
{
  bool found=false;     //variable for search in used_indexes
  int random = -1;
  int index=0; //index used to write used_indexes
  do{
          random=rand()% N_QUESTIONS;
          found = searchValueInArray (random);
          if (!found){
            used_indexes[index]=random;
            index++;
          }
     }while (found);
     return random;
}

void inizializeStruct (Question* questions){
  for (int i=0; i<N_QUESTIONS; i++){
         used_indexes[i]=-1;
     }

  /*------------------------------Question 1-----------------------------------------*/
    strcpy (questions[0].question, "It emits light");
    strcpy (questions[0].answer[0], "Salvatore Lo Iacono");
    strcpy (questions[0].answer[1], "Mutex");
    strcpy (questions[0].answer[2], "LEDs");
    strcpy (questions[0].answer[3], "PWM");
    questions[0].id_right_answer = 2;  //right answer's index in answers vector
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 2-----------------------------------------*/
    strcpy (questions[1].question, "His surname's Miele");
    strcpy (questions[1].answer[0], "Carlo");
    strcpy (questions[1].answer[1], "Marina");
    strcpy (questions[1].answer[2], "Domenica");
    strcpy (questions[1].answer[3], "Anna");
    questions[1].id_right_answer = 0;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 3-----------------------------------------*/
    strcpy (questions[2].question, "STM32F401's bible");
    strcpy (questions[2].answer[0], "Datasheet");
    strcpy (questions[2].answer[1], "Rosarium");
    strcpy (questions[2].answer[2], "Freud's book");
    strcpy (questions[2].answer[3], "NISC site");
    questions[2].id_right_answer = 0;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 4-----------------------------------------*/
    strcpy (questions[3].question, "Analog to digital:");
    strcpy (questions[3].answer[0], "ACDC");
    strcpy (questions[3].answer[1], "DAC");
    strcpy (questions[3].answer[2], "Slipknot");
    strcpy (questions[3].answer[3], "ADC");
    questions[3].id_right_answer = 3;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 5-----------------------------------------*/
    strcpy (questions[4].question, "A pin in ChibiOS");
    strcpy (questions[4].answer[0], "USART");
    strcpy (questions[4].answer[1], "ADC");
    strcpy (questions[4].answer[2], "Frodo's Ring");
    strcpy (questions[4].answer[3], "Pad");
    questions[4].id_right_answer = 3;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 6-----------------------------------------*/
    strcpy (questions[5].question, "ChibiOS's father");
    strcpy (questions[5].answer[0],"Luke Skywalker");
    strcpy (questions[5].answer[1], "Alan Smith");
    strcpy (questions[5].answer[2], "ChibiOS doesn't have father");
    strcpy (questions[5].answer[3], "Giovanni Di Sirio");
    questions[5].id_right_answer = 3;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 7-----------------------------------------*/
    strcpy (questions[6].question, "A test can be");
    strcpy (questions[6].answer[0], "Difficult");
    strcpy (questions[6].answer[1], "Functional");
    strcpy (questions[6].answer[2], "Sad");
    strcpy (questions[6].answer[3], "NISC");
    questions[6].id_right_answer = 1;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 8-----------------------------------------*/
    strcpy (questions[7].question, "It can drive OLED");
    strcpy (questions[7].answer[0], "I2C");
    strcpy (questions[7].answer[1], "PWM");
    strcpy (questions[7].answer[2], "UDP");
    strcpy (questions[7].answer[3], "TCP");
    questions[7].id_right_answer = 0;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 9-----------------------------------------*/
    strcpy (questions[8].question, "I2C communicates as");
    strcpy (questions[8].answer[0], "Simplex");
    strcpy (questions[8].answer[1], "Full duplex");
    strcpy (questions[8].answer[2], "Half duplex");
    strcpy (questions[8].answer[3], "By phone");
    questions[8].id_right_answer = 2;
    /*---------------------------------------------------------------------------------*/

    /*------------------------------Question 10-----------------------------------------*/
    strcpy (questions[9].question, "Terms interface to");
    strcpy (questions[9].answer[0], "A-Scell");
    strcpy (questions[9].answer[1], "System bus");
    strcpy (questions[9].answer[2], "Shell");
    strcpy (questions[9].answer[3], "Paper");
    questions[9].id_right_answer = 2;
    /*---------------------------------------------------------------------------------*/


   /*---------------------------------------------------------------------------------*/


}

#if 0
void printStructOnTerminal(Question* questions){
  char character[4] = {'A', 'B', 'C','D'};   //only an array to have question's index without specify it in the for cycle below
  int i,j=0;

  for (i=0; i<N_QUESTIONS; i++){
    chprintf((BaseSequentialStream*)&SD2,"Question %d): %s\r\n", i+1, questions[i].question);
      for (j=0; j<4; j++){
        chprintf((BaseSequentialStream*)&SD2,"%c) %s \r\n",character[j], questions[i].answer[j]);
      }
  }
}

#endif


void printSingleQuestionOnTerminal(Question* to_print)
{
  char character[4] = {'A', 'B', 'C','D'};

  chprintf((BaseSequentialStream*)&SD2,"Question: %s\r\n", to_print->question);

      for (int j=0; j<4; j++){
        chprintf((BaseSequentialStream*)&SD2,"%c) %s\r\n",character[j], to_print->answer[j]);

      }

}





bool searchValueInArray (int ran){
  bool found=false;
  for (int i=0; i<N_QUESTIONS; i++){
    if (used_indexes[i]==ran)
      found=true;
  }
 return found;
}



bool checkRightAnswer (Question* questions){
  bool result=false;
    extern int risp_selezionata;
      if(questions->id_right_answer==risp_selezionata)
      {
        result=true;
        chprintf((BaseSequentialStream*)&SD2, "Right answer!\r\n");
      }
      else
      {
        chprintf((BaseSequentialStream*)&SD2, "Wrong answer\r\n");
      }
      return result;
}

