/*****************************************************************************************

ommpc(One More Music Player Client) - A Music Player Daemon client targetted for the gp2x

Copyright (C) 2007 - Tim Temple(codertimt@gmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*****************************************************************************************/

#include "commandFactory.h"
#include "threadParms.h"
#include <SDL.h>
#include <iostream>



#define DELAY 800000
using namespace std;
CommandFactory::CommandFactory(mpd_Connection* mpd)
: m_timer(0)
, m_next(false)
, m_prev(false)
, m_start(false)
, m_select(false)
, m_play(false)
, m_prevDir(false)
, m_rand(false)
, m_append(false)
, m_mpd(mpd)
, m_setVol(false)
, m_delayCommand(false)
{

}

/*CMD_PLAY_PAUSE=1, CMD_STOP, CMD_PREV, CMD_NEXT, CMD_FF, CMD_RW,
  CMD_UP, CMD_DOWN, CMD_VOL_UP, CMD_VOL_DOWN,  
  CMD_ADD_TO_PL, CMD_APPEND_PL, CMD_NEW_PL, CMD_LOAD_PL, CMD_DEL_FROM_PL, CMD_IMMEDIATE_PLAY, 
  CMD_SELECT_MODE, CMD_TOGGLE_VIEW, CMD_SHOW_MENU
  CMD_MODE_RANDOM, CMD_MODE_REPEAT, CMD_QUIT */
