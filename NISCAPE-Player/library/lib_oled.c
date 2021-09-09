/***********************************************************************
_______________________________ NISC 2021 ______________________________
_______________________ Team 12 (The Jailbreakers) _____________________
___________________________ "NISCAPE" Project __________________________
_____________________________ MIT License ______________________________
***********************************************************************/

#include "lib_oled.h"

/*
 * def variable
 */
char buffer[20];   //buffer included in osLibtest_sequence (?)
extern int disp_lettere[4]; //array for answers order

extern int difficulty; //game difficulty

//Time Ticks
extern int timeTicks; //used to change the answers screen based on time


// Possible Answers Screen
void answersScreen(void){
  oledClearScreen();

//  Debug drawings
//  ssd1306DrawLine(&SSD1306D1, 0, 32, 128, 32, SSD1306_COLOR_WHITE); //Test crosshair
//  ssd1306DrawLine(&SSD1306D1, 64, 0, 64, 64, SSD1306_COLOR_WHITE);  //Test crosshair
//  ssd1306UpdateScreen(&SSD1306D1);

// Setup a char vector according to numeric IDs, to show answers in correct order
  char *answers[4];

  for(int i = 0;i <4;i++){
    if(disp_lettere[i] == 0){
       answers[i]="A";
    }
    else if(disp_lettere[i] == 1){
      answers[i]="B";
   }
    else if(disp_lettere[i] == 2){
      answers[i]="C";
   }
    else if(disp_lettere[i] == 3){
      answers[i]="D";
   }
  }

  if(difficulty==0){
    // Drawings
    oledDrawWindrose(64, 32, 5, "COLOR"); //center windrose
    oledDrawWindrose(15, 15, 5, "COLOR"); //top left windrose
    oledDrawWindrose(112, 15, 5, "COLOR"); //top right windrose
    oledDrawWindrose(15, 49, 5, "COLOR"); //low left windrose
    oledDrawWindrose(112, 49, 5, "COLOR"); //low right windrose
    // Answers
    oledWriteString(59,3,answers[0],"11x18", "COLOR"); //North
    oledWriteString(80,24,answers[1],"11x18", "COLOR"); //East
    oledWriteString(59,45,answers[2],"11x18", "COLOR"); //South
    oledWriteString(40,24,answers[3],"11x18", "COLOR"); //West
  }
  else if(difficulty==1){
    // Drawings based on time ticks
    if(timeTicks==4){
      oledDrawWindrose(15, 15, 5, "COLOR"); //top left windrose
    }
    if(timeTicks>=3){
      oledDrawWindrose(112, 15, 5, "COLOR"); //top right windrose
    }
    if(timeTicks>=2){
      oledDrawWindrose(15, 49, 5, "COLOR"); //low left windrose
    }
    if(timeTicks>=1){
      oledDrawWindrose(64, 32, 5, "COLOR"); //center windrose
      oledDrawWindrose(112, 49, 5, "COLOR"); //low right windrose
      oledWriteString(59,3,answers[0],"11x18", "COLOR"); //North
      oledWriteString(80,24,answers[1],"11x18", "COLOR"); //East
      oledWriteString(59,45,answers[2],"11x18", "COLOR"); //South
      oledWriteString(40,24,answers[3],"11x18", "COLOR"); //West
    }
  }
}

// Correct Answer Screen
static void correctAnswer(void){
  oledDrawImage(thumbsUpIcon);
}

// Game Over Screen
static void gameOverScreen(void){
  oledWriteString(1,1,"____GAME OVER____","7x10", "COLOR");
  oledFillLowerScreen();
  oledWriteString(20,20,"Now you are...","7x10", "BLACK");
  oledWriteString(1,42,"A NISC 2022 TUTOR!","7x10", "BLACK");
}

// Startup Screen
static void startupScreen(void){
  oledClearScreen();
  oledFillUpperScreen();
  oledWriteString(2,2,"___By Team 12___","7x10", "BLACK");
  oledWriteString(2,20,"ESCAPE NISC","11x18", "COLOR");
  oledWriteString(42,40,"2021","11x18", "COLOR");
}


// Victory Screen
static void victoryScreen(void){
  oledWriteString(1,1,"_____VICTORY_____","7x10", "COLOR");
  oledFillLowerScreen();
  oledWriteString(20,20,"YOU WON!","11x18", "BLACK");
  oledWriteString(15,42,"FREEDOOM!","11x18", "BLACK");
}

// Wrong Answer Screen
static void wrongAnswer(void){
  oledDrawImage(warningIcon);
}

/*
 * Non static functions
 * Based on the above functions, include screen clearing and other functions to simplify syntax
 */

//correct answer screen
void correctAnswerOLED(void)
{
  oledClearScreen();
  correctAnswer();
}

//game over screen
void gameOverOLED(void)
{
  oledClearScreen();
  gameOverScreen();
}

//OLED initialization
void initOLED(void)
{
  oledSetupInitStart();
  oledClearScreen();
  startupScreen();
}

//victory screen
void winnerOLED(void)
{
    oledClearScreen();
    victoryScreen();
}

//wrong answer screen
void wrongAnswerOLED(void)
{
  oledClearScreen();
  wrongAnswer();
}


