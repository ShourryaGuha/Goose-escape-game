//gooseEscapeGamePlay.cpp

#include <iostream>
#include <cmath>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

extern Console out;
	

//Print the game world
  
void printGameBoard(int gameBoard[MAX_COL][MAX_ROW])
{
	for (int col = 0; col < MAX_COL; col++)
	{
        for (int row = 0; row < MAX_ROW; row++)
		{
			int x_location_on_board = col;
			int y_location_on_board = row;
			if(gameBoard[col][row] == SHALL_NOT_PASS && row != MAX_ROW - 1)
			    terminal_put(x_location_on_board,y_location_on_board,WALL_CHAR);
			if(gameBoard[col][row] == WINNER)
				terminal_put(x_location_on_board,y_location_on_board,WIN_CHAR);
			if(gameBoard[col][row] == HEALTH_REGEN)
				terminal_put(x_location_on_board,y_location_on_board,
							REGEN_ICON);
        }
    }
    
    //Abbreviation for Player Health
    terminal_put(1, MAX_ROW-1, PLAYER_ABBREV);
    terminal_put(2, MAX_ROW-1, HEALTH_ABBREV);
    
    //printing health bar 
    for (int count = 0; count < MAX_HEALTH; count++)
	{
    		terminal_put(count + STARTING_HEALTH_INDEX, MAX_ROW-1, HEALTH_ICON);
	}
    
}

//when the goose captures the player
bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}

//	Player has won function
bool playerWon(Actor const & player, int & winnerCol, int & winnerRow)
{
	int xLoc = player.get_x();
    int yLoc = player.get_y();
    
    /* 8 locations around the winner position which when the player 
	coincides with the player will win */
    bool topLeft = (xLoc == winnerCol-1 && yLoc == winnerRow-1);
    bool top = (xLoc == winnerCol && yLoc == winnerRow-1);
    bool topRight = (xLoc == winnerCol+1 && yLoc == winnerRow-1); 
    bool left = (xLoc == winnerCol-1 && yLoc == winnerRow);
    bool right = (xLoc == winnerCol+1 && yLoc == winnerRow);
    bool botLeft = (xLoc == winnerCol-1 && yLoc == winnerRow+1);
    bool bottom = (xLoc == winnerCol && yLoc == winnerRow+1);
    bool botRight = (xLoc == winnerCol+1 && yLoc == winnerRow+1);
    
    return (topLeft || top || topRight || left || right ||
            botLeft || bottom || botRight);
}


//Move the player to a new location based on the user input.  
void movePlayer(int key, Actor & player, int gameBoard[MAX_COL][MAX_ROW])
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
        
    int newLocationX = player.get_x() + xMove;
    int newLocationY = player.get_y() + yMove;
    
    if (player.can_move(xMove, yMove) 
      && gameBoard[newLocationX][newLocationY] != SHALL_NOT_PASS)
        player.update_location(xMove, yMove);
}

//Function for gosse to follow the player
void moveGoose(int key, Actor & monster, Actor & player, 
				int gameBoard[MAX_COL][MAX_ROW])
{
	int yMove = 0, xMove = 0;
	
	if (!captured( player, monster))
	{
		
		if (key == TK_UP)
    	{
	    	if(player.get_y() > monster.get_y())
    	   		yMove = 1;
    		else
    			yMove = -1;
		}
		else if (key == TK_DOWN)
    	{
	    	if(player.get_y() < monster.get_y())
    	    	yMove = -1;
    		else
   				yMove = 1; 
   		}
    	else if (key == TK_LEFT)
   		{
			if(player.get_x() > monster.get_x())
    	    	xMove = 1;
    		else
    			xMove = -1;    
		}
		else if (key == TK_RIGHT)
  	  	{
	    	if(player.get_x() < monster.get_x())
    	    	xMove = -1;
    		else
    			xMove = 1; 
		}
	} 
	
    int newLocationX = monster.get_x() + xMove;
    int newLocationY = monster.get_y() + yMove;
    
    if (monster.can_move(xMove, yMove) 
      && gameBoard[newLocationX][newLocationY] != SHALL_NOT_PASS 
	  && gameBoard[newLocationX][newLocationY] != WINNER)
        monster.update_location(xMove, yMove);
}

// Allocates a random winner location every time the game starts 
void randomWinnerLocation(int gameBoard[MAX_COL][MAX_ROW], int &winnerCol, 
							int &winnerRow)
{
	
	int randX = 0;
	int randY = 0;
	do {
		randX = rand() % MAX_COL;
		randY = rand() % (MAX_ROW - 1);
	} while (gameBoard[randX][randY] == SHALL_NOT_PASS);
	
	gameBoard[randX][randY] = WINNER;
	
	winnerCol = randX;
	winnerRow = randY;
	
}

// Randomizes the placement of the wall horizontally or vertically
void randomWallPlacement(int gameBoard[MAX_COL][MAX_ROW], Actor const & player,  Actor const & monster) 
{
	//gives 1 or 0 for horizontal or vertical wall
	int isHoriz = rand() % ZERO_OR_ONE_RANDOM;  
	
	if (isHoriz == 1)
	{
		// horizontal wall implementation
		int wallRow = 0;
		do 
		{
			wallRow = (rand() % (MAX_ROW - 2)) + 1;
		} while(wallRow == player.get_y() ||  
				wallRow == monster.get_y());
				
		int wallColStart = rand() % (MAX_COL - WALL_LENGTH_COL);
		
		for(int col = wallColStart; col < WALL_LENGTH_COL + wallColStart; col++)
		{
			gameBoard[col][wallRow] = SHALL_NOT_PASS;
		}
		
	} 
	else 
	{
		// vertical wall implementation
		
			int wallCol = 0;
			
		do 
		{
			wallCol = rand() % MAX_COL; 
		} while(wallCol == player.get_x() ||  
				wallCol == monster.get_x());
						
		int wallRowStart = rand() % (MAX_ROW - 1 - WALL_LENGTH_ROW);
		
		for(int row = wallRowStart; row < WALL_LENGTH_ROW + wallRowStart; row++)
		{
			gameBoard[wallCol][row] = SHALL_NOT_PASS;
		}
		
	}
	
}

//Randomizes placement of health regeneration powerup every time the game starts
void randomHealthRegenLocation(int gameBoard[MAX_COL][MAX_ROW], int & healthCol, 
							int & healthRow)
{
	
	int randX = 0;
	int randY = 0;
	do 
	{
		randX = rand() % MAX_COL;
		randY = rand() % MAX_ROW - 1;
	} while (gameBoard[randX][randY] == SHALL_NOT_PASS || 
			gameBoard[randX][randY] == WINNER);
	
	gameBoard[randX][randY] = HEALTH_REGEN;
	
	healthCol = randX;
	healthRow = randY;
	
}

// Checks if player got the powerup and returns appropriate value
bool regenerated(Actor const & player, int & healthCol, int & healthRow)
{
    return (player.get_x() == healthCol 
         && player.get_y() == healthRow);
}

//Regenerates player health completely 
void regenerateHealth(int & healthAmount)
{
	for (int count = 0; count < MAX_HEALTH; count++)
	{
    		terminal_put(count + STARTING_HEALTH_INDEX, MAX_ROW-1, HEALTH_ICON);
	}
	//healthAmount = 3;
	
}

