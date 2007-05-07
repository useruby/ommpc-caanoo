#include "browser.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "config.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Browser::Browser(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, SDL_Rect& rect,	
					Config& config, int skipVal, int numPerScreen)
: Scroller(mpd, screen, font, rect, config, skipVal, numPerScreen-1)
, m_nowPlaying(0)
, m_view(0)
, m_curTitle("")
, m_curAlbum("")
, m_updatingDb(false)
, m_refresh(true)
{
	m_config.getItemAsColor("sk_main_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
	m_config.getItemAsColor("sk_main_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_main_curItemBackColor", m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b);
	m_config.getItemAsColor("sk_main_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
    ls("");
    //ls("tim");
}

void Browser::ls(std::string item)
{
	switch(m_view) {
		case 0:
			browseFileSystem(item);
			break;
		case 1:
			if(item.empty() || (m_curTitle.empty() && item == ".."))
				browseArtists();
			else if(m_curAlbum.empty())
				browseAlbumsByArtist(item);
//			else
//				browseTitleByAlbum(item);
			break;
		default:
			browseFileSystem(item);
			break;
	}
}

void Browser::browseFileSystem(std::string dir) {
	mpd_sendLsInfoCommand(m_mpd, dir.c_str());
	m_curDir = dir;
	m_listing.clear();
	m_listing.push_back(make_pair("..", 0));
	mpd_InfoEntity* mpdItem = mpd_getNextInfoEntity(m_mpd);
	while(mpdItem != NULL) {
		std::string item = "";
		int type = mpdItem->type;
		if(type == 0) {
			item = mpdItem->info.directory->path;
		} else if(type == 1) {
			item = mpdItem->info.song->file;
		} 
		/*else if(type == 2) {
			item = mpdItem->info.playlistFile->path;
		} else {
			throw runtime_error("Unknown mpd entity");
		}
		*/
		int pos = item.rfind("/");;
		if(pos != string::npos) {
			item = item.substr(pos+1);
		}
//	cout << item << endl;
		m_listing.push_back(make_pair(item, type));
		mpd_freeInfoEntity(mpdItem);
		mpdItem = mpd_getNextInfoEntity(m_mpd);
	}
	m_lastItemNum = m_listing.size()-1;
}

void Browser::browseArtists() {
	mpd_sendListCommand(m_mpd, MPD_TAG_ITEM_ARTIST, NULL);

	m_listing.clear();
	char * artist = mpd_getNextArtist(m_mpd);	
	while(artist != NULL) {
		m_listing.push_back(make_pair(artist, 6));
		free(artist);
		artist = mpd_getNextArtist(m_mpd);	
	}
	m_lastItemNum = m_listing.size()-1;

}

void Browser::browseAlbumsByArtist(string artist) {
	mpd_sendListCommand(m_mpd, MPD_TAG_ITEM_ALBUM, artist.c_str());
cout << "artist " << artist << endl;
	m_listing.clear();
	m_listing.push_back(make_pair("..", 0));
	char * album = mpd_getNextAlbum(m_mpd);	
	while(album != NULL) {
		m_listing.push_back(make_pair(album, 0));
		free(album);
		album = mpd_getNextAlbum(m_mpd);	
	}
	m_lastItemNum = m_listing.size()-1;
}

std::string Browser::currentItemName()
{
	return m_curItemName;
}
std::string Browser::currentItemPath()
{
	return m_curDir+m_curItemName;

}

void Browser::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus)
{
	if(mpdStatusChanged & SONG_CHG) {
		m_nowPlaying = mpdStatus->song;	
	} 
	if(mpdStatusChanged & STATE_CHG) { 
		m_curState = mpdStatus->state;
		m_refresh = true;
	}
	if(mpdStatusChanged & UPDB_CHG) {
		if(mpdStatus->updatingDb == 0) 
			m_updatingDb = false;
		else
			m_updatingDb = true;
		ls("");
		m_refresh = true;	
		cout << "updating db " << m_updatingDb<<  endl;
	}
}

void Browser::processCommand(int command) {

	if(command > 0) {
		m_refresh = true;
		if(Scroller::processCommand(command)) {
			//scroller command...parent class processes
		} else if(command == CMD_IMMEDIATE_PLAY) {
			std::string dir;
			if(m_curItemName == "..") {
				int pos = m_curDir.rfind("/");;
				if(pos == string::npos || pos == 0) 
					dir = "";
				else
					dir = m_curDir.substr(0, pos);
			} else  {
				dir = m_curDir;
				if(!dir.empty())
					dir += "/";
				dir += m_curItemName;
			}
			if(m_curItemType == 0) { //direcotry
				ls(dir);
				m_curItemNum = 0;
				m_topItemNum = 0;
				m_curItemName = "";
			} else if(m_curItemType == 1) {
				std::string song = "";
				if(!m_curDir.empty())
					song = m_curDir+"/";
				song += m_curItemName;
				int id = mpd_sendAddIdCommand(m_mpd, song.c_str());
				mpd_finishCommand(m_mpd);
				mpd_sendMoveIdCommand(m_mpd, id, m_nowPlaying+1);
				mpd_finishCommand(m_mpd);
				mpd_sendPlayCommand(m_mpd, m_nowPlaying+1);
				mpd_finishCommand(m_mpd);
			} else if(m_curItemType == 6) {
				ls(m_curItemName);	
			}
		} else if(command == CMD_PREV_DIR) {
			std::string dir;
			int pos = m_curDir.rfind("/");;
			if(pos == string::npos || pos == 0) 
				dir = "";
			else
				dir = m_curDir.substr(0, pos);
			ls(dir);
			m_curItemNum = 0;
			m_topItemNum = 0;
			m_curItemName = "";
		} else if(command  == CMD_ADD_TO_PL) {
			if(m_curItemType == 1) {
				std::string song = "";
				if(!m_curDir.empty())
					song = m_curDir+"/";
				song += m_curItemName;
				mpd_sendAddCommand(m_mpd, song.c_str());
				mpd_finishCommand(m_mpd);
			}
		} else if(command == CMD_PAUSE) {
			if(m_curState == MPD_STATUS_STATE_PAUSE) {
				m_curState = MPD_STATUS_STATE_PLAY;	
				mpd_sendPauseCommand(m_mpd, 0);
				mpd_finishCommand(m_mpd);
			} else if(m_curState == MPD_STATUS_STATE_PLAY) {
				m_curState = MPD_STATUS_STATE_PAUSE;
				mpd_sendPauseCommand(m_mpd, 1);
				mpd_finishCommand(m_mpd);
			}
		} else if(command == CMD_TOGGLE_VIEW) {
			/*
			   if(m_view == 1)
			   m_view = 0;
			   else
			   ++m_view;
			   if(m_view == 0)
			   ls(m_curDir);
			   else 
			   ls("");
			   */
		}
	}
}
void Browser::draw(bool forceRefresh)
{
	if(forceRefresh || m_refresh) {
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_clearRect);
		SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

		SDL_Surface *sText;
		sText = TTF_RenderText_Blended(m_font, m_curDir.c_str(), m_itemColor);
		SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
		SDL_FreeSurface(sText);
		m_destRect.y += m_skipVal*2;
		m_curItemClearRect.y += m_skipVal*2;

		Scroller::draw();	

		if(m_updatingDb) {

			SDL_Rect dstrect;
			dstrect.x = (m_screen->w - 80) / 2;
			dstrect.y = (m_screen->h - m_skipVal) / 2;
			dstrect.w = 80;
			dstrect.h = m_skipVal;

			SDL_FillRect(m_screen, &dstrect, SDL_MapRGB(m_screen->format, m_pauseColor.r, m_pauseColor.g, m_pauseColor.b));
			sText = TTF_RenderText_Blended(m_font, "Updating Db...", m_pauseItemColor);
			SDL_BlitSurface(sText,NULL, m_screen, &dstrect );
			SDL_FreeSurface(sText);


		}
		m_refresh = false;
	}
	
}

