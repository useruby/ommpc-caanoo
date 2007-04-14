#include "popup.h"
#include "threadParms.h"
#include "commandFactory.h"

#include <iostream>
#include <stdexcept>

using namespace std;

Popup::Popup(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect,
				int skipVal, int numPerScreen)
: Scroller(mpd, screen, TTF_OpenFont("Vera.ttf", 10), rect, skipVal, numPerScreen)
, m_config(config)
, m_pos(0)
, m_type(0)
{
	m_borderRect.x = m_clearRect.x-1;
	m_borderRect.y = m_clearRect.y-1;
	m_borderRect.h = m_clearRect.h+2;
	m_borderRect.w = m_clearRect.w+2;
}

int Popup::selectedAction()
{
	int action;
	if(m_type == 0) 
		action= m_listing[m_curItemNum].second;
	else 
		action = -1;

	return action;
}

string Popup::selectedText()
{
	string r;
	if(m_type == 0) 
		r = m_listing[m_curItemNum].first;
	else 
		r = "";

	return r;
}

void Popup::setItemsText(Scroller::listing_t& items)
{
	m_listing.clear();
	m_listing = items;
/*
	for(vector<string>::iterator vIter = items.begin();
	vIter != items.end() ;
	++vIter) {
		m_items.push_back((*vIter));
		cout << "pusing " << (*vIter) << endl;
	}
*/
	m_lastItemNum = m_listing.size()-1;
	m_type = 0;
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
	
	m_borderRect.x = m_clearRect.x-1;
	m_borderRect.y = m_clearRect.y-1;
	m_borderRect.h = m_clearRect.h+2;
	m_borderRect.w = m_clearRect.w+2;

}

int Popup::processCommand(int command) 
{
	int rCommand = 0;

	if(Scroller::processCommand(command)) {
		//
	} else if(command == CMD_PLAY_PAUSE) {
		rCommand = CMD_POP_SELECT;				
	}

	return rCommand;

}

void Popup::draw()
{
	switch(m_type) {
		case POPUP_LIST: 
			drawSelectList();
		break;
		case POPUP_CONFIRM:
//			drawConfirm();
		break;
		case POPUP_MENU:
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
	SDL_FillRect(m_screen, &m_borderRect, SDL_MapRGB(m_screen->format, 255,255,255));
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));

    SDL_Color color = { 255,255,255, 0 };
	SDL_Surface *sText;
	sText = TTF_RenderText_Solid(m_font, m_name.c_str(), color);
	SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
	SDL_FreeSurface(sText);
	m_destRect.y += m_skipVal*2;

	Scroller::draw();
	m_destRect.y = m_origY;

}
