#include "plBrowser.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "playlist.h"
#include <iostream>
#include <stdexcept>

using namespace std;

PLBrowser::PLBrowser(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, SDL_Rect& rect, int skipVal, int numPerScreen, Playlist& pl)
: Scroller(mpd, screen, font, rect, skipVal, numPerScreen)
, m_playlist(pl)
{
    ls("");
}

void PLBrowser::ls(std::string dir)
{
	mpd_sendLsInfoCommand(m_mpd, dir.c_str());

	m_curDir = dir;
	m_listing.clear();
	m_listing.push_back(make_pair("New Playlist", 3));	
	m_listing.push_back(make_pair("RandomPlaylist", 4));	
	mpd_InfoEntity* mpdItem = mpd_getNextInfoEntity(m_mpd);
	while(mpdItem != NULL) {
		std::string item = "";
		int type = mpdItem->type;
		if(type == 2) 
			item = mpdItem->info.playlistFile->path;
	/*
		if(type == 0) {
			item = mpdItem->info.directory->path;
		} else if(type == 1) {
			item = mpdItem->info.song->file;
		} else if(type == 2) {
			item = mpdItem->info.playlistFile->path;
		} else {
			throw runtime_error("Unknown mpd entity");
		}
	*/
		int pos = item.rfind("/");;
		if(pos != string::npos) {
			item = item.substr(pos+1);
		}
		if(type != 1)
			m_listing.push_back(make_pair(item, type));
		mpd_freeInfoEntity(mpdItem);
		mpdItem = mpd_getNextInfoEntity(m_mpd);
	}
	m_lastItemNum = m_listing.size()-1;
}

void PLBrowser::updateListing()
{
	ls("");
}

std::string PLBrowser::currentItemName()
{
	return m_curItemName;
}
std::string PLBrowser::currentItemPath()
{
	return m_curDir+m_curItemName;

}

void PLBrowser::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus)
{
}

void PLBrowser::processCommand(int command)
{
	if(Scroller::processCommand(command)) {
		//scroller command...parent class processes
	} else if(command == CMD_LOAD_PL) {
		if(m_curItemType == 2) {
			std::string pl = "";
			if(!m_curDir.empty())
				pl = m_curDir+"/";
			pl += m_curItemName;
			mpd_sendClearCommand(m_mpd);
			mpd_finishCommand(m_mpd);
			mpd_sendLoadCommand(m_mpd, pl.c_str());
			mpd_finishCommand(m_mpd);
			mpd_sendPlayCommand(m_mpd, 0);
			mpd_finishCommand(m_mpd);
			m_playlist.initName(pl);
		} else if(m_curItemType == 3) {
			mpd_sendClearCommand(m_mpd);
			mpd_finishCommand(m_mpd);
		} else if(m_curItemType == 4) {
			mpd_sendClearCommand(m_mpd);
			mpd_finishCommand(m_mpd);
			m_playlist.initRandomPlaylist();	
		}
	} else if(command == CMD_APPEND_PL) {
		if(m_curItemType == 2) {
			std::string pl = "";
			if(!m_curDir.empty())
				pl = m_curDir+"/";
			pl += m_curItemName;
			mpd_sendLoadCommand(m_mpd, pl.c_str());
			mpd_finishCommand(m_mpd);
		}
	} else if(command == CMD_DEL_PL) {
		mpd_sendRmCommand(m_mpd, m_curItemName.c_str());
		mpd_finishCommand(m_mpd);
		ls("");
		
	} 
}

void PLBrowser::draw(bool forceRefresh)
{
	if(forceRefresh)
		ls("");
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));
	m_destRect.y = m_origY;

    SDL_Color color = { 255,255,255, 0 };

	SDL_Surface *sText;
	sText = TTF_RenderText_Solid(m_font, "Playlists", color);
	SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
	SDL_FreeSurface(sText);
	m_destRect.y += m_skipVal*2;

	Scroller::draw();	
}

