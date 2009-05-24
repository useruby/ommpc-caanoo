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

#include "menu.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "config.h"
#include "guiPos.h"
#include "songDb.h"
#include "keyboard.h"
#include "playlist.h"
#include <iostream>
#include <stdexcept>
#include <SDL_image.h>

using namespace std;

Menu::Menu(mpd_Connection* mpd, SDL_Surface* screen, SDL_Surface* bg, TTF_Font* font, 
				SDL_Rect& rect,	Config& config, int skipVal, int numPerScreen, SongDb& songdb, Keyboard& kb, Playlist& pl)
: Scroller(mpd, screen, bg, font, rect, config, skipVal, numPerScreen-1)
, m_songDb(songdb)
, m_keyboard(kb)
, m_pl(pl)
, m_menu1Active(0)
, m_menu2Active(0)
, m_view(0)
{
	m_config.getItemAsColor("sk_main_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_main_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
    
	string skinName = m_config.getItem("skin");
	m_drawIcons =  m_config.getItemAsNum("drawIcons");

	initItems(CMD_SHOW_MENU);
}

void Menu::initItems(int command)
{
	m_buttons.clear();
	switch(command) {
		case CMD_SHOW_MENU:
			{
				m_view = 0;
				int xOffset = 0;
				int yOffset = 12;
				MenuButton butt0("Now Playing");
				butt0.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_NP);
				m_buttons.push_back(butt0);

				MenuButton butt1("Current Playlist");
				xOffset += 64;
				butt1.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_PL);
				m_buttons.push_back(butt1);

				MenuButton butt2("Music Library");
				xOffset += 64;
				butt2.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_LIB);
				m_buttons.push_back(butt2);

				MenuButton butt3("Playlists");
				xOffset += 64;
				butt3.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_PLS);
				m_buttons.push_back(butt3);
				MenuButton butt4("Bookmarks");
				xOffset += 64;
				butt4.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_BKMRKS);
				m_buttons.push_back(butt4);

				m_numPerRow = 5;
				xOffset = 0;
				yOffset += 64;
				MenuButton butt5("Settings");
				butt5.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_MENU_SETTINGS);
				m_buttons.push_back(butt5);

				m_buttons[m_menu1Active].active(true);
			}
			break;
		case CMD_MENU_SETTINGS:
			{
				m_view = 1;
				int xOffset = 0;
				int yOffset = 12;
				MenuButton butt0("Main Menu");
				butt0.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_MENU);
				m_buttons.push_back(butt0);

				xOffset += 64;
				MenuButton butt1("Update MPD Db");
				butt1.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_MPD_UPDATE);
				m_buttons.push_back(butt1);

				MenuButton butt2("Options");
				xOffset += 64;
				butt2.init(m_config, m_destRect.x+xOffset,m_destRect.y+yOffset, "np", CMD_SHOW_OPTIONS);
				m_buttons.push_back(butt2);

				m_buttons[m_menu2Active].active(true);
			}
			break;
		}
	
}

void Menu::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus, bool updatingSongDb)
{
	if(mpdStatusChanged & STATE_CHG) { 
		m_curState = mpdStatus->state;
		m_refresh = true;
	}
}

int Menu::processCommand(int command, GuiPos& guiPos)
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
		switch(command) {
			case CMD_DOWN:
				m_buttons[m_menu1Active].active(false);
				if(m_menu1Active + m_numPerRow < m_buttons.size())
					m_menu1Active += m_numPerRow;
				else if(m_buttons.size() > m_numPerRow)
					m_menu1Active = m_buttons.size() -1;
				m_buttons[m_menu1Active].active(true);
				m_refresh = true;
				break;
			case CMD_UP:
				m_buttons[m_menu1Active].active(false);
				if(m_menu1Active - m_numPerRow > 0)
					m_menu1Active -= m_numPerRow;
				else
					m_menu1Active = 0;
				m_buttons[m_menu1Active].active(true);
				m_refresh = true;
				break;
			case CMD_LEFT:
				m_buttons[m_menu1Active].active(false);
				if(m_menu1Active -1 >= 0)
					m_menu1Active--;
				m_buttons[m_menu1Active].active(true);
				m_refresh = true;
				break;
			case CMD_RIGHT:
				m_buttons[m_menu1Active].active(false);
				if(m_menu1Active+1 < m_buttons.size())
					m_menu1Active++;
				m_buttons[m_menu1Active].active(true);
				m_refresh = true;
				break;
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

void Menu::draw(bool forceRefresh, long timePerFrame, bool inBack)
{
	if(forceRefresh || (!inBack && m_refresh)) {
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_clearRect);
		SDL_BlitSurface(m_bg, &m_clearRect, m_screen, &m_clearRect );
		SDL_Surface *sText;
		for(vector<MenuButton>::iterator bIter = m_buttons.begin();
			bIter != m_buttons.end();
			++bIter) {
			(*bIter).draw(m_screen, m_bg, forceRefresh||m_refresh);
		}
		m_refresh = false;
	}
	
}
