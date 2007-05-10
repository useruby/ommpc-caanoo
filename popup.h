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

#ifndef __POPUP_H__
#define __POPUP_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "libmpdclient.h"
#include "config.h"
#include "playlist.h"
#include "scroller.h"

class GP2XRegs;

class Popup : public Scroller
{
public:
	typedef std::vector<std::vector<std::string> > optionsText_t;
	typedef std::vector<std::vector<std::string>::iterator > optionsTextIters_t;
	typedef enum {POPUP_LIST, POPUP_CONFIRM, POPUP_MENU, POPUP_OPTIONS}; // popup types
	typedef enum {POPUP_CANCEL, POPUP_SAVE_PL, POPUP_LAUNCH, POPUP_DETACH, 
				  POPUP_EXIT, POPUP_DO_SAVE_PL, POPUP_DO_LAUNCH, POPUP_SHOW_OPTIONS, 
				  POPUP_SAVE_OPTIONS, POPUP_MPD_UPDATE}; //popup action
	Popup(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect,
				int skipVal, int numPerScreen, GP2XRegs& gp2xregs);
	
	void setItemsText(Scroller::listing_t& items, int type);
	void setSize(SDL_Rect& rect);
	void setTitle(std::string name);
	std::string selectedText();
	void setOptionsText();
	void saveOptions();
	int selectedAction();
	int processCommand(int command);	
	void draw();
	void drawSelectList();
protected:
	
	GP2XRegs& m_gp2xRegs;
	std::string m_name;
	int m_pos;
	int m_delayCnt;

	SDL_Rect m_borderRect;
	SDL_Color m_borderColor;
	int m_type;

	//for options menu
	optionsText_t m_optionsText;
	optionsTextIters_t m_optionsIters;
	std::vector<int> m_selectedIndexes;
	std::vector<std::string> m_selectedOptions;
};

#endif
