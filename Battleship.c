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

#include "battleship.h"

/********************************************************************************************
* Function: welcomeScreen()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function displays the opening screen and rules.
* Input Parameters: N/A
* Returns: N/A
* Preconditions: The program has opened and is running.
* Postconditions: The user can hit enter to initiate the game.
********************************************************************************************/
void welcomeScreen(void) 
{
	printf("          _____       _    _    _            _    _      \n         | __  | ___ | |_ | |_ | | ___  ___ | |_ |_| ___ \n         | __ -|| .'||  _||  _|| || -_||_ -||   || || . |\n         |_____||__,||_|  |_|  |_||___||___||_|_||_||  _|\n                                                    |_|  \n");
	printf("------------------------------------------------------------------\n");
	printf("         Sink the computer's ships before it sinks yours!\n");
	printf("------------------------------------------------------------------\n\n");
	printf ("                        How to Play:\n");
	printf ("  This is a two player game where you are player 1 and the computer is\n");
	printf ("  player 2. You will be prompted initailly to either manually or randomly\n");
	printf ("  place your ships. These are the ships which can be placed:\n\n");
	printf ("  [c] Carrier - 5 cells  [b] Battleship - 4 cells  [r] Cruiser - 3 cells\n");
	printf ("  [s] Submarine - 3 cells  [d] Destroyer - 2 cells\n\n"); 
	printf ("  The game will start with a random selection of who gets to go first. It\n");
	printf ("  continues by each player trying to sink all of the other's ships.\n\n");
	printf ("  X - hit  O - miss\n\n");
	printf ("  The first player who is able to sink all of the other player's ships wins!\n\n");
}

/********************************************************************************************
* Function: initializeGameBoard()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function initializes the two gameboard arrays.
* Input Parameters: The game board array.
* Returns: N/A
* Preconditions: ROWS and COLS are defined as macros. The array exists.
* Postconditions: Array is initialized to water.
********************************************************************************************/
void initializeGameBoard(Cell gameBoard[][COLS])
{
	int i = 0, j = 0;

	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++) 
		{
			gameBoard[i][j].symbol = WATER;
			gameBoard[i][j].position.row = i;
			gameBoard[i][j].position.column = j;
		}
	}
}

/********************************************************************************************
* Function: printGameBoard()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function prints the game board to screen with ability to show/not show pegs.
* Input Parameters: The game board array and TRUE/FALSE for showing pegs.
* Returns: N/A
* Preconditions: The input parameters are defined.
* Postconditions: The game board is printed to the screen.
********************************************************************************************/
void printGameBoard(Cell gameBoard[][COLS], Boolean showPegs) 
{
	int i = 0, j = 0;

	printf("  0 1 2 3 4 5 6 7 8 9\n");

	for (i = 0; i < ROWS; i++)
	{
		printf("%d ", i);

		for (j = 0; j < COLS; j++)
		{
			if (showPegs == TRUE) 
			{
				printf("%c ", gameBoard[i][j].symbol);
			}
			else 
			{
				switch (gameBoard[i][j].symbol) 
				{
					case HIT: printf("%c ", HIT); break;
					case MISS: printf("%c ", MISS); break;
					case WATER: 
					default: printf("%c ", WATER); break;
				}
			}
		}
		putchar('\n');
	}
}

/********************************************************************************************
* Function: putShipOnGameBoard()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function allows for placement of the 5 types of ships.
* Input Parameters: The game board array, the ship, the position, and the direction.
* Returns: N/A
* Preconditions: The input parameters are defined.
* Postconditions: A ship is placed over a range of target cells.
********************************************************************************************/
void putShipOnGameBoard(Cell gameBoard[][COLS], WaterCraft ship, Coordinate position, int direction) 
{
	int i = ship.length - 1;

	for (i = 0; i < ship.length; i++) 
	{
		if (direction == HORIZONTAL) 
		{
			gameBoard[position.row][position.column + i].symbol = ship.symbol;
		}
		else // Vertical
		{
			gameBoard[position.row + i][position.column].symbol = ship.symbol;
		}
	}
}

