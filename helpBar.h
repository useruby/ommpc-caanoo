#ifndef __HELPBAR_H__
#define __HELPBAR_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "libmpdclient.h"
#include "config.h"

class HelpBar
{
public:
	HelpBar(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect);
	void updateStatus(bool mpdStatusChanged, mpd_Status * mpdStatus);	
	void draw(int curMode, bool forceUpdate);
protected:
	
	mpd_Connection* m_mpd;
	SDL_Surface* m_screen;
	SDL_Rect m_destRect;
	SDL_Rect& m_clearRect;
	Config& m_config;
	TTF_Font* m_font;
	int m_pos;
	int m_inc;
	int m_origY;
		
	std::vector<std::vector<std::string> >m_modeCmdText;
};

#endif
