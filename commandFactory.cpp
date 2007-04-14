#include "commandFactory.h"

#include <SDL.h>
#include <iostream>

#define DELAY 10
using namespace std;
CommandFactory::CommandFactory()
: m_timer(0)
, m_next(false)
, m_prev(false)
{

}

/*CMD_PLAY_PAUSE=1, CMD_STOP, CMD_PREV, CMD_NEXT, CMD_FF, CMD_RW,
  CMD_UP, CMD_DOWN, CMD_VOL_UP, CMD_VOL_DOWN,  
  CMD_ADD_TO_PL, CMD_APPEND_PL, CMD_NEW_PL, CMD_LOAD_PL, CMD_DEL_FROM_PL, CMD_IMMEDIATE_PLAY, 
  CMD_SELECT_MODE, CMD_TOGGLE_VIEW, 
  CMD_MODE_RANDOM, CMD_MODE_REPEAT, CMD_QUIT */
int CommandFactory::getCommand(bool keysHeld[], int curMode, int& repeatDelay)
{
	int command = 0;
	if(repeatDelay == 1 || repeatDelay > DELAY) {
		//common commands
		if (keysHeld[SDLK_ESCAPE])
			command = CMD_QUIT;	
		else if (keysHeld[SDLK_s])
			command = CMD_STOP;
		else if (keysHeld[SDLK_UP]||keysHeld[SDLK_k])
			command = CMD_UP;	
		else if (keysHeld[SDLK_DOWN]||keysHeld[SDLK_j])
			command = CMD_DOWN;
		else if (keysHeld[SDLK_0])
			command = CMD_VOL_UP;
		else if (keysHeld[SDLK_9])
			command = CMD_VOL_DOWN;
		else if (keysHeld[SDLK_v])
			command	= CMD_TOGGLE_VIEW;
		else if (keysHeld[SDLK_m])
			command	= CMD_TOGGLE_MODE;
		else {
			switch(curMode) {
				case 0:
					{ //song browser
						if (keysHeld[SDLK_RETURN] || keysHeld[SDLK_p])
							command = CMD_IMMEDIATE_PLAY;
						else if (keysHeld[SDLK_i])
							command = CMD_ADD_TO_PL;
					}
					break;
				case 1:
					{ //playlist
						if (keysHeld[SDLK_RETURN] || keysHeld[SDLK_p] || keysHeld[SDLK_SPACE])
							command = CMD_PLAY_PAUSE;
						else if (keysHeld[SDLK_d])
							command = CMD_DEL_FROM_PL;
						else if (keysHeld[SDLK_n]) {
							if(repeatDelay > DELAY) {
								command = CMD_FF;
								m_next = false;
							}
							else
								m_next = true;
						} else if (keysHeld[SDLK_b]) {
							if(repeatDelay > DELAY) {
								command = CMD_RW;	
								m_prev = false;
							}
							else
								m_prev = true;
						} else if (keysHeld[SDLK_r]) {
						   command = CMD_MODE_RANDOM;
						}  else if (keysHeld[SDLK_t]) {
						   command = CMD_MODE_REPEAT;
						}  else if (keysHeld[SDLK_w]) {
						   command = CMD_SAVE_PL;
						} else { 
							if(!keysHeld[SDLK_n]) {
								if(m_next) {
									command = CMD_NEXT;
									m_next = false;
								}
							}
							if (!keysHeld[SDLK_b]) {
								if(m_prev) {
									command = CMD_PREV;
									m_prev = false;
								}
							}
						}
					}
					break;
				case 2: 
					{ //playlist browser
						if (keysHeld[SDLK_RETURN] || keysHeld[SDLK_p])
							command = CMD_LOAD_PL;
						if (keysHeld[SDLK_a])
							command = CMD_APPEND_PL;

					}
					break;
				default:
					command = 0;
			}
		}
	}
	return command;
}
