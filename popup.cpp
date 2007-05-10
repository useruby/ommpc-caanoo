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

#include "popup.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "gp2xregs.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <dirent.h>
using namespace std;

Popup::Popup(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect,
				int skipVal, int numPerScreen, GP2XRegs& gp2xRegs)
: Scroller(mpd, screen, TTF_OpenFont("Vera.ttf", 10), rect, config, skipVal, numPerScreen)
, m_pos(0)
, m_type(0)
, m_gp2xRegs(gp2xRegs)
{
	m_borderRect.x = m_clearRect.x-1;
	m_borderRect.y = m_clearRect.y-1;
	m_borderRect.h = m_clearRect.h+2;
	m_borderRect.w = m_clearRect.w+2;

	m_config.getItemAsColor("sk_popup_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
	m_config.getItemAsColor("sk_popup_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_popup_curItemBackColor", m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b);
	m_config.getItemAsColor("sk_popup_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
	m_config.getItemAsColor("sk_popup_borderColor", m_borderColor.r, m_borderColor.g, m_borderColor.b);
}

int Popup::selectedAction()
{
	int action;
	if(m_type == POPUP_LIST || m_type == POPUP_MENU || m_type == POPUP_OPTIONS) 
		action= m_listing[m_curItemNum].second;
	else 
		action = -1;

	return action;
}

string Popup::selectedText()
{
	string r;
	if(m_type == POPUP_LIST || m_type == POPUP_MENU || m_type == POPUP_OPTIONS) 
		r = m_listing[m_curItemNum].first;
	else 
		r = "";

	return r;
}

void Popup::setItemsText(Scroller::listing_t& items, int type)
{
	m_listing.clear();
	m_listing = items;
	m_curItemNum = 0;	
	m_lastItemNum = m_listing.size()-1;
	m_type = type;
	if(type == POPUP_OPTIONS)
		setOptionsText();
}

void Popup::setOptionsText()
{
	vector<string> curOption;
	vector<string>::iterator curIter;
	for(int i=50; i<=220; i+=5) {
		ostringstream mhz;
		mhz << i;
		curOption.push_back(mhz.str());
	}
	
	m_optionsText.push_back(curOption);
	curOption.clear();
	curOption.push_back("true");
	curOption.push_back("false");
	m_optionsText.push_back(curOption);
	curOption.clear();

	DIR * udir = opendir("skins/");

	if(udir != NULL) {
		struct dirent * dirent = readdir(udir);

		bool done = false;
		while(dirent != NULL) {
			if(dirent->d_name[0] != '.') {
				string ename = "skins/";
				ename += dirent->d_name;
				struct stat s;

				if (stat(ename.c_str(), &s) < 0) {
					string msg = "error calling stat on ";
					msg += ename;
					throw runtime_error(msg.c_str());
				}

				if (S_ISDIR(s.st_mode)) 
					curOption.push_back(dirent->d_name);
			}
			dirent = readdir(udir);
		}
	}
	m_optionsText.push_back(curOption);
	curOption.clear();

	m_optionsIters.clear();	
	curIter = find(m_optionsText[0].begin(), m_optionsText[0].end(), m_config.getItem("cpuSpeed"));
	m_optionsIters.push_back(curIter);
	curIter = find(m_optionsText[1].begin(), m_optionsText[1].end(), m_config.getItem("showAlbumArt"));
	m_optionsIters.push_back(curIter);
	curIter = find(m_optionsText[2].begin(), m_optionsText[2].end(), m_config.getItem("skin"));
	m_optionsIters.push_back(curIter);
}

void Popup::saveOptions()
{
	string oldSkin = m_config.getItem("skin");
	string oldSpeed = m_config.getItem("cpuSpeed");
	string oldSaa = m_config.getItem("showAlbumArt");
	
	int itemNum = 0;
	for(listing_t::iterator vIter = m_listing.begin();
		vIter != m_listing.end(); 
		++vIter) {
		if((*vIter).second > 20 && itemNum < m_optionsIters.size()) {
			std::string name;
			if(itemNum == 0)
				name = "cpuSpeed";
			else if(itemNum == 1)
				name = "showAlbumArt";
			else if(itemNum == 2)
				name = "skin";
			m_config.setItem(name, (*m_optionsIters[itemNum]));
		}
		++itemNum;
	}

	
	m_config.saveConfigFile();

	if(oldSpeed != m_config.getItem("cpuSpeed")) {
	//set cpu clock
	mpd_sendPauseCommand(m_mpd, 1);
	mpd_finishCommand(m_mpd);
	m_gp2xRegs.setClock(m_config.getItemAsNum("cpuSpeed"));
	mpd_sendPauseCommand(m_mpd, 0);
	mpd_finishCommand(m_mpd);
	}
	
	m_config.init();
	//reload skin file to pick up on any skin changes/album art flage changes.
	if(oldSkin == m_config.getItem("skin") || (oldSaa == m_config.getItem("showAlbumArt")))
		; //
		
}

void Popup::setTitle(std::string name)
{
	m_name = name;
}

void Popup::setSize(SDL_Rect& rect)
{

	m_clearRect = rect;
	m_destRect.x = rect.x+5;
	m_destRect.y = rect.y+5;
	m_origY = m_destRect.y;
	
	m_curItemClearRect = m_destRect;
	m_borderRect.x = m_clearRect.x-1;
	m_borderRect.y = m_clearRect.y-1;
	m_borderRect.h = m_clearRect.h+2;
	m_borderRect.w = m_clearRect.w+2;

}

int Popup::processCommand(int command) 
{
	int rCommand = command;

	if(Scroller::processCommand(command)) {
		rCommand = 0;
	} else if(m_type == POPUP_OPTIONS) {
		if(command == CMD_LEFT) {
			if (m_optionsIters[m_curItemNum] == m_optionsText[m_curItemNum].begin())
				m_optionsIters[m_curItemNum] = m_optionsText[m_curItemNum].end() - 1;
			else
				m_optionsIters[m_curItemNum]--;
		} else if(command == CMD_RIGHT) {
			m_optionsIters[m_curItemNum]++;
			if (m_optionsIters[m_curItemNum] == m_optionsText[m_curItemNum].end())
				m_optionsIters[m_curItemNum] = m_optionsText[m_curItemNum].begin();
		}
	}

	return rCommand;

}

void Popup::draw()
{
	switch(m_type) {
		case POPUP_LIST: 
		case POPUP_MENU:
		case POPUP_OPTIONS:
			drawSelectList();
		break;
		case POPUP_CONFIRM:
//			drawConfirm();
		break;
//			drawMenu();
		break;
		default:
		break;
	}
}

void Popup::drawSelectList()
{
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_borderRect);
	SDL_FillRect(m_screen, &m_borderRect, SDL_MapRGB(m_screen->format, m_borderColor.r, m_borderColor.g, m_borderColor.b));
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

	SDL_Surface *sText;
	sText = TTF_RenderText_Blended(m_font, m_name.c_str(), m_itemColor);
	SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
	SDL_FreeSurface(sText);
	m_destRect.y += m_skipVal*2;
	m_curItemClearRect.y += m_skipVal*2;

	if(m_type == POPUP_OPTIONS) {
		m_selectedOptions.clear();
		m_selectedOptions.push_back((*m_optionsIters[0]));
		m_selectedOptions.push_back((*m_optionsIters[1]));
		m_selectedOptions.push_back((*m_optionsIters[2]));
		Scroller::draw(m_selectedOptions);
	}
	else 
		Scroller::draw();

}
