#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
STORES THE CONSTANTS FOR THE GAME LOGIC, SO THINGS DON'T HAVE TO BE RETYPED.
THESE VALUES ARE TO REMAIN STATIC AND CONSISTENT.
**/

unsigned char ROW_VALUES[8] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };
unsigned char COLUMN_VALUES[8] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };
unsigned char FALLING_SEQUENCE[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

/**
CUSTOM PLAYER AVATARS FOR LCD DISPLAY
**/
unsigned char player1[8] = {
  0x04,
  0x04,
  0x0E,
  0x11,
  0x00,
  0x1F,
  0x04,
  0x04
};

unsigned char player2[8] = {
  0x11,
    0x0A,
    0x04,
    0x0E,
    0x00,
    0x0A,
    0x00,
    0x0A
};

unsigned char player3[8] = {
  0x04,
  0x04,
  0x11,
  0x00,
  0x1F,
  0x00,
  0x15,
  0x04
};

unsigned char player4[8] = {
  0x11,
  0x09,
  0x04,
  0x02,
  0x05,
  0x0A,
  0x11,
  0x00
};

#endif
