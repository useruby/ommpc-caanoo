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

#include "statsBar.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "playlist.h"
#include "artButton.h"
#include "guiPos.h"
#include "rptButton.h"
#include "config.h"
#include "threadParms.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>


using namespace std;

ArtButton::ArtButton(artThreadParms_t& artParms, string label, string id)
: Button(id, "overlay")
, m_active(false)
, m_label(label)
, m_sText(NULL)
, m_id(id)
, m_displayText(true)
, m_artParms(artParms)
, m_artist("")
, m_album("")
, m_type("")
, m_genre("")
, m_track("")
, m_showInfo(false)			
{
	m_font = TTF_OpenFont("Vera.ttf", 10);
	m_skipVal = (int)(TTF_FontLineSkip( m_font ) * 1.1);//config.getItemAsFloat("sk_font_main_extra_spacing"));
	if(m_label.empty())
		m_displayText = false;
	m_artistSurface = NULL;
 m_albumSurface=NULL;
 m_typeSurface=NULL;
 m_genreSurface=NULL;
 m_trackSurface=NULL;
}


void ArtButton::init(Config& config, int command)
{
	string btnName = "sk_"+m_name;
	m_clearRect.x = config.getItemAsNum(btnName+"_x");
	m_clearRect.y = config.getItemAsNum(btnName+"_y");
	m_clearRect.w = config.getItemAsNum(btnName+"_width");
	m_clearRect.h = config.getItemAsNum(btnName+"_height");

	config.getItemAsColor("sk_art_flip_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_mouseRect = m_clearRect;
	m_command = command;	
} 


bool ArtButton::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus,
						int rtmpdStatusChanged, mpd_Status* rtmpdStatus, bool updatingSongDb, mpd_Connection* mpd, Config& config)
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
			mpd_sendCurrentSongCommand(mpd);
			mpd_InfoEntity* songEntity = mpd_getNextInfoEntity(mpd);
			if(songEntity != NULL) {
				m_artParms.songFile = config.getItem("albumArtRoot") + songEntity->info.song->file;	
				string title;	
				if(songEntity->info.song->album != NULL)
					m_album  = songEntity->info.song->album;
				else 
					m_album = " ";
				if(songEntity->info.song->artist != NULL)
					m_artist  = songEntity->info.song->artist;
				else 
					m_artist = " ";
				if(songEntity->info.song->genre != NULL)
					m_genre  = songEntity->info.song->genre;
				else 
					m_genre = " ";
				if(songEntity->info.song->track != NULL)
					m_track  = songEntity->info.song->track;
				else 
					m_track = " ";
				if(songEntity->info.song->file != NULL)
					title = songEntity->info.song->file;
		
				int pos = title.rfind('.');
				string ext = title.substr(pos+1);
				if(ext == "mp3")
					ext = " MP3";
				else if(ext == "ogg")
					ext = " OGG";
				else if(ext == "mp4" || ext == "m4p" || ext == "m4a")
					ext = " AAC";
				else if (ext == "flac")
					ext = "FLAC";
				else
					ext = " ";

				m_type = ext;
				if(m_albumSurface != NULL) {
					SDL_FreeSurface(m_albumSurface);
					
				}
				m_albumSurface = TTF_RenderUTF8_Blended(m_font, m_album.c_str(), m_itemColor);
				if(m_artistSurface != NULL)
					SDL_FreeSurface(m_artistSurface);
				m_artistSurface = TTF_RenderUTF8_Blended(m_font, m_artist.c_str(), m_itemColor);
				if(m_typeSurface != NULL)
					SDL_FreeSurface(m_typeSurface);
				m_typeSurface = TTF_RenderUTF8_Blended(m_font,("File Type: "+ m_type).c_str(), m_itemColor);
				if(m_genreSurface != NULL)
					SDL_FreeSurface(m_genreSurface);
				m_genreSurface = TTF_RenderUTF8_Blended(m_font, ("Genre: "+m_genre).c_str(), m_itemColor);
				if(m_trackSurface != NULL)
					SDL_FreeSurface(m_trackSurface);
				m_trackSurface = TTF_RenderUTF8_Blended(m_font, ("Track: "+m_track).c_str(), m_itemColor);
			
			} else {
				m_artParms.songFile = "";
			}
			m_artParms.doArtLoad = true;
			m_refresh = true;
		}
	}	

	return m_refresh;			
}


int ArtButton::processCommand(int command, GuiPos& guiPos)
{
	int rCommand = command;
	if(command > 0) {
		if(command == CMD_CLICK) {
			if(guiPos.curX > m_mouseRect.x 
				&& guiPos.curX < m_mouseRect.x + m_mouseRect.w
				&& guiPos.curY > m_mouseRect.y 
				&& guiPos.curY < m_mouseRect.y + m_mouseRect.h) {
				rCommand = m_command;
				
				m_showInfo = !m_showInfo;
				m_refresh = true;	
				m_counter = m_clearRect.h;
				m_moveRect = m_clearRect;
				m_active = true;
			}
		} else if(command == CMD_POP_SELECT && m_active) {
			rCommand = m_command;
		}
	}
	

	return rCommand;
}

bool ArtButton::draw2(SDL_Surface* screen, SDL_Surface* bg, bool forceRefresh)
{
	if(!m_artParms.doArtLoad && (m_refresh||forceRefresh)) {
		//clear this portion of the screen 
		cout << "here "<< m_clearRect.x<< endl;
		SDL_SetClipRect(screen, &m_clearRect);
		SDL_BlitSurface(bg, &m_clearRect, screen, &m_clearRect );
		if(!m_showInfo) {
			SDL_BlitSurface(m_artParms.artSurface, NULL, screen, &m_clearRect );
		}
		else {
			SDL_Rect saveRect = m_clearRect;
			m_clearRect.y+=m_skipVal*3;
			SDL_BlitSurface(m_albumSurface, NULL, screen, &m_clearRect );
			m_clearRect.y+=m_skipVal;
			SDL_BlitSurface(m_artistSurface, NULL, screen, &m_clearRect );
			m_clearRect.y+=m_skipVal*2;
			SDL_BlitSurface(m_genreSurface, NULL, screen, &m_clearRect );
			m_clearRect.y+=m_skipVal;
			SDL_BlitSurface(m_trackSurface, NULL, screen, &m_clearRect );
			m_clearRect.y+=m_skipVal;
			SDL_BlitSurface(m_typeSurface, NULL, screen, &m_clearRect );
			m_clearRect = saveRect;
		}
		m_artParms.doArtLoad = false;
		m_refresh = false;
	}
	return m_refresh;
}

