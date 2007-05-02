#ifndef __ALBUMART_H__
#define __ALBUMART_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "threadParms.h"
#include "libmpdclient.h"
#include "config.h"

class AlbumArt
{
public:
	AlbumArt(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect,
					artThreadParms_t& m_artParms);
	void updateStatus(int mpdStatusChanged, mpd_Status * mpdStatus,
						int rtmpdStatusChanged, mpd_Status* rtmpdStatus);
	void draw(bool forceUpdate);
protected:
	
	mpd_Connection* m_mpd;
	SDL_Surface* m_screen;
	SDL_Rect m_destRect;
	SDL_Rect& m_clearRect;
	Config& m_config;
	int m_pos;
	int m_inc;
	int m_origY;

	bool m_update;
	artThreadParms_t& m_artParms;
	
	//colors
	SDL_Color m_backColor;
};

#endif
