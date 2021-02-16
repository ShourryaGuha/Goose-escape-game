// gooseEscapeMain.cpp

#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

Console out;

int main()
{
	srand(time(NULL));
	
	//Set up the window.  
    terminal_open();
  	terminal_set(SETUP_MESSAGE);

 	//creating player and monster objects
	Actor player(PLAYER_CHAR, PLAYER_STARTING_POS_X , PLAYER_STARTING_POS_Y);  
	Actor monster(MONSTER_CHAR, MONSTER_STARTING_POS_X ,MONSTER_STARTING_POS_Y);
	    
    int gameBoard[MAX_COL][MAX_ROW] = {0};
    
	for (int col = 0; col < MAX_COL; col++)
	{
        for (int row = 0; row < MAX_ROW; row++)
		{
            gameBoard[col][row] = EMPTY;
        }
    }
	
	//function call to randomly create a wall either horizontally or vertically
	randomWallPlacement(gameBoard, player, monster);
    
    // allocation of a row where player health is displayed
    for (int col = 0; col < MAX_COL; col++)
	{
        int row = MAX_ROW-1;
        gameBoard[col][row] = SHALL_NOT_PASS;
    }
    
    int winnerRow = 0, winnerCol = 0;
    int healthRegenRow = 0, healthRegenCol = 0;
    
    /*function call to randomly place winner location each time the game is 
	played*/
	randomWinnerLocation(gameBoard, winnerCol, winnerRow);
	
	/*function call to randomly place power up location to regenerate health 
	each time the game is played*/
	randomHealthRegenLocation(gameBoard, healthRegenCol, healthRegenRow);
    
    // Call the function to print the game board
	printGameBoard(gameBoard);
  	
	// Printing the instructions
    out.writeLine("Escape the Goose! " + monster.get_location_string());
	out.writeLine("Use the arrow keys to move");
	out.writeLine("If the goose catches you, you lose!");
	out.writeLine("Be careful! Sometimes the goose can jump through walls!");


    int keyEntered = TK_A; // can be any valid value that is not ESCAPE or CLOSE
    int countRegenerate = 0;//Power up is one time use only, so a count for that
    int health_regen = player.get_health();
    
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
        && player.get_health() != 0 && !playerWon(player, winnerCol, winnerRow))
	{
	    // get player key press
	    keyEntered = terminal_read();
		
		// Move player and goose
        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
    	    movePlayer( keyEntered, player, gameBoard);
			moveGoose( keyEntered, monster, player, gameBoard);	    
        }
        
        //Reduce player health by 1 if captured
        if(captured( player, monster)){
        	player.reduce_health_actor();
        	player.reinitialize_location(player.get_x(), player.get_y(), 
										PLAYER_STARTING_POS_X, 
										PLAYER_STARTING_POS_Y);
        	monster.reinitialize_location(monster.get_x(), monster.get_y(), 
										MONSTER_STARTING_POS_X, 
										MONSTER_STARTING_POS_Y);
		}
		
		//Full health for player if it gets to the ppower up
		if(regenerated(player, healthRegenCol, healthRegenRow) 
			&& countRegenerate == 0)
		{
			regenerateHealth(health_regen);
			player.set_health();
			countRegenerate++;  //one time use only
		}
  	}

    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
        out.writeLine("Game has ended");
    	if(player.get_health() == 0)   //player loses all its health
    		out.writeLine("You have died!");
    	else if(playerWon(player, winnerCol, winnerRow))
    		out.writeLine("You have won!");
        // Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }

	//game is done, close it  
    terminal_close();
}
