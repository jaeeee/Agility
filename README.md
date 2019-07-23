# EE/CS120B Final Project - Agility
> https://drive.google.com/drive/u/0/folders/1aaQPAo0M4R1NvSNEk8jFg70kN8HeKf0L

# About
Agility is a game that tests your reflexes and motor skills. I programmed this game onto an Atmega-1284 microcontroller using AVR tools. The game handles bit manipulation and collisions. The game is won when you have received 9 points, but when you are hit by a projectile, you will lose the game.

# Team
* Daniel Kwong

# Tasks
The game is a SynchSM which has 2 tasks and 1 menu FSM.
* `MENU` - serves as a menu in which the player can enter in and out of games, and allows the player to set their game avatar. 
* `DISPLAY` - doubles as an input and output task. Allows processing of input whilst simultaneously outputting to LCD and LED matrix.
* `FALLING_STATES` - handles the "AI" of the game, and calculates collisions and contact with the player.

# Libraries
I used some pre-built libraries that were either given to us from our labs or were native to C.
* stdlib.h
* timer.h
* scheduler.h
* bit.h
* io.h
* eeprom
* util/delay.h

# Build-upons
* Game Logic
* 8x8 LED Matrix 
* Custom characters / player look customization
* EEPROM

