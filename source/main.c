/*	Author: dkwon014
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: FINAL PROJECT
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <util/delay.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#include <avr/eeprom.h>
#include "io.c"
#include <stdlib.h>
#endif

//button configuration
/** MENU SELECTION (1-5) **/
#define F_CPU 1000000 UL
#define BUTTON1 (~PIND & 0x01) //right
#define BUTTON2 (~PIND & 0x02) //left
#define BUTTON3 (~PIND & 0x04)
#define DEFAULT_SPEED 60
// #define MOVES_PER_TICK 3

// #define BUTTON4 (~PINA & 0x08) //down (p2)
// #define BUTTON5 (~PINA & 0x10) //reset?

uint8_t highScore;
unsigned char myPlayer; //0x00, 0x01, 0x02, 0x03
unsigned char bestPlayer;
unsigned char playerX = 0;
unsigned char playerY = 0;
unsigned char currentDirection = 1; //trajectory of player
unsigned char gameState; //status of the game
unsigned char mousePos;
int score;
unsigned char speed = DEFAULT_SPEED;
// unsigned char moves = MOVES_PER_TICK;

/**
GAME DATA (MATRIX)
ROWS[1] and COLUMNS[1] designated for player
ROWS[0] and COLUMNS[0] designated for BOT
playerCoords stores the Y coordinates of the player (two values)

**/
unsigned char playerCoords[2];
unsigned char ROWS[2];
unsigned char COLUMNS[2];

enum STATES {
  STATE_OUT
};

enum FALL_STATES {
  FALL_START,
  SPAWN,
  TICK1,
  TICK2,
  TICK3,
  TICK4,
  TICK5,
  TICK6,
  TICK7,
  CALCULATE
};

enum MENU_STATES {
  START,
  WAIT1,
  INC,
  WAIT2,
  START_GAME
}
m_state;

void menu_tick() {
  switch (m_state) { //transitions
  case START:
    if (BUTTON2 && !BUTTON1) {
      m_state = WAIT1;
    } else if (!BUTTON2 && BUTTON1) {
      m_state = WAIT2;
    } else {
      m_state = START;
    }
    break;
  case WAIT1:
    if (!BUTTON2) {
      m_state = INC;
    } else {
      m_state = WAIT1;
    }
    break;
  case INC:
    m_state = START;
    break;
  case WAIT2:
    if (!BUTTON1) {
      m_state = START_GAME;
    } else {
      m_state = WAIT2;
    }
    break;
  case START_GAME:
    // gameState = 0x01;
    if (gameState == 0x01) {
      m_state = START_GAME;
    } else {
      m_state = START;
    }
    break;
  }
  switch (m_state) { //actions
  case START:
    // PORTA = 0xFF;
    // sendMenu();
    break;
  case WAIT1:
    break;
  case INC:
    if (mousePos < 10) {
      mousePos++;
    } else {
      mousePos = 7;
    }
    sendMenu();
    // mousePos++;
    break;
  case WAIT2:
    break;
  case START_GAME:
    gameState = 0x01;
    switch (mousePos) {
    case 7:
      myPlayer = 0x00;
      break;
    case 8:
      myPlayer = 0x01;
      break;
    case 9:
      myPlayer = 0x02;
      break;
    case 10:
      myPlayer = 0x04;
      break;
    }
    sendInGame();
    break;
  }
}

void EEPROM_Write(unsigned char address, unsigned char data) {
  eeprom_write_byte(address, data);
}

unsigned char EEPROM_Read(unsigned char address) {
  return eeprom_read_byte(address);
}

void displayLEDMatrix() { //updates the matrix?
  updateMatrixSingle(playerX, playerY);
  for (int i = 0; i < 2; i++) {
    _delay_us(500);
    PORTA = ROWS[i];
    PORTB = COLUMNS[i];
  }
}

void printPos() {
  LCD_init();
  LCD_ClearScreen();
  LCD_DisplayString(18, "curentY: ");
  LCD_Cursor(29);
  LCD_WriteData(('0' + playerY));
  LCD_Cursor(31);
}

int tick(int state) {
  switch (state) {
  case STATE_OUT:
  // if (moves > 0) {
    if (gameState == 0x01) {
    updateMatrixSingle(playerX, playerY);
    if (BUTTON1) { //left
      moveCharacter(1);
    }
    if (BUTTON2) { //right
      moveCharacter(0);
    }
  }
    break;
  // }
}
    // break;
    // moves--;
  return state;
}

int generateRandom() {
  return (rand() % 8) + 1;;
}

unsigned char convert(int x) {
  switch (x) {
  case 1:
    return 0x01;
    break;
  case 2:
    return 0x02;
    break;
  case 3:
    return 0x04;
    break;
  case 4:
    return 0x08;
    break;
  case 5:
    return 0x10;
    break;
  case 6:
    return 0x20;
    break;
  case 7:
    return 0x40;
    break;
  case 8:
    return 0x80;
    break;
  case 9:
    return 0x90;
    break;
  default:
    return 0x00;
    break;
  }
}