/********************************************************************************************
* Function: manuallyPlaceShipsOnGameBoard()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function allows for a player to manually place ships.
* Input Parameters: The game board array and the ship array.
* Returns: N/A
* Preconditions: The input coordinates are correct (separate function to check this).
* Postconditions: Ships are placed to the game board.
********************************************************************************************/
void manuallyPlaceShipsOnGameBoard(Cell gameBoard[][COLS], WaterCraft ship[]) 
{
	char stringPosition[11] = "";
	int i = 0, j = 0;

	Coordinate position[5];
	Boolean isValid = FALSE;

	fflush(stdin);

	for (i = 0; i < NUM_OF_SHIPS; i++) 
	{
		while (TRUE) 
		{
			system ("cls");
			printGameBoard (gameBoard, TRUE);
			printf (" Enter %d cells to place the %s over (enter without spaces):\n", ship[i].length, ship[i].name);
			printf (" -> ");
			scanf ("%s", stringPosition);

			// Returns false if not successful
			if (convertStringtoPosition(position, stringPosition, ship[i].length)) 
			{
				isValid = TRUE;

				for (j = 0; j < ship[i].length; j++) 
				{
					if (gameBoard[position[j].row][position[j].column].symbol == WATER) 
					{
						gameBoard[position[j].row][position[j].column].symbol = ship[i].symbol;
					} 
					else 
					{
						isValid = FALSE;
						printf (" -> Invalid input!\n");

						if (j != 0)
							while (j >= 0) {
								gameBoard[position[j].row][position[j].column].symbol = WATER;
								j--;
							}
						break;
					}
				}
			} 
			else 
			{
				isValid = FALSE;
				printf (" -> Invalid input!\n");
			}
			if (isValid == TRUE) break;
		}
	}
}

/********************************************************************************************
* Function: randomlyPlaceShipsOnGameBoard()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function allows for random ship placement.
* Input Parameters: The game board array and the ship array.
* Returns: N/A
* Preconditions: The input parameters are defined.
* Postconditions: Ships are randomly placed on the game board.
********************************************************************************************/
void randomlyPlaceShipsOnGameBoard(Cell gameBoard[][COLS], WaterCraft ship[]) 
{
	Coordinate position;
	int direction = -1;
	int i = 0;

	for (i = 0; i < NUM_OF_SHIPS; i++) 
	{
		while (TRUE) 
		{
			direction = getRandomNumber(0, 1); // 0 - Horizontal, 1- Vertical
			position = generatePosition(direction, ship[i].length);

			if (isValidLocation (gameBoard, position, direction, ship[i].length)) break;
		}
		putShipOnGameBoard(gameBoard, ship[i], position, direction);
	}
}

/********************************************************************************************
* Function: updateGameBoard()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function updates the status of the game board (hit or miss).
* Input Parameters: The game board array and the target.
* Returns: N/A
* Preconditions: The input parameters are defined.
* Postconditions: The game board is updated properly.
********************************************************************************************/
void updateGameBoard(Cell gameBoard[][COLS], Coordinate target) 
{
	switch (gameBoard[target.row][target.column].symbol) 
	{
		// Miss 
		case WATER: 
			{
			gameBoard[target.row][target.column].symbol = MISS;
			break;
			}

		// Hit
		case CARRIER: 
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			{
			gameBoard[target.row][target.column].symbol = HIT;
			break;
			}

		case HIT:
		case MISS:
		default: break;	
	}	
}

/********************************************************************************************
* Function: checkBoundsOfCardinal()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function checks if the direction is valid.
* Input Parameters: The directions boolean, the bound, and the direction.
* Returns: N/A
* Preconditions: The input parameters are defined.
* Postconditions: It updates the directions array (cardinals[]).
********************************************************************************************/
void checkBoundsOfCardinal(Boolean cardinals[], int bound, int direction) 
{
	switch (direction) 
	{
		case NORTH: 
			
			if (bound < 0) 
			{
				cardinals[0] = FALSE;
			}
			else
			{
				cardinals[0] = TRUE;
				break;
			}

		case SOUTH:
			
			if (bound > 9) 
			{
				cardinals[1] = FALSE;
			}
			else   
			{
				cardinals[1] = TRUE;
				break;
			}
			

		case WEST:
			
			if (bound < 0) 
			{
				cardinals[2] = FALSE;
			}
			else
			{
				cardinals[2] = TRUE;
				break;
			}
			

		case EAST:
			
			if (bound > 9)  
			{
				cardinals[3] = FALSE;
			}
			else
			{
				cardinals[3] = TRUE;	
				break;
			}
			
	}
}

