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

#include "bookmarks.h"
#include "threadParms.h"
#include "commandFactory.h"
#include "playlist.h"
#include "config.h"
#include "statsBar.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <dirent.h>
using namespace std;

Bookmarks::Bookmarks(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, SDL_Rect& rect, int skipVal, int numPerScreen, Playlist& pl, Config& config, StatsBar& sb)
: Scroller(mpd, screen, font, rect, config, skipVal, numPerScreen)
, m_playlist(pl)
, m_sb(sb)
, m_refresh(true)
{
	m_curDir = config.getItem("programRoot") + "bookmarks/";
	
	m_config.getItemAsColor("sk_main_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
	m_config.getItemAsColor("sk_main_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_main_curItemBackColor", m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b);
	m_config.getItemAsColor("sk_main_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
	 
	ls(m_curDir);
}

void Bookmarks::ls(std::string dir)
{
	m_listing.clear();
	m_listing.push_back(make_pair("Create Bookmark", 7));	

	DIR * udir = opendir(m_curDir.c_str());

	if(udir != NULL) {
		struct dirent * dirent = readdir(udir);

		bool done = false;
		while(dirent != NULL) {
			string ename = dirent->d_name;
			string displayName = ename.substr(0, ename.size() - 6);
			if(ename[0] != '.' && ename.substr(ename.size() - 5) == "bkmrk") {
				m_listing.push_back(make_pair(displayName, 8));	
			}
			dirent = readdir(udir);
		}

	}	
			
	m_lastItemNum = m_listing.size()-1;
}

void Bookmarks::updateListing()
{
	ls(m_curDir);
}

std::string Bookmarks::currentItemName()
{
	return m_curItemName;
}
std::string Bookmarks::currentItemPath()
{
	return m_curDir+m_curItemName;

}

void Bookmarks::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus)
{
	if(mpdStatusChanged & SONG_CHG) {
		m_nowPlaying = mpdStatus->song;	
	}
	if(mpdStatusChanged & STATE_CHG) { 
		m_curState = mpdStatus->state;
		m_refresh = true;
	}
}

void Bookmarks::doSave()
{
	string curTitle = m_playlist.nowPlayingTitle();
	string formattedTime = m_sb.formattedElapsedTime();
	int curTime = m_sb.elapsedTime();
	string curMpdPath;
	string bfile = m_curDir+curTitle+"_"+formattedTime+".bkmrk";
	ofstream out(bfile.c_str(), ios::out| ios::trunc);
   
	if(out.fail()) {
		cout << "unable to create bookmark: can't open " << bfile << endl;
	}
	

	mpd_sendCurrentSongCommand(m_mpd);
	mpd_InfoEntity* songEntity = mpd_getNextInfoEntity(m_mpd);
	if(songEntity != NULL) {
		curMpdPath = songEntity->info.song->file;       
	}

	out << curMpdPath << endl;
	out << curTime << endl;
	ls("");
}

void Bookmarks::processCommand(int command)
{
	try {
	if(Scroller::processCommand(command)) {
		//scroller command...parent class processes
	} else if(command == CMD_LOAD_BKMRK) {
		if(m_curItemName == "Create Bookmark") {
			doSave();
		} else {
			char tmp[256];
			ifstream in((currentItemPath()+".bkmrk").c_str(), ios::in);
			if(!in.fail()) {
				in.getline(tmp, 256);
				std::string songPath(tmp);
				in.getline(tmp, 256);
				int elapsed = atoi(tmp);

				int id = mpd_sendAddIdCommand(m_mpd, songPath.c_str());
				mpd_finishCommand(m_mpd);
				mpd_sendMoveIdCommand(m_mpd, id, m_nowPlaying+1);
				mpd_finishCommand(m_mpd);
				mpd_sendPlayCommand(m_mpd, m_nowPlaying+1);
				mpd_finishCommand(m_mpd);
				mpd_sendSeekCommand(m_mpd, m_nowPlaying+1, elapsed);	
				mpd_finishCommand(m_mpd);
			}
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
	} else if(command == CMD_SAVE_BKMRK) {
		doSave();
	} else if(command == CMD_DEL_BKMRK) {
		unlink((currentItemPath()+".bkmrk").c_str());
		ls("");
	} 
	} catch (std::exception & e) {
		cout << e.what() << endl;
	}
}

void Bookmarks::draw(bool forceRefresh)
{
	if(forceRefresh || m_refresh) {
		ls("");
		//clear this portion of the screen 
		SDL_SetClipRect(m_screen, &m_clearRect);
		SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

		SDL_Surface *sText;
		sText = TTF_RenderText_Blended(m_font, "Bookmarks", m_itemColor);
		SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
		SDL_FreeSurface(sText);
		m_destRect.y += m_skipVal*2;
		m_curItemClearRect.y += m_skipVal*2;

		Scroller::draw();		
	}
}

