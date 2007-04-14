#ifndef __NOWPLAYING_H__
#define __NOWPLAYING_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "libmpdclient.h"
#include "config.h"
#include "playlist.h"

class NowPlaying
{
public:
	NowPlaying(mpd_Connection* mpd, SDL_mutex* lock,  SDL_Surface* screen, Config& config, SDL_Rect& rect, Playlist& playlist);
	
	void updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus);
	void draw();
protected:
	
	mpd_Connection* m_mpd;
	SDL_mutex* m_lock;
	SDL_Surface* m_screen;
	SDL_Rect m_destRect;
	SDL_Rect& m_clearRect;
	Config& m_config;
	TTF_Font* m_font;
	Playlist& m_playlist;
	std::string m_name;
	int m_pos;
	int m_inc;
	int m_origY;
	int m_delayCnt;

	int m_nowPlaying;
};

#endif