// enum FALL_STATES { FALL_START, SPAWN, TICK1, TICK2, TICK3, TICK4, TICK5, TICK6, TICK7, CALCULATE };
int fall_tick(int state) {
  switch (state) {
  case FALL_START:
    if (gameState == 0x01) {
      state = SPAWN;
    } else {
      state = FALL_START;
      // m_state = START;
    }
    break;
  case SPAWN:
    ROWS[0] = FALLING_SEQUENCE[0];
    COLUMNS[0] = ~(convert(generateRandom())); //SET COLUMN TO SPAWN IN
    state = TICK1;
    break;
  case TICK1:
    ROWS[0] = FALLING_SEQUENCE[1];
    state = TICK2;
    break;
  case TICK2:
    ROWS[0] = FALLING_SEQUENCE[2];
    state = TICK3;
    break;
  case TICK3:
    ROWS[0] = FALLING_SEQUENCE[3];
    state = TICK4;
    break;
  case TICK4:
    ROWS[0] = FALLING_SEQUENCE[4];
    state = TICK5;
    break;
  case TICK5:
    ROWS[0] = FALLING_SEQUENCE[5];
    state = TICK6;
    break;
  case TICK6:
    ROWS[0] = FALLING_SEQUENCE[6];
    state = TICK7;
    break;
  case TICK7:
    ROWS[0] = FALLING_SEQUENCE[7];
    state = CALCULATE;
    break;
  case CALCULATE:
// moves = MOVES_PER_TICK;
    // if (COLUMNS[0] == playerCoords[0] || COLUMNS[0] == playerCoords[1]) {
    if (COLUMNS[0] != playerCoords[0] && COLUMNS[0] != playerCoords[1]) {
      if (score >= 9) {
        state = FALL_START;
        gameState = 0x00;
        // sendWin();
        sendGameEnd(1); // send win endStatus
        //sendMenu();
        // sendInGame();
      } else {
        score++;
        // moves++;
        state = SPAWN;
        // state =
        // task2.elapsedTime+=5;
        sendInGame();
        speed-=5;
        TimerSet(speed);
      }
    } else {
      state = FALL_START;
      gameState = 0x00;
      // sendLose();
      sendGameEnd(0); // send loss endStatus
    }
    break;
  }
  return state;
}
//updates PLAYER
void updateMatrixSingle(unsigned char x, unsigned char y) {
  playerCoords[0] = ~(ROW_VALUES[y]); //save first coordinate of player
  if (currentDirection == 1) {
    playerCoords[1] = ~(ROW_VALUES[y + 1]);
    y = ROW_VALUES[y] + ROW_VALUES[y + 1];
  } else {
    playerCoords[1] = ~(ROW_VALUES[y - 1]);
    y = ROW_VALUES[y - 1] + ROW_VALUES[y];
  }
  // x = HEX_VALUES[x];
  unsigned char convertedY = ~y;
  // unsigned char hexVal1, unsigned char hexVal2;
  ROWS[1] = x;
  COLUMNS[1] = convertedY;
  // ROWS[0] = 0x80;
  // COLUMNS[0] = ~0x80;
}

void moveCharacter(unsigned char pos) {
  // if (moves < 1) {
    // return;
  // }
  switch (pos) {
  case 0:
    if (playerY > 0) {
      playerY--;
    }
    // else {
    //   // currentDirection = 1;
    //   // moveCharacter(1);
    // }
    break;
  case 1: //move right
    if (playerY < 6) {
      playerY++;
    }
    // else {
    //   // currentDirection = 0;
    //   // moveCharacter(0);
    // }
    break;
  }
  updateMatrixSingle(playerX, playerY);
  // moves--;
}

/**
Where endstatus = 0 is a loss
and endstatus = 1 is a win
**/
void sendGameEnd(unsigned char endStatus) {
  TimerSet(DEFAULT_SPEED);
  speed = DEFAULT_SPEED;
  checkHighScore();
  score = 0;
  ROWS[0] = 0;
  COLUMNS[0] = 0;
  ROWS[1] = 0;
  COLUMNS[1] = 0;
  // mousePos = 7;
  LCD_init();
  LCD_ClearScreen();
  if (endStatus == 1) {
  LCD_DisplayString(5, "WINNER: ");
} else {
  LCD_DisplayString(5, "LOSER: ");
}
  LCD_WriteData(myPlayer);
  LCD_DisplayString_NO_CLEAR(18, "PLAY AGAIN?");
}

