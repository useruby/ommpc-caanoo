#include "nowPlaying.h"
#include "threadParms.h"
#include <iostream>
#include <stdexcept>

using namespace std;

NowPlaying::NowPlaying(mpd_Connection* mpd, SDL_mutex* lock, SDL_Surface* screen, Config& config, SDL_Rect& rect, Playlist& pl)
: m_mpd(mpd)
, m_lock(lock)
, m_screen(screen)
, m_config(config)
, m_clearRect(rect)
, m_pos(0)
, m_playlist(pl)
, m_name("")
, m_inc(true) 
, m_delayCnt(0)
, m_nowPlaying(0)
{
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_origY = m_destRect.y;
	m_font = TTF_OpenFont( "Vera.ttf", 12 );
}

void NowPlaying::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus)
{
	if(mpdStatusChanged & SONG_CHG) {
		m_nowPlaying = mpdStatus->song;
		m_name = m_playlist.nowPlayingText(m_nowPlaying);
		m_pos = 0;
	}

}
void NowPlaying::draw()
{
	if(m_delayCnt > 2) {
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_clearRect);
		SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));

		if(!m_name.empty()) {	
			SDL_Color color = { 255,255,255, 0 };

			SDL_Surface *sText;
			sText = TTF_RenderText_Solid(m_font, m_name.c_str(), color);
			if(sText->w > m_clearRect.w) {
				std::string name = m_name;
				if(m_inc) {
					if(m_pos < name.length())
						name = name.substr(m_pos);
					else {
						m_inc = false;
						name = name.substr(m_pos-1);
					}
				} else {
					if(m_pos >= 0)
						name = name.substr(m_pos);
					else 
						m_inc = true;
				}
				SDL_FreeSurface(sText);
				sText = TTF_RenderText_Solid(m_font, name.c_str(), color);
			}
			SDL_BlitSurface(sText, NULL, m_screen, &m_destRect );
			SDL_FreeSurface(sText);
			if(m_inc)
				++m_pos;
			else
				--m_pos;
		}
		m_delayCnt = 0;
	} else {
		++m_delayCnt;
	}
	

}