int CommandFactory::getCommand(bool keysHeld[], int curMode, int& repeatDelay, bool popupVisible, int volume, long delayTime)
{
	int command = 0;
	
	if(repeatDelay == 1 || delayTime > DELAY) {
		//common commands
		if (keysHeld[SDLK_ESCAPE])
			command = CMD_DETACH;	
		else if (keysHeld[GP2X_VK_UP] || keysHeld[SDLK_UP]||keysHeld[SDLK_k])
			command = CMD_UP;	
		else if (keysHeld[GP2X_VK_DOWN] || keysHeld[SDLK_DOWN]||keysHeld[SDLK_j])
			command = CMD_DOWN;
		else if (keysHeld[GP2X_VK_LEFT] || keysHeld[SDLK_UP]||keysHeld[SDLK_LEFT])
			command = CMD_LEFT;	
		else if (keysHeld[GP2X_VK_RIGHT] || keysHeld[SDLK_RIGHT]||keysHeld[SDLK_RIGHT])
			command = CMD_RIGHT;
		else if (keysHeld[GP2X_VK_VOL_UP] || keysHeld[SDLK_0])
			command = CMD_VOL_UP;
		else if (keysHeld[GP2X_VK_VOL_DOWN] || keysHeld[SDLK_9])
			command = CMD_VOL_DOWN;
		else if (keysHeld[GP2X_VK_SELECT] || keysHeld[SDLK_m]) {
			if(!m_delayCommand) {
				if(delayTime >= DELAY) {
					command	= CMD_TOGGLE_VIEW;
					m_select = false;
					m_delayCommand = true;
				} else if(delayTime < DELAY){
					m_select = true;
				}
			}
		}
		else if (keysHeld[GP2X_VK_START] || keysHeld[SDLK_c]) {
			if(!m_delayCommand) {
				if(delayTime >= DELAY*4) {
					command	= CMD_TOGGLE_SCREEN;
					m_start = false;
					m_delayCommand = true;
				} else if(delayTime < DELAY*4){
					m_start = true;
				}
			}
		}
		else if (keysHeld[GP2X_VK_FB] || keysHeld[SDLK_p]) {
			if(popupVisible) {
					command = CMD_POP_SELECT;
			} else {
				if(!m_delayCommand) {
					if(delayTime >= DELAY) {
						command	= CMD_PAUSE;
						m_play = false;
						m_delayCommand = true;
					} else if(delayTime < DELAY){
						m_play = true;
					}
				}
			}
		}
		else {
			if(popupVisible) {
				if (keysHeld[GP2X_VK_FX] || keysHeld[SDLK_s])
					command = CMD_POP_CANCEL;
			} else {
				switch(curMode) {
					case 0:
						{ //song browser
							if (keysHeld[GP2X_VK_FA] || keysHeld[SDLK_i])
								command = CMD_ADD_TO_PL;
							else if (keysHeld[GP2X_VK_FX] || keysHeld[SDLK_s]) {
								if(!m_delayCommand) {
									if(delayTime >= DELAY) {
										command	= CMD_STOP;
										m_prevDir = false;
										m_delayCommand = true;
									} else if(delayTime < DELAY){
										m_prevDir = true;
									}
								}
							}
							else {
								if (!keysHeld[GP2X_VK_FB]) {
									if(m_play) {
										command	= CMD_IMMEDIATE_PLAY;
										m_play = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_START]) {
									if(m_start) {
										command	= CMD_SHOW_MENU;
										m_start = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_FX]) {
									if(m_prevDir) {
										command = CMD_PREV_DIR;
										m_prevDir = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_SELECT]) {
									if(m_select) {
										command = CMD_TOGGLE_MODE;
										m_select = false;
									}
									m_delayCommand = false;
								}
							}
						}
						break;
					case 1:
						{ //playlist
							if (keysHeld[GP2X_VK_FY])
								command	= CMD_MOVE_IN_PL;
							else if (keysHeld[GP2X_VK_FX])
								command = CMD_STOP;
							else if (keysHeld[GP2X_VK_FR] || keysHeld[SDLK_n]) {
								if(delayTime > DELAY) {
									if(volume != 0)
										m_volume = volume;
									m_setVol = true;
									//mpd_sendSetvolCommand(m_mpd, -100);
									//mpd_finishCommand(m_mpd);
									command = CMD_FF;
									m_next = false;
								}
								else
									m_next = true;
							} else if (keysHeld[GP2X_VK_FL] || keysHeld[SDLK_b]) {
								if(delayTime > DELAY) {
									if(volume != 0)
										m_volume = volume;
									m_setVol = true;
									//mpd_sendSetvolCommand(m_mpd, -100);
									//mpd_finishCommand(m_mpd);
									command = CMD_RW;	
									m_prev = false;
								}
								else
									m_prev = true;
							} else if (keysHeld[GP2X_VK_FA] || keysHeld[SDLK_s]) {
								if(!m_delayCommand) {
									if(delayTime >= DELAY) {
										command	= CMD_DEL_FROM_PL;
										m_rand = false;
										m_delayCommand = true;
									} else if(delayTime < DELAY){
										m_rand = true;
									}
								}
							} else if (keysHeld[SDLK_r]) {
								command = CMD_MODE_RANDOM;
							}  else if (keysHeld[SDLK_t]) {
								command = CMD_MODE_REPEAT;
							} else { 
								if (!keysHeld[GP2X_VK_FB]) {
									if(m_play) {
										command	= CMD_PLAY_PAUSE;
										m_play = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_FA]) {
									if(m_rand) {
										command	= CMD_RAND_RPT;
										m_rand = false;
									}
									m_delayCommand = false;
								}
								if(!keysHeld[GP2X_VK_FR] && !keysHeld[SDLK_n]) {
									if(m_next) {
										command = CMD_NEXT;
										m_next = false;
									} else if(m_setVol){
									//	mpd_sendSetvolCommand(m_mpd, m_volume);
									//	mpd_finishCommand(m_mpd);
										m_setVol = false;
									}
								}
								if (!keysHeld[GP2X_VK_FL] && !keysHeld[SDLK_b]) {
									if(m_prev) {
										command = CMD_PREV;
										m_prev = false;
									} else if(m_setVol){
								//		mpd_sendSetvolCommand(m_mpd, m_volume);
								//		mpd_finishCommand(m_mpd);
										m_setVol = false;
									}
								}
								if (!keysHeld[GP2X_VK_START] && !keysHeld[SDLK_c]) {
									if(m_start) {
										command	= CMD_SHOW_MENU;
										m_start = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_SELECT]) {
									if(m_select) {
										command = CMD_TOGGLE_MODE;
										m_select = false;
									}
									m_delayCommand = false;
								}
							}
						}
						break;
					case 2: 
						{ //playlist browser
							if (keysHeld[GP2X_VK_FX])
								command = CMD_STOP;
							else if (keysHeld[GP2X_VK_FA] || keysHeld[SDLK_s]) {
								if(!m_delayCommand) {
									if(delayTime >= DELAY) {
										command	= CMD_DEL_PL;
										m_append = false;
										m_delayCommand = true;
									} else if(delayTime < DELAY){
										m_append = true;
									}
								}
							}
							else { 
								if (!keysHeld[GP2X_VK_FB]) {
									if(m_play) {
										command	= CMD_LOAD_PL;
										m_play = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_START] && !keysHeld[SDLK_c]) {
									if(m_start) {
										command	= CMD_SHOW_MENU;
										m_start = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_SELECT]) {
									if(m_select) {
										command = CMD_TOGGLE_MODE;
										m_select = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_FA]) {
									if(m_append) {
										command	= CMD_APPEND_PL;
										m_append = false;
									}
									m_delayCommand = false;
								}
							}
						}
						break;
					case 3:
						{ //bookmark browser
							if (keysHeld[GP2X_VK_FX])
								command = CMD_STOP;
							else if (keysHeld[GP2X_VK_FA] || keysHeld[SDLK_s]) {
								if(!m_delayCommand) {
									if(delayTime >= DELAY) {
										command	= CMD_DEL_BKMRK;
										m_append = false;
										m_delayCommand = true;
									} else if(delayTime < DELAY){
										m_append = true;
									}
								}
							}
							else { 
								if (!keysHeld[GP2X_VK_FB]) {
									if(m_play) {
										command	= CMD_LOAD_BKMRK;
										m_play = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_START] && !keysHeld[SDLK_c]) {
									if(m_start) {
										command	= CMD_SHOW_MENU;
										m_start = false;
									}
									m_delayCommand = false;
								}
								if (!keysHeld[GP2X_VK_SELECT]) {
									if(m_select) {
										command = CMD_TOGGLE_MODE;
										m_select = false;
									}
									m_delayCommand = false;
								}
							}

						}
						break;
					default:
						command = 0;
				}
			}
		}
	}
	return command;
}


int CommandFactory::getCommandWhileLocked(bool keysHeld[], int curMode, int& repeatDelay, bool popupVisible, long delayTime)
{
	int command = 0;
	
	if(repeatDelay == 1 || delayTime > DELAY) {
		if (keysHeld[GP2X_VK_START] || keysHeld[SDLK_c]) {
			if(!m_delayCommand) {
				if(delayTime >= DELAY*4) {
					command	= CMD_TOGGLE_SCREEN;
					m_start = false;
					m_delayCommand = true;
				} else if(delayTime < DELAY*4){
					m_start = true;
				}
			}
		} else {
			if (!keysHeld[GP2X_VK_START] && !keysHeld[SDLK_c]) {
				if(m_start) {
					m_start = false;
				}
				m_delayCommand = false;
			}
		}
	}

	return command;
}
