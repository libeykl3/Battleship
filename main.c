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

int main(void)
{
	// Variable declarations
	Stats players[2] = {{0, 0, 0, 0.0}, {0, 0, 0, 0.0}};
	Cell playerOneGameBoard[ROWS][COLS], playerTwoGameBoard[ROWS][COLS];
	Coordinate target, targetTemp, targetOrigin, targetAI;
	WaterCraft ship[NUM_OF_SHIPS] = {{'c', 5, "Carrier"}, {'b', 4, "Battleship"}, {'r', 3, "Cruiser"}, {'s', 3, "Submarine"}, {'d', 2, "Destroyer"}};
	Boolean huntMode = TRUE, targetMode = FALSE, flipper = TRUE, cardinals[4] = {TRUE, TRUE, TRUE, TRUE}, hasAShipSunk = FALSE;
	short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2}, {5, 4, 3, 3, 2}}, player  = 0, shot = 0;
	int option = 0, north = 0, south = 0, east = 0, west = 0, i = 0, counter = 1;
	char shipSymbol = '\0'; 
	FILE *outStream = NULL;

	// Prep log file
	outStream = fopen (LOG_FILE_NAME, "w");

	// Make sure random is random
	srand((unsigned int) time(NULL));

	// Opening Screen
	welcomeScreen();
	systemMessage("  Press enter to continue!");
	system("cls");

	// Prep players' game boards
	initializeGameBoard(playerOneGameBoard);
	initializeGameBoard(playerTwoGameBoard);

	// Random or manual ship placement
	printf(" -> Make your selection:\n");
	printf(" -> 1. Manually\n");
	printf(" -> 2. Randomly\n");
	printf(" -> Enter Option: ");
	scanf("%d", &option);
	
	switch (option) 
	{
		case 1: manuallyPlaceShipsOnGameBoard(playerOneGameBoard, ship); // Manual placement
	            break;
		case 2: randomlyPlaceShipsOnGameBoard(playerOneGameBoard, ship); // Random placement
				break;
	}

	// Randomly place computer's board
	randomlyPlaceShipsOnGameBoard(playerTwoGameBoard, ship);
	printf(" -> The computer's fleet has been assembled.\n");

	// Randomly choose who goes first
	player = getRandomNumber(0, 1);
	printf(" -> Player %d has been randomly selected to go first.\n", player + 1);
	systemMessage(" -> Press enter to continue!\n");
	system("cls");

	// Main Loop
	while (TRUE) 
	{

		// Write to battleship_log.txt
		fprintf(outStream, "Player %d's turn.\n", player + 1);

		// Go between player 1 and player 2
		switch (player) 
		{

			case PLAYER_ONE: 
				
				// Show player 2's board
				printf(" -> Player 2's Board:\n");
				printGameBoard(playerTwoGameBoard, FALSE);
				printf(" -> PLAYER 1'S TURN\n");
				

				// Enter a target to attack
				do 
				{
					target = getTarget(); 
					shot = checkShot(playerTwoGameBoard, target);
					
					// Shows invalid target
					if (shot == -1) 
					{
						printf (" -> Try inputting another target!\n");
					}

				} while (shot == -1);

				// Saves a character in that cell
				shipSymbol = playerTwoGameBoard[target.row][target.column].symbol;
				break;

			case PLAYER_TWO: 

				// AI

				// Show player 1's board
				printf(" -> Player 1's Board:\n");
				printGameBoard (playerOneGameBoard, TRUE);
				printf(" -> COMPUTER'S TURN\n");

				// If ship already sunk, reset values and try again
				if (hasAShipSunk) 
				{
					hasAShipSunk = FALSE;
					targetMode = FALSE;
					huntMode = TRUE;
				}
				
				// If computer's random shot is a hit
				if (targetMode) 
				{
					target = targetAI;

					do 
					{
						if (cardinals[NORTH]) 
						{
							target.row = north;
						} 
						else if (cardinals[SOUTH]) 
						{
							target.row = south;
						} 
						else if (cardinals[WEST]) 
						{
							target.column = west;
						} 
						else if (cardinals[EAST]) 
						{
							target.column = east;
						} 
						else if (!cardinals[NORTH] && !cardinals[SOUTH] && !cardinals[WEST]  && !cardinals[EAST]  && !hasAShipSunk) 
						{
							// All directions are false
							
							// Set target value back to original value
							target = targetOrigin;
							targetTemp = target;

							// Make sure to check a cell over the cells around it
							north = target.row - counter;
							targetTemp.row = north;

							// Check the value from north
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && north >= 0) 
							{
								cardinals[NORTH] = TRUE;
							}

							targetTemp = target;
							south = target.row + counter;
							targetTemp.row = south;

							// Check the value from south
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && south <= 9) 
							{
								cardinals[SOUTH] = TRUE;
							}

							targetTemp = target;
							west = target.column - counter;
							targetTemp.column = west;

							// Check the value from west
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && west >= 0) 
							{
								cardinals[WEST] = TRUE;
							}

							targetTemp = target;
							east = target.column + counter;
							targetTemp.column = east;

							// Check the value from east
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && east <= 9) 
							{
								cardinals[EAST] = TRUE;
							}

							// Number of cells to check away from target cell
							counter++;
						} 
						else  
						{
							// When all are not met, returns back to start again at random
							targetMode = FALSE;
							huntMode = TRUE;
							break;
						}
						
						// Hit or miss?
						shot = checkShot (playerOneGameBoard, target);

					} 
					while (shot == -1 && targetMode == TRUE);

					// Flip value of directions to check
					if (shot == 1 && huntMode == FALSE) 
					{
						for (i = 0; i < 4; i++) 
						{
							if (flipper == FALSE)
							{
								cardinals[i] = FALSE;
							}
							if (cardinals[i] == flipper) 
							{
								flipper = FALSE;
							}
						}
					} else 
					{
						for (i = 0; i < 4; i++) 
						{
							if (flipper == TRUE && cardinals[i] != FALSE) 
							{
								cardinals[i] = FALSE;
								break;
							}
						}
					}

					// Reset the flipping variable
					flipper = TRUE;
				}

				// Start player 2's turn. Once there's a hit, Hunt Mode.
				if (huntMode) 
				{	

					// Reset values
					counter = 1;
					flipper = TRUE;
					for (i = 0; i < 4; i++)
					{
						cardinals[i] = TRUE;
					}

					// Get random cell to target
					do 
					{
						target.row = getRandomNumber(0, 9);
						target.column = getRandomNumber(0, 9);

						// Check if target was already a hit or miss
						shot = checkShot (playerOneGameBoard, target);
					} while (shot == -1);

					// Hit initiates value to be saved
					if (shot == 1) targetOrigin = target;
				}

				// Address the shot values - 1 is a hit, 0 is a miss, and -1 is already hit or missed
				if (shot == 1) 
				{
					// If all directions are false and no sunken ship, reset target to original target
					if (!cardinals[NORTH] && !cardinals[SOUTH] && !cardinals[WEST]  && !cardinals[EAST]  && !hasAShipSunk) 
					{ 
						target = targetOrigin; 
					}

					// Do not use Hunt Mode when using Target Mode
					huntMode = FALSE;
					targetMode = TRUE;
					targetAI = target;

					// Get values of adjacent cells. Make sure they're not out of bounds.
					if (cardinals[NORTH] == TRUE) 
					{
						north = (targetAI.row - 1);
						checkBoundsOfCardinal(cardinals, north, NORTH);
						targetTemp = target;
						targetTemp.row = north;
						if (checkShot(playerOneGameBoard, targetTemp) == -1)
						{
							cardinals[NORTH] = FALSE;
						}
					}
					
					if (cardinals[SOUTH] == TRUE) 
					{
						south = targetAI.row + 1;
						checkBoundsOfCardinal (cardinals, south, SOUTH);
						targetTemp = target;
						targetTemp.row = south;
						if (checkShot(playerOneGameBoard, targetTemp) == -1)
						{
							cardinals[SOUTH] = FALSE;
						}
					}

					if (cardinals[WEST] == TRUE) 
					{
						west  = targetAI.column - 1;
						checkBoundsOfCardinal(cardinals, west, WEST);
						targetTemp = target;
						targetTemp.column = west;
						if (checkShot(playerOneGameBoard, targetTemp) == -1)
						{
							cardinals[WEST] = FALSE;
						}
					}

					if (cardinals[EAST] == TRUE) 
					{
						east  = targetAI.column + 1;
						checkBoundsOfCardinal(cardinals, east, EAST);
						targetTemp = target;
						targetTemp.column = east;
						if (checkShot(playerOneGameBoard, targetTemp) == -1)
						{
							cardinals[EAST] = FALSE;
						}
					}
				}

				// Save character of target cell
				shipSymbol = playerOneGameBoard[target.row][target.column].symbol;
				break;
		}

		// Tell player hit or miss
		if (shot == 1) // Hit
		{
			printf(" -> %d, %d is a hit!\n", target.row, target.column);

			// Write to battleship_log.txt
			fprintf(outStream, "%d, %d is a hit!\n", target.row, target.column);

			// Record number of hits
			players[player].numHits++;

			// Did the ship sink?
			if (player == 1)
			{
				hasAShipSunk = checkSunkShip (sunkShip, !player, shipSymbol, outStream);
			}
			else
			{
				checkSunkShip (sunkShip, !player, shipSymbol, outStream);
			}

		} 
		else // Miss
		{
			printf(" -> %d, %d is a miss!\n", target.row, target.column);

			// Write to battleship_log.txt
			fprintf(outStream, "%d, %d is a miss!\n", target.row, target.column);
			players[player].numMisses++;
		}
		
		if (player == 0) //If Player 1 then update player 2's gameboard
		{
			updateGameBoard (playerTwoGameBoard, target);
		}
		else //If Player 2 then update player 1's gameboard
		{
			updateGameBoard (playerOneGameBoard, target);
		}

		// Winner based on hits?
		if (isWinner (players, player)) 
		{
			printf ("\n> Player %d wins!\n", player + 1);

			// Write to battleship_log.txt
			fprintf(outStream, "\n ->>> Player %d wins! <<<-\n", player + 1);
			break;
		}

		systemMessage(" -> Press enter to continue!\n");

		// Switch players
		player = !player;	

		system("cls");
	}

	// Print results and stats. Also found in log file.
	players[0].totalShots = players[0].numHits + players[0].numMisses;
	players[0].hitMissRatio = ((double) players[0].numHits/(double) players[0].numMisses) * 100;
	players[1].totalShots = players[1].numHits + players[1].numMisses;
	players[1].hitMissRatio = ((double) players[1].numHits/(double) players[1].numMisses) * 100;
	fprintf(outStream, "+===================================================\n");
	fprintf(outStream, "|                    PLAYER STATS                   \n");
	fprintf(outStream, "+---------------------------------------------------\n");
	fprintf(outStream, "| PLAYER 1 : %d hits                                \n", players[0].numHits);
	fprintf(outStream, "|            %d misses                              \n", players[0].numMisses);
	fprintf(outStream, "|            %d total shots                         \n", players[0].totalShots);
	fprintf(outStream, "|            %.2lf%% hit/miss ratio                 \n", players[0].hitMissRatio);
	fprintf(outStream, "| COMPUTER : %d hits                                \n", players[1].numHits);
	fprintf(outStream, "|            %d misses                              \n", players[1].numMisses);
	fprintf(outStream, "|            %d total shots                         \n", players[1].totalShots);
	fprintf(outStream, "|            %.2lf%% hit/miss ratio                 \n", players[1].hitMissRatio);
	fprintf(outStream, "+===================================================");

	fclose (outStream);

	return 0;
}
