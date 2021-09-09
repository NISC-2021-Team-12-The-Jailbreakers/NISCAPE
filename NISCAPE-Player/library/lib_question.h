/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#ifndef LIBRARY_LIB_QUESTION_H_
#define LIBRARY_LIB_QUESTION_H_

#include "ch.h"
#include "hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chprintf.h"

#define N_QUESTIONS 10 //number of questions
#define N_CHAR 20

typedef struct q_struct{  //this is the struct that exstablishes the form of every question
  char question [N_CHAR];
  char answer [4][N_CHAR];
  int id_right_answer;
}Question;



void inizializeStruct(Question* questions);  //a function to fill the struct of the game
void printStructOnTerminal(Question* questions); //a function to print all the AQs of the game on the terminal
void printSingleQuestionOnTerminal(Question* to_print); //a function to print a single question on the terminal
bool searchValueInArray (int);
bool checkRightAnswer (Question*); //a function to check if answer is right
int generateRandomQuestion(void);

#endif /* LIBRARY_LIB_QUESTION_H_ */
