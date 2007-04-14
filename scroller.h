#ifndef __SCROLLER_H__
#define __SCROLLER_H__

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "libmpdclient.h"

class Scroller
{
public:
	typedef std::vector<std::pair<std::string, int> >listing_t;
	Scroller(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, 
				SDL_Rect& rect,int skipVal, int numPerScreen);

	bool processCommand(int command);
	void draw();

protected:
	mpd_Connection* m_mpd;

	listing_t m_listing;
	
	SDL_Surface* m_screen;
	TTF_Font* m_font;
	SDL_Rect m_destRect;
	SDL_Rect& m_clearRect;
	
	int m_origY;
	int m_topItemNum;
	int m_curItemNum;
	int m_lastItemNum;
	std::string m_curItemName;
	int m_curItemType;
	
	int m_skipVal;
	int m_numPerScreen;
};

#endif
