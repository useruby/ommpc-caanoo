#include "playlist.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "popup.h"
#include "timestamp.h"

#include <iostream>
#include <stdexcept>
#include <time.h>

using namespace std;

Playlist::Playlist(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, Config& config, SDL_Rect& rect, int skipVal, int numPerScreen)
: Scroller(mpd, screen, font, rect, skipVal, numPerScreen)
, m_config(config)
, m_curElapsed(0)
, m_view(0)
, m_curState(0)
, m_random(false)
, m_otg(false)
, m_modified(false)
, m_name("")
, m_moveTo(-1)
, m_moveFrom(-1)
{
	m_origY = m_destRect.y;

	load("");
}

void Playlist::load(std::string dir)
{
	if(!dir.empty()) { //load playlist from disk
	}

	mpd_sendPlaylistInfoCommand(m_mpd, -1);

	//m_path = dir;
	m_listing.clear();	
	mpd_InfoEntity* mpdItem = mpd_getNextInfoEntity(m_mpd);
	while(mpdItem != NULL) {
		std::string item = "";
		int type = mpdItem->type;
		if(type == 1) {
			if(m_view == 0 && mpdItem->info.song->title != NULL
						   && mpdItem->info.song->artist != NULL) { //Artist - Title
				item = mpdItem->info.song->title;
				item = " - " + item;
				item = mpdItem->info.song->artist + item;
			} else if(m_view ==1 && mpdItem->info.song->title != NULL) { //Title
				item = mpdItem->info.song->title;
			} else {
				item = mpdItem->info.song->file;
			}
		} else {
			throw runtime_error("Unknown mpd entity for playlist");
		}
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

void Playlist::makeNowPlayingVisible()
{
	if(m_nowPlaying < m_topItemNum ||
		m_nowPlaying > m_topItemNum + m_numPerScreen) {

		m_topItemNum  = m_nowPlaying - (m_numPerScreen/2);
	}

}

bool Playlist::showSaveDialog(Popup& popup)
{
	TimeStamp ts;
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_LIST;
	if(!m_name.empty())
		items.push_back(make_pair(m_name, type));	
			
	items.push_back(make_pair("playlist_" + ts.currentTimeAsString(3, false), 
								(int)Popup::POPUP_DO_SAVE_PL)); 
	items.push_back(make_pair("Cancel", (int)Popup::POPUP_CANCEL)); 
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 180;
	popRect.h = m_skipVal*5+15;
	popRect.x = (m_screen->w - popRect.w) / 2;
	popRect.y = (m_screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("Save Playlist As...");
	show = true;

	return show;
}

int Playlist::getRand(int max)
{
    /* r is a random floating point value in the range [0,1) {including 0, not including 1}. Note we must convert rand() and/or RAND_MAX+1 to floating point values to avoid integer division. In addition, Sean Scanlon pointed out the possibility that RAND_MAX may be the largest positive integer the architecture can represent, so (RAND_MAX+1) may result in an overflow, or more likely the value will end up being the largest negative integer the architecture can represent, so to avoid this we convert RAND_MAX and 1 to doubles before adding. */
	double r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
   /* x is a random integer in the range [0,M) {including 0, not including M}. If M is an integer then the range is [0,M-1] {inclusive} */
    int x = (int)(r * (max));
	return x;
}

void Playlist::initRandomPlaylist()
{
	m_random = true;
	time_t t = time(NULL);
	srand(t);
	mpd_sendListallCommand(m_mpd, "");

	m_listing.clear();	
	m_all.clear();
	mpd_InfoEntity* mpdItem = mpd_getNextInfoEntity(m_mpd);
	int i = 0;
	while(mpdItem != NULL) {
		std::string item = "";
		if(mpdItem->type == 1) {
			item = mpdItem->info.song->file;
			m_all.push_back(item);	
		}	
		mpdItem = mpd_getNextInfoEntity(m_mpd);
	}

	int size = m_all.size();
	for(int i=0; i<65; ++i) {
		mpd_sendAddCommand(m_mpd, m_all[getRand(size)].c_str());	
		mpd_finishCommand(m_mpd);
	}
	m_otg = true;
}

void Playlist::initNewPlaylist()
{
	m_otg = true;
}

void Playlist::initName(std::string name)
{
	m_name = name;
}

std::string Playlist::currentItemName()
{
	return m_curItemName;
}
std::string Playlist::currentItemPath()
{
	return m_curDir+m_curItemName;

}

std::string Playlist::nowPlayingText(int song)
{
	if(song <= m_listing.size() && !m_listing.empty())
		return m_listing[song].first;
	else 
		return "";
}

void Playlist::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus,
							int rtmpdStatusChanged, mpd_Status* rtmpdStatus, int repeatDelay)
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

	if(statusChanged & PL_CHG) {
		load("");
	}		
	if((statusChanged & ELAPSED_CHG) && repeatDelay == 0) { 
		m_curElapsed = status->elapsedTime;	
	}
	if(statusChanged & STATE_CHG) { 
		m_curState = status->state;
	}
	if(statusChanged & SONG_CHG) {
		m_nowPlaying = status->song;	
		m_curItemNum = m_nowPlaying;
		makeNowPlayingVisible();
/*
		if(m_random && m_safe) {
			//append new random song
			mpd_sendDeleteCommand(m_mpd, 0);
			mpd_finishCommand(m_mpd);
			mpd_sendAddCommand(m_mpd, m_all[getRand(m_all.size())].c_str());	
			mpd_finishCommand(m_mpd);
			m_safe = false;
		} else {
			m_safe = true;
		}
*/
	}

}

void Playlist::processCommand(int command, int& rtmpdStatusChanged, mpd_Status* rtmpdStatus, int repeatDelay)
{
	if(m_moveFrom >= 0 && command != 0 && command != CMD_UP && command != CMD_DOWN && command != CMD_MOVE_IN_PL) {
		m_moveFrom = -1;
	}	
	if(Scroller::processCommand(command)) {
		//scroller command...parent class processes
	} else if(command == CMD_PLAY_PAUSE) {
		if(m_curItemType == 1) {
			if(m_curState == MPD_STATUS_STATE_PAUSE && m_nowPlaying == m_curItemNum) {
				m_curState = MPD_STATUS_STATE_PLAY;	
				mpd_sendPauseCommand(m_mpd, 0);
				mpd_finishCommand(m_mpd);
			} else if(m_curState == MPD_STATUS_STATE_PLAY && m_nowPlaying == m_curItemNum) {
				m_curState = MPD_STATUS_STATE_PAUSE;
				mpd_sendPauseCommand(m_mpd, 1);
				mpd_finishCommand(m_mpd);
			} else {
				mpd_sendStopCommand(m_mpd);
				mpd_finishCommand(m_mpd);
				mpd_sendPlayCommand(m_mpd, m_curItemNum);
				mpd_finishCommand(m_mpd);
			}
		}
	} else if(command == CMD_NEXT) {
		mpd_sendNextCommand(m_mpd);
		mpd_finishCommand(m_mpd);
	} else if(command == CMD_PREV) {
		mpd_sendPrevCommand(m_mpd);
		mpd_finishCommand(m_mpd);
	} else if(command == CMD_FF) {
		if(repeatDelay > 0) {
			cout << repeatDelay << endl;
			if(repeatDelay > 20)
				mpd_sendSeekCommand(m_mpd, m_curItemNum, m_curElapsed + repeatDelay*2);
			else 	
				mpd_sendSeekCommand(m_mpd, m_curItemNum, m_curElapsed + repeatDelay);
			mpd_finishCommand(m_mpd);
		}
	} else if(command == CMD_RW) {
		if(repeatDelay > 0) {
			if(repeatDelay > 20)
				mpd_sendSeekCommand(m_mpd, m_curItemNum, m_curElapsed - repeatDelay*2);
			else 	
				mpd_sendSeekCommand(m_mpd, m_curItemNum, m_curElapsed - repeatDelay);
			mpd_finishCommand(m_mpd);
		}
	} else if(command == CMD_TOGGLE_VIEW) {
		if(m_view == 2)
			m_view = 0;
		else
			++m_view;

		load("");
	} else if(command == CMD_DEL_FROM_PL) {
		mpd_sendDeleteCommand(m_mpd, m_curItemNum);
		mpd_finishCommand(m_mpd);
		rtmpdStatusChanged += PL_CHG;
		mpd_sendStatusCommand(m_mpd);
		rtmpdStatus = mpd_getStatus(m_mpd);
	} else if(command == CMD_MOVE_IN_PL) {
		if(m_moveFrom >= 0) {
			m_moveTo = m_curItemNum;
			mpd_sendMoveCommand(m_mpd, m_moveFrom, m_moveTo);
			mpd_finishCommand(m_mpd);
			m_moveFrom = -1;
			rtmpdStatusChanged += PL_CHG;
			mpd_sendStatusCommand(m_mpd);
			rtmpdStatus = mpd_getStatus(m_mpd);
			} else if(command == CMD_TOGGLE_MODE) {
		} else {	
			m_moveFrom = m_curItemNum;
		}
	}
}

void Playlist::draw(bool forceRefresh)
{
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));
	m_destRect.y = m_origY;

	Scroller::draw();

	SDL_Surface *sText;
    SDL_Color color = { 255,255,255, 0 };
	if(m_curState == MPD_STATUS_STATE_PAUSE) {
		SDL_Rect dstrect;
		dstrect.x = (m_screen->w - 50) / 2;
		dstrect.y = (m_screen->h - m_skipVal) / 2;
		dstrect.w = 50;
		dstrect.h = m_skipVal;
		SDL_FillRect(m_screen, &dstrect, SDL_MapRGB(m_screen->format, 0, 0, 0));
		sText = TTF_RenderText_Solid(m_font, "PAUSED", color);
		SDL_BlitSurface(sText,NULL, m_screen, &dstrect );
		SDL_FreeSurface(sText);
	}

}