/********************************************************************************************
* Function: systemMessage()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function prints a message to the screen to be acted on.
* Input Parameters: A pointer to the string.
* Returns: N/A
* Preconditions: The input parameters are defined.
* Postconditions: The message is printed to screen.
********************************************************************************************/
void systemMessage(char *message)
{
	char ch = '\0';

	do 
	{
		printf ("%s", message);
	} while ((ch = getch()) != '\r');
}

/********************************************************************************************
* Function: checkSunkShip()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function checks if a ship has sunk by looking at the length not hit.
* Input Parameters: The sunkShip array, player, ship's symbol, and the output file.
* Returns: TRUE or FALSE
* Preconditions: The stream to the output file is created.
* Postconditions: Status of sunken/floating ship can be used.
********************************************************************************************/
Boolean checkSunkShip(short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE *stream) 
{
	Boolean sunk = FALSE;

	switch (shipSymbol) 
	{
		case CARRIER:    
			if (--sunkShip[player][0] == 0) 
			{
				printf(" -> Player %d's Carrier is sunk!\n", player + 1);

				// Write to battleship_log.txt
				fprintf(stream, "Player %d's Carrier is sunk!\n", player + 1);

				sunk = TRUE;
			}
			break;

		case BATTLESHIP: 
			if (--sunkShip[player][1] == 0) 
			{
				printf(" -> Player %d's Battleship is sunk!\n", player + 1);

				// Write to battleship_log.txt
				fprintf(stream, "Player %d's Battleship is sunk!\n", player + 1);

				sunk = TRUE;
			}
			break;

		case CRUISER:    
			if (--sunkShip[player][2] == 0) 
			{
				printf(" -> Player %d's Cruiser is sunk!\n", player + 1);

				// Write to battleship_log.txt
				fprintf(stream, "Player %d's Cruiser is sunk!\n", player + 1);

				sunk = TRUE;
			}
			break;

		case SUBMARINE:  
			if (--sunkShip[player][3] == 0) 
			{
				printf (" -> Player %d's Submarine is sunk!\n", player + 1);

				// Write to battleship_log.txt
				fprintf (stream, "Player %d's Submarine is sunk!\n", player + 1);

				sunk = TRUE;
			}
			break;

		case DESTROYER:  
			if (--sunkShip[player][4] == 0) 
			{
				printf (" -> Player %d's Destroyer is sunk!\n", player + 1);

				// Write to battleship_log.txt
				fprintf (stream, "Player %d's Destroyer is sunk!\n", player + 1);

				sunk = TRUE;
			}
			break;
	}
	return sunk;
}

/********************************************************************************************
* Function: isValidLocation()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function checks if the position, direction, and length is valid for placement.
* Input Parameters: The game board array, the position, the direction, and the length.
* Returns: TRUE or FALSE
* Preconditions: The input parameters are defined.
* Postconditions: States whether a ship can be placed validly.
********************************************************************************************/
Boolean isValidLocation(Cell gameBoard[][COLS], Coordinate position, int direction, int length) 
{
	int i = length - 1;
	Boolean isValid = TRUE;

	for (i = 0; isValid && i < length; i++) 
	{
		if (direction == HORIZONTAL) 
		{
			if (gameBoard[position.row][position.column + i].symbol != WATER && (position.column + i) < COLS)
			{
				isValid = FALSE;
			}
		} 
		else // Vertical
		{ 
			if (gameBoard [position.row + i][position.column].symbol != WATER && (position.row + i) < ROWS)
			{
				isValid = FALSE;
			}
		}
	}
	return isValid;
}


