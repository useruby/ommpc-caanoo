#include "albumArt.h"
#include "threadParms.h"
#include "threadFunctions.h"
#include <iostream>
#include <stdexcept>

using namespace std;

AlbumArt::AlbumArt(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect,
					artThreadParms_t& artParms)
: m_mpd(mpd)
, m_screen(screen)
, m_config(config)
, m_clearRect(rect)
, m_pos(0)
, m_update(false)
, m_artParms(artParms)
{
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_config.getItemAsColor("sk_art_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
}

void AlbumArt::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus,
							int rtmpdStatusChanged, mpd_Status* rtmpdStatus)
{
	mpd_Status * status;
	int statusChanged;

	if(rtmpdStatusChanged > 0) {
		status = rtmpdStatus;
		statusChanged = rtmpdStatusChanged;
	} else {
		status = mpdStatus;
		statusChanged = mpdStatusChanged;
	}

	if(statusChanged > 0) {
		if(statusChanged & SONG_CHG) {
			mpd_sendCurrentSongCommand(m_mpd);
			mpd_InfoEntity* songEntity = mpd_getNextInfoEntity(m_mpd);
			if(songEntity != NULL) {
				m_artParms.songFile = m_config.getItem("albumArtRoot") + songEntity->info.song->file;		
			} else {
				m_artParms.songFile = "";
			}
			m_artParms.doArtLoad = true;
			m_update = true;
		}
	}	
			

}

void AlbumArt::draw(bool forceUpdate)
{
	if((m_update || forceUpdate) && !m_artParms.doArtLoad) {
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_clearRect);
		SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

		SDL_BlitSurface(m_artParms.artSurface, NULL, m_screen, &m_destRect );
		m_artParms.doArtLoad = false;
	}

}
