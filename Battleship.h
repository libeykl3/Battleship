/********************************************************************************************
* Programmer: Kyle Libey
* Class: CptS 121, Spring 2015; Lab Section 5
* Programming Assignment: PA #5 - Yahtzee
* Date: March 18, 2015
*
* Description: This program is the game of Battleship. It is comprised of two players, one
* of which is the computer. The game is played out like a normal game of Battleship where
* the first person to sink all of the other player's fleet wins. The game then outputs to
* a log file all of the moves for the game. For this programming assignment, I got some help
* from a friend who had already taken the class. In particular, I got assistance with some
* of the more advanced AI and array/enum type specifics.
********************************************************************************************/

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>


// Constant Macros
#define ROWS 10
#define COLS 10
#define LOG_FILE_NAME "battleship_log.txt"
#define NUM_OF_SHIPS 5
#define HORIZONTAL 0
#define VERTICAL 1
#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define WATER '~'
#define HIT 'X'
#define MISS 'O'
#define CARRIER 'c'
#define BATTLESHIP 'b'
#define CRUISER 'r'
#define SUBMARINE 's'
#define DESTROYER 'd'
#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3


// Enumerated Types
typedef enum 
{
	FALSE, TRUE
} Boolean;

typedef enum 
{
	CARRIER_L = 5, 
	BATTLESHIP_L = 4,
	CRUISER_L = 3, 
	SUBMARINE_L = 3,
	DESTROYER_L = 2
} ShipType;

// Structures

typedef struct watercraft 
{
	char symbol;
	short length;
	char *name;
} WaterCraft;

typedef struct stats 
{
	int numHits;
	int numMisses;
	int totalShots;
	double hitMissRatio;
} Stats;

typedef struct coordinate 
{
	int row;
	int column;
} Coordinate;

typedef struct cell 
{
	char symbol; 
	Coordinate position;
} Cell;


// Function Protypes
void welcomeScreen(void);
void initializeGameBoard(Cell gameBoard[][COLS]);
void printGameBoard(Cell gameBoard [][COLS], Boolean showPegs);
void putShipOnGameBoard(Cell gameBoard[][COLS], WaterCraft ship, Coordinate position, int direction);
void manuallyPlaceShipsOnGameBoard(Cell gameBoard[][COLS], WaterCraft ship[]);
void randomlyPlaceShipsOnGameBoard(Cell gameBoard[][COLS], WaterCraft ship[]);
void updateGameBoard(Cell gameBoard[][COLS], Coordinate target);
void checkBoundsOfCardinal(Boolean cardinals[], int bound, int direction);
void systemMessage(char *message);
Boolean checkSunkShip(short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE *stream);
Boolean isValidLocation(Cell gameBoard[][COLS], Coordinate position, int direction, int length);
Boolean convertStringtoPosition(Coordinate position[], char *stringPosition, int length);
Boolean isWinner(Stats players[], int player);
Coordinate generatePosition(int direction, int length);
Coordinate getTarget(void);
short checkShot(Cell gameBoard[][COLS], Coordinate target);
int getRandomNumber(int lowest, int highest);

#endif