void sendMenu() {
  // checkHighScore();
  score = 0;
  ROWS[0] = 0;
  COLUMNS[0] = 0;
  ROWS[1] = 0;
  COLUMNS[1] = 0;
  LCD_init();
  LCD_ClearScreen();
  LCD_DisplayString(18, "highscore: ");
  LCD_Cursor(29);
  // EEPROM_Write(0x01, 5);
  // EEPROM_Write(0x00, 0x02);
  // highScore = EEPROM_Read(0x01);
  bestPlayer = EEPROM_Read(0xFF);
  // highScore = eeprom_read_byte((uint8_t*)22);
  // eeprom_write_word(22, highScore);
  // LCD_WriteData(('0' + highScore));
  highScore = eeprom_read_byte((uint8_t*)22);
  //reset highscore
  if(highScore > 20){
  highScore = 0;
  }
eeprom_write_word(22, highScore);
LCD_WriteData(highScore + '0');
  LCD_Cursor(31);
  LCD_WriteData(EEPROM_Read(0xFF));
  //load acharacters
  LCD_createChar(0, player1);
  LCD_createChar(1, player2);
  LCD_createChar(2, player3);
  LCD_createChar(3, player4);
  LCD_Cursor(7);
  LCD_WriteData(0x00);
  LCD_WriteData(0x01);
  LCD_WriteData(0x02);
  LCD_WriteData(0x04);
  LCD_Cursor(mousePos); //cursor pos
}

void checkHighScore() {
  if (score > highScore) {
    eeprom_write_byte((uint8_t*)22, score);
		highScore = eeprom_read_byte((uint8_t*)22);
    EEPROM_Write(0xFF, myPlayer); // writes new top player
    bestPlayer = myPlayer;
    // eeprom_write_byte((uint8_t*)46, highscore);
  }
}

void sendInGame() {
  LCD_init();
  LCD_ClearScreen();
  LCD_DisplayString(1, "Score");
  LCD_DisplayString(21, "Player: ");
  LCD_Cursor(28);
  LCD_WriteData(myPlayer);
  LCD_Cursor(30);
  LCD_Cursor(1);
  LCD_WriteData('0' + score);
}

void EEPROM_RESET() {
  EEPROM_Write(0x01, 0x00);
  EEPROM_Write(0x00, 0x00);
}

int main(void) {
  DDRA = 0xFF;
  PORTD = 0x00;
  DDRB = 0xFF;
  PORTB = 0x00;
  DDRC = 0xFF;
  PORTC = 0x00;
  DDRD = 0xC0;
  PORTD = 0x7F;
  static task task1;
  static task task2;
  task * tasks[] = {
    & task1,
    & task2
  };
  const unsigned short numTasks = sizeof(tasks) / sizeof(task * );
  /**
  TASKS INITIALIZATIONS
  **/
  //TASK 1 (INPUT)
  task1.state = 0; //Task initial state.
  task1.period = 10; //Task Period.
  task1.elapsedTime = 0; //Task current elapsed time.
  task1.TickFct = & tick; //Function pointer for the tick.
  //TASK 2 (FALLING)
  task2.state = 0;
  task2.period = 20;
  task2.elapsedTime = 0;
  task2.TickFct = & fall_tick;

  mousePos = 7;
  srand(time(NULL));
  TimerSet(speed);
  TimerOn();
  // EEPROM_RESET();
  sendMenu();
  unsigned short i; // Scheduler for-loop iterator
  playerX = 1;
  playerY = 0;
  // PORTB = 0x10;
  m_state = START;
  // PORTA = 0xFF;
  while (1) {
    if (gameState == 0x00) { //game not going on rn
      PORTA = 0xFF;
      PORTB = 0x00;
      // sendMenu();
      menu_tick();
    }
    if (gameState == 0x01) { //playing
      displayLEDMatrix();
      for (i = 0; i < numTasks; i++) {
        if (tasks[i]-> elapsedTime == tasks[i]-> period) {
          tasks[i]-> state = tasks[i] -> TickFct(tasks[i]-> state);
          tasks[i]-> elapsedTime = 0;
        }
        tasks[i]-> elapsedTime += 1;
      }
      if (BUTTON3) { // SOFT RESET
        gameState = 0x00;
        score = 0;
        ROWS[0] = 0;
        COLUMNS[0] = 0;
        ROWS[1] = 0;
        COLUMNS[1] = 0;
        playerY = 0;
        mousePos = 7;
        // LCD_ClearScreen();
        sendMenu();
        for (i = 0; i < numTasks; i++ ){
          tasks[i]->state = 0;
        }
        // score = 0;
        // ROWS[0] = 0;
        // COLUMNS[0] = 0;
        // ROWS[1] = 0;
        // COLUMNS[1] = 0;
      }
      while (!TimerFlag);
      TimerFlag = 0;
      // moves = 3;
    }
  }
  return 0;
}
