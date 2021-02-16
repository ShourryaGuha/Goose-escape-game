// gooseEscapeGamePlay.hpp

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"

//This file is all about the game world.  


const int EMPTY = 0;
const int SHALL_NOT_PASS = 1;
const int WINNER = 2;
const int HEALTH_REGEN = 3;

// Placement of walls, player, monster, health icon and winner location

const int MAX_ROW = 21; //one extra row for showing health of player
const int MAX_COL = 80;
const int WALL_LENGTH_COL = 20;  //horizontal wall length
const int WALL_LENGTH_ROW = 7;   //vertical wall length

const int PLAYER_STARTING_POS_X = 10;
const int PLAYER_STARTING_POS_Y = 10;
const int MONSTER_STARTING_POS_X = 70;
const int MONSTER_STARTING_POS_Y = 17;

const int MAX_HEALTH = 3; 
const int STARTING_HEALTH_INDEX = 4;

const int ZERO_OR_ONE_RANDOM = 2;

//display characters
const int PLAYER_CHAR = int('@');
const int MONSTER_CHAR = int('G');
const int WALL_CHAR = int('o');
const int WIN_CHAR = int('%'); //Win location
const int PLAYER_ABBREV = int('P');
const int HEALTH_ABBREV = int('H');
const int HEALTH_ICON = int('*');  //health
const int REGEN_ICON = int('$'); //Powerup 

// print the game board function prototype

void printGameBoard(int gameBoard[MAX_COL][MAX_ROW]);

// Do something when the goose captures the player
bool captured(Actor const & player, Actor const & monster);

//Move player function
void movePlayer(int key, Actor & player, int gameBoard[MAX_COL][MAX_ROW]);

// Function prototype for win function
bool playerWon(Actor const & player, int & winnerCol, int & winnerRow);

// Function for monster to follow the actor wherever it goes
void moveGoose(int key, Actor & monster, Actor & player, 
				int gameBoard[MAX_COL][MAX_ROW]);

// Allocates a random winner location every time the game starts 
void randomWinnerLocation(int gameBoard[MAX_COL][MAX_ROW], int & winnerCol, 
						int & winnerRow);

// Randomizes the placement of the wall
void randomWallPlacement(int gameBoard[MAX_COL][MAX_ROW], Actor const & player, 
						Actor const & monster);

//Randomizes placement of health regeneration powerup every time the game starts 
void randomHealthRegenLocation(int gameBoard[MAX_COL][MAX_ROW], int & healthCol, 
							int & healthRow);
// Checks if player got the powerup
bool regenerated(Actor const & player, int & healthCol, int & healthRow);
//Regenerates player to full health
void regenerateHealth(int & healthAmount);
#endif
