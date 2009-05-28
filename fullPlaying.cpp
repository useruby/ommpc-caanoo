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

#include "fullPlaying.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "config.h"
#include "guiPos.h"
#include "keyboard.h"
#include "playlist.h"
#include <iostream>
#include <stdexcept>
#include <SDL_image.h>

using namespace std;

FullPlaying::Menu(mpd_Connection* mpd, SDL_Surface* screen, SDL_Surface* bg, TTF_Font* font, 
				SDL_Rect& rect,	Config& config, int skipVal, int numPerScreen, SongDb& songdb, Keyboard& kb, Playlist& pl)
: Scroller(mpd, screen, bg, font, rect, config, skipVal, numPerScreen-1)
, m_keyboard(kb)
, m_menu1Active(0)
, m_menu2Active(0)
, m_view(0)
{
	m_config.getItemAsColor("sk_main_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_main_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
   
}

void FullPlaying::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus, bool updatingSongDb)
{
	if(mpdStatusChanged & STATE_CHG) { 
		m_curState = mpdStatus->state;
		m_refresh = true;
	}
}

int FullPlaying::processCommand(int command, GuiPos& guiPos)
{
	int rCommand = command;
	bool done = false;
	int curItem = 0;
	if(command > 0) {
		for(vector<MenuButton>::iterator bIter = m_buttons.begin();
			bIter != m_buttons.end() && !done;
			++bIter) {
			rCommand = (*bIter).processCommand(command, guiPos);
			if(rCommand != command) {
				done = true;
				if(command == CMD_CLICK) {
					if(m_view == 0) {
						m_buttons[m_menu1Active].active(false);
						m_menu1Active = curItem;
					} else {
						m_buttons[m_menu2Active].active(false);
						m_menu2Active = curItem;
					}
				}
			}
			++curItem;
		}
		
		switch(rCommand) {
			case CMD_SHOW_MENU:
			case CMD_MENU_SETTINGS:
				initItems(rCommand);
				m_refresh = true;
				break;
		}
	}
	return rCommand;
}

void FullPlaying::draw(bool forceRefresh, long timePerFrame, bool inBack)
{
	if(forceRefresh || (!inBack && m_refresh)) {
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_clearRect);
		SDL_BlitSurface(m_bg, &m_clearRect, m_screen, &m_clearRect );
		SDL_Surface *sText;
		for(vector<FullPlayingButton>::iterator bIter = m_buttons.begin();
			bIter != m_buttons.end();
			++bIter) {
			(*bIter).draw(m_screen, m_bg, forceRefresh||m_refresh);
		}
		m_refresh = false;
	}
	
}
