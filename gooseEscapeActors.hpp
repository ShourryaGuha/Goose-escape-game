//gooseEscapeActors.hpp

#ifndef GOOSE_ESCAPE_ACTORS
#define GOOSE_ESCAPE_ACTORS
#include <cmath>
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"

const int MAX_ROW_ACTOR = 21;
const int STARTING_HEALTH_LOC = 4;
const int PLAYER_HEALTH = 3;  //Max player health

class Actor
{
  private:
    int actorChar;      
    int location_x, location_y;
    int health;  // new private data member for player health
	
  public:
    Actor()
    {
        actorChar = int('A');
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        put_actor();
        health = PLAYER_HEALTH;  //Initialise health of player
    }

    Actor(char initPlayerChar, int x0, int y0)
    {
        change_char(initPlayerChar);
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        update_location(x0,y0);
        health =  PLAYER_HEALTH;
    }
    
    int get_health() const  // Accessor for player health
    {
    	return health;
	}
	
	void set_health()
	{
		health = PLAYER_HEALTH;
	}
    
    int get_x() const
    {
        return location_x;
    }
    
    int get_y() const
    {
        return location_y;
    }
        
    string get_location_string() const
    {
        char buffer[80];
        itoa(location_x,buffer,10);
        string formatted_location = "(" + string(buffer) + ",";
        itoa(location_y,buffer,10);
        formatted_location += string(buffer) + ")";
        return formatted_location;
    }
    
    void change_char(char new_actor_char)
    {
        actorChar = min(int('~'),max(int(new_actor_char),int(' ')));
    }

    bool can_move(int delta_x, int delta_y) const
    {
        int new_x = location_x + delta_x;
        int new_y = location_y + delta_y;

        return new_x >= MIN_BOARD_X && new_x <= MAX_BOARD_X
          && new_y >= MIN_BOARD_Y && new_y <= MAX_BOARD_Y;
    }

    void update_location(int delta_x, int delta_y)
    {
        if (can_move(delta_x, delta_y))
        {
            terminal_clear_area(location_x, location_y, 1, 1);
            location_x += delta_x; 
            location_y += delta_y;
            put_actor();
        }
    }
    
    void put_actor() const
    {
        terminal_put(location_x, location_y, actorChar);
        terminal_refresh();
    }
    
    //Function to reduce player health by 1 if caught
    int reduce_health_actor()
	{
    	terminal_clear_area(STARTING_HEALTH_LOC + health - 1, 
							MAX_ROW_ACTOR-1, 1, 1);	
    	health -= 1;
   	}
	
	/*Function to reinitialize player and goose location everytime player is 
	caught */
	void reinitialize_location(int current_loc_x, int current_loc_y, 
								int new_loc_x, int new_loc_y)
	{
		update_location(new_loc_x - current_loc_x, new_loc_y - current_loc_y);
	}
    
};
#endif
