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
, m_inc(true) 
, m_delayCnt(0)
, m_delayCnt2(0)
, m_nowPlaying(0)
, m_artistNoScroll(false)
, m_noScroll(false)
{

	//m_origY = m_destRect.y;
	m_font = TTF_OpenFont( "Vera.ttf", 12 );
	m_skipVal = TTF_FontLineSkip( m_font );
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_scrollClearRect = m_destRect;
	m_scrollClearRect.w = m_clearRect.w;
	m_scrollClearRect.h = m_skipVal;
		
	m_fontSmall = TTF_OpenFont( "Vera.ttf", 9 );
	m_artistRect.x = rect.x;
	m_artistRect.y = rect.y + m_skipVal;	
	m_artistClearRect = m_artistRect;
	m_artistClearRect.w = m_clearRect.w;
	int artistSkipVal = TTF_FontLineSkip( m_fontSmall );
	m_artistClearRect.h = artistSkipVal;

	m_clearRect.y = m_artistRect.y + artistSkipVal;
	m_clearRect.h = m_clearRect.h - m_clearRect.y;
	
	m_config.getItemAsColor("sk_nP_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
	m_config.getItemAsColor("sk_nP_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_nP_curItemBackColor", m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b);
	m_config.getItemAsColor("sk_nP_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
	m_format = m_config.getItemAsNum("sk_nP_format");
}

void NowPlaying::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus)
{
	if(mpdStatusChanged & SONG_CHG) {
		m_nowPlaying = mpdStatus->song;
		m_title = m_playlist.nowPlayingTitle(m_nowPlaying);
		m_artist = m_playlist.nowPlayingArtist(m_nowPlaying);
		if(m_format == 0)  {
			m_scrollingText = m_artist + " - " + m_title;
		} else if(m_format == 1) {
			m_scrollingText = m_title;
		} else {
			m_scrollingText = m_artist + " - " + m_title;
		}
		m_pos = 0;
		m_noScroll = false;
		m_artistNoScroll = false;
	}

}
void NowPlaying::draw()
{
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));
	SDL_Surface *sText;
	if(m_noScroll) {
		SDL_SetClipRect(m_screen, &m_scrollClearRect);
		SDL_FillRect(m_screen, &m_scrollClearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));
		sText = TTF_RenderText_Blended(m_font, m_lastScrollText.c_str(), m_itemColor);
		SDL_BlitSurface(sText, NULL, m_screen, &m_destRect );
		SDL_FreeSurface(sText);
	}
	if(m_artistNoScroll) {
		SDL_SetClipRect(m_screen, &m_artistClearRect);
		SDL_FillRect(m_screen, &m_artistClearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));
		sText = TTF_RenderText_Blended(m_fontSmall, m_lastArtistText.c_str(), m_itemColor);
		SDL_BlitSurface(sText, NULL, m_screen, &m_artistRect );
		SDL_FreeSurface(sText);
	}

	if(m_delayCnt > 2 && !m_noScroll) {
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_scrollClearRect);
		SDL_FillRect(m_screen, &m_scrollClearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

		if(!m_scrollingText.empty()) {	
			sText = TTF_RenderText_Blended(m_font, m_scrollingText.c_str(), m_itemColor);
			m_lastScrollText = m_scrollingText;
			if(sText->w > m_clearRect.w) {
				std::string name = m_scrollingText;
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
				sText = TTF_RenderText_Solid(m_font, name.c_str(), m_itemColor);
				m_lastScrollText = name;
			} else {
				m_noScroll = true;
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
	if(m_format == 1) {
		if(m_delayCnt2 > 2 && !m_artistNoScroll) {
			//clear this portion of the screen 
			SDL_SetClipRect(m_screen, &m_artistClearRect);
			SDL_FillRect(m_screen, &m_artistClearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

			if(!m_artist.empty()) {	
				sText = TTF_RenderText_Blended(m_fontSmall, m_artist.c_str(), m_itemColor);
				m_lastArtistText = m_artist;
				if(sText->w > m_clearRect.w) {
					std::string name = m_artist;
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
					sText = TTF_RenderText_Blended(m_fontSmall, name.c_str(), m_itemColor);
					m_lastArtistText = name;
				} else {
					m_artistNoScroll = true;
				}
				SDL_BlitSurface(sText, NULL, m_screen, &m_artistRect );
				SDL_FreeSurface(sText);
				if(m_inc)
					++m_pos;
				else
					--m_pos;
			}
			m_delayCnt2 = 0;
		} else {
			++m_delayCnt2;
		}
	}

}
