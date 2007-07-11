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

#ifndef __COMMANDS_H__ 
#define __COMMANDS_H__

#include "libmpdclient.h"


	typedef enum {	CMD_PLAY_PAUSE=1, CMD_PAUSE, CMD_STOP, CMD_PREV, CMD_NEXT, CMD_FF, CMD_RW,
					CMD_UP, CMD_DOWN, CMD_VOL_UP, CMD_VOL_DOWN,  
					CMD_ADD_TO_PL, CMD_NEW_PL, CMD_APPEND_PL, CMD_LOAD_PL, CMD_DEL_PL,
					CMD_DEL_FROM_PL, CMD_MOVE_IN_PL, CMD_MPD_UPDATE,
					CMD_IMMEDIATE_PLAY, CMD_SAVE_PL, CMD_POP_SELECT, CMD_POP_CANCEL,
					CMD_TOGGLE_MODE, CMD_TOGGLE_VIEW, CMD_SHOW_MENU, 
					CMD_LAUNCH_APP, CMD_DETACH, CMD_TOGGLE_SCREEN, CMD_PREV_DIR,
					CMD_LOAD_BKMRK, CMD_SAVE_BKMRK, CMD_DEL_BKMRK,
					CMD_SHOW_OPTIONS, CMD_SAVE_OPTIONS, CMD_LEFT, CMD_RIGHT,
			   		CMD_MODE_RANDOM, CMD_MODE_REPEAT, CMD_RAND_RPT, CMD_QUIT,
					CMD_MPD_ADD_ALL } cmdTypes_t;

class CommandFactory
{
public:
					
	CommandFactory(mpd_Connection* mpd);
	int getCommand(bool keysHeld[], int curMode, int& timer, bool popupVisible, int vol, long delayTime);
	int getCommandWhileLocked(bool keysHeld[], int curMode, int& timer, bool popupVisible, long delayTime);

protected:
	int m_timer;	
	bool m_next;
	bool m_prev;
	bool m_start;
	bool m_select;
	bool m_play;
	bool m_prevDir;
	bool m_rand;
	bool m_append;
	mpd_Connection* m_mpd;

	int m_volume;
	bool m_setVol;
	bool m_delayCommand;
};

#endif