/********************************************************************************************
* Function: convertStringtoPosition()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function converts a string of coordinates into integers.
* Input Parameters: The position array, the pointer to stringPosition, and the length.
* Returns: flag
* Preconditions: Coordinate system is correct.
* Postconditions: Usable coordinates.
********************************************************************************************/
Boolean convertStringtoPosition(Coordinate position[], char *stringPosition, int length) 
{
	Boolean flag = TRUE;
	char temp = '\0';
	int i = 0, j = 0, k = 1;

	// Ship length good?
	if (strlen (stringPosition)/2 == length) 
	{
		// Loop through ship's length
		for (i = 0; i < length && flag; i++) 
		{
			// Each cell a digit?
			if (isdigit (stringPosition[j]) && isdigit (stringPosition[k])) 
			{
				position[i].row    = stringPosition[j] - '0';
				position[i].column = stringPosition[k] - '0'; 

				j += 2;
				k += 2;
			} 
			else 
			{
				flag = FALSE;
			}
		}
	} 
	else 
	{
		flag = FALSE;
	}
	return flag;
}

/********************************************************************************************
* Function: isWinner()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function determines a winner based on the 17 total pegs (hits) for each player.
* Input Parameters: The players array and player number.
* Returns: TRUE or FALSE
* Preconditions: The input parameters are defined.
* Postconditions: A winner is determined.
********************************************************************************************/
Boolean isWinner (Stats players[], int player) 
{
	Boolean isWin = FALSE;

	if (players[player].numHits == 17) 
	{
		isWin = TRUE;
	}

	return isWin;
}

/********************************************************************************************
* Function: generatePosition()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function generates a position based on direction and length no bigger than the boundries.
* Input Parameters: The direction and the length.
* Returns: Position
* Preconditions: The input parameters are defined.
* Postconditions: The position is defined.
********************************************************************************************/
Coordinate generatePosition(int direction, int length) 
{
	Coordinate position;

	if (direction == HORIZONTAL) 
	{
		position.row    = getRandomNumber (0, ROWS);
		position.column = getRandomNumber (0, COLS - length);
	}
	else // Vertical
	{
		position.row    = getRandomNumber (0, ROWS - length);
		position.column = getRandomNumber (0, COLS);
	}

	return position;
}

/********************************************************************************************
* Function: getTarget()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function reads row and column values from the player.
* Input Parameters: N/A
* Returns: target
* Preconditions: The input parameters are defined.
* Postconditions: The target coordinates are returned.
********************************************************************************************/
Coordinate getTarget(void) 
{
	Coordinate target;

	fflush(stdin);

	printf (" -> Enter Target Coordinates (separated by a space):\n");
	printf (" -> ");
	scanf ("%d %d", &target.row, &target.column);

	return target;
}

/********************************************************************************************
* Function: checkShot()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function checks whether the coordinates are a miss, hit, water, or watercraft.
* Input Parameters: The game board array and the target coordinates.
* Returns: Hit integer
* Preconditions: The input parameters are defined.
* Postconditions: The type of hit is determined.
********************************************************************************************/
short checkShot(Cell gameBoard[][COLS], Coordinate target)
{
	int hit = -2;

	switch (gameBoard[target.row][target.column].symbol) 
	{
		// Miss
		case WATER: 
			{
			hit = 0;
			break;
			}

		// Hit
		case CARRIER: 
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			{
			hit = 1;
			break;
			}

		case HIT:
		case MISS:
		default:
			{
			hit = -1;
			break;
			}
	}	

	return hit;
}

/********************************************************************************************
* Function: getRandomNumber()
* Date Created: 3/18/2015
* Date Last Modified: 4/6/2015
* Description: This function returns a rondom number between the highest and lowest numbers, including them.
* Input Parameters: The lowest and highest numbers.
* Returns: A random number
* Preconditions: The input parameters are defined.
* Postconditions: A random number is found between the two.
********************************************************************************************/
int getRandomNumber(int lowest, int highest) 
{
	if (lowest == 0)
	{
		return rand () % ++highest;
	}
	if (lowest > 0)
	{
		return rand () % ++highest + lowest;
	}
}
