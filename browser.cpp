#include "browser.h"
#include "threadParms.h"
#include "commandFactory.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Browser::Browser(mpd_Connection* mpd, SDL_Surface* screen, 
					TTF_Font* font, SDL_Rect& rect,	int skipVal, int numPerScreen)
: Scroller(mpd, screen, font, rect, skipVal, numPerScreen)
, m_nowPlaying(0)
{
    ls("tim");
}

void Browser::ls(std::string dir)
{
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
		m_listing.push_back(make_pair(item, type));
		mpd_freeInfoEntity(mpdItem);
		mpdItem = mpd_getNextInfoEntity(m_mpd);
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
}

void Browser::processCommand(int command) {

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
		}
	} else if(command  == CMD_ADD_TO_PL) {
		if(m_curItemType == 1) {
			std::string song = "";
			if(!m_curDir.empty())
				song = m_curDir+"/";
			song += m_curItemName;
			mpd_sendAddCommand(m_mpd, song.c_str());
			mpd_finishCommand(m_mpd);
		}
	}

}
void Browser::draw(bool forceRefresh)
{
	m_destRect.y = m_origY;
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));
    SDL_Color color = { 255,255,255, 0 };

	SDL_Surface *sText;
	sText = TTF_RenderText_Solid(m_font, m_curDir.c_str(), color);
	SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
	SDL_FreeSurface(sText);
	m_destRect.y += m_skipVal*2;

	Scroller::draw();	
}

