#ifndef __SCROLLER_H__
#define __SCROLLER_H__

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "libmpdclient.h"

class Config;

class Scroller
{
public:
	typedef std::vector<std::pair<std::string, int> >listing_t;
	Scroller(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, 
				SDL_Rect& rect, Config& config, int skipVal, int numPerScreen);

	bool processCommand(int command);
	void draw();
	void draw(std::vector<std::string>& selectedOptions);

	int skipVal();
protected:
	mpd_Connection* m_mpd;

	listing_t m_listing;
	
	SDL_Surface* m_screen;
	TTF_Font* m_font;
	SDL_Rect m_destRect;
	SDL_Rect& m_clearRect;
	SDL_Rect m_curItemClearRect;
	Config& m_config;
	
	int m_origY;
	int m_topItemNum;
	int m_curItemNum;
	int m_lastItemNum;
	std::string m_curItemName;
	int m_curItemType;
	
	int m_skipVal;
	int m_numPerScreen;

	//colors
	SDL_Color m_backColor;
	SDL_Color m_itemColor;
	SDL_Color m_curItemBackColor;
	SDL_Color m_curItemColor;
	
};

#endif
