#include "statsBar.h"
#include "threadParms.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
using namespace std;

StatsBar::StatsBar(mpd_Connection* mpd, SDL_mutex* lock, SDL_Surface* screen, Config& config, SDL_Rect& rect)
: m_mpd(mpd)
, m_lock(lock)
, m_screen(screen)
, m_config(config)
, m_clearRect(rect)
, m_volume("")
, m_playType("")
, m_crossfade("")
, m_elapsed("")
, m_total("")
, m_bitRate("")
, m_cols(2)
{
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_origY = m_destRect.y;
	m_font = TTF_OpenFont( "Vera.ttf", 8 );
	m_itemH = TTF_FontLineSkip( m_font );
	
}

void StatsBar::draw(int mpdStatusChanged, mpd_Status* mpdStatus,
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
		m_items.clear();
		ostringstream out;
		if(statusChanged & VOL_CHG) { 
			out << "Vol: " << status->volume/5;
			m_items.push_back(out.str());
		} else {
			m_items.push_back("");
		}
	
		if(statusChanged & RPT_CHG || statusChanged & RND_CHG) { 
			out.str("");
			string playType = "";
			if(status->repeat == 1)
				playType = "Rpt";
			if(status->random == 1) {
				if(!playType.empty())
					playType += "/";
				playType += "Rnd";
			}
			if(playType.empty())
				playType = " ";
			m_items.push_back(playType);
		} else {
			m_items.push_back("");
		}
/*		out.str("");
		out << mpdStatus->crossfade;
		m_crossfade = out.str();
*/

		if(statusChanged & SONG_CHG) {
			int total = status->totalTime;
			int mins = total/60;
			int seconds = total - (mins*60);
			out << setfill('0') << setw(2) << mins << ":" << setw(2)<< seconds;

			m_total = out.str();
		}	
			
		if(statusChanged & ELAPSED_CHG) { 
			out.str("");
			int elapsed = status->elapsedTime;
			int mins = elapsed/60;
			int seconds = elapsed - (mins*60);
			out << setfill('0') << setw(2) << mins << ":" << setw(2)<< seconds;

			out << "/" << m_total;
			m_items.push_back(out.str());
		} else {
			m_items.push_back("");
		}

		if(statusChanged & RATE_CHG) { 
			out.str("");
			out << status->bitRate << " kbps";
			m_items.push_back(out.str());
		} else {
			m_items.push_back("");
		}
	}
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
//	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));
	
	SDL_Color color = {0,0,0,0};
	int x = m_destRect.x;
	int y = m_destRect.y;
	int curCol = 0;	
	SDL_Rect clearRect = {x+2, y+2, 50, m_itemH}; 
	SDL_Rect curItemRect = {x+2, y+2, 50, m_itemH}; 

	
	for(vector<string>::iterator itemIter = m_items.begin();
	itemIter != m_items.end();
	++itemIter) {
		if(!(*itemIter).empty()) {
			SDL_FillRect(m_screen, &clearRect, SDL_MapRGB(m_screen->format, 255, 255, 255));
			SDL_Surface *sText;
			sText = TTF_RenderText_Solid(m_font, (*itemIter).c_str(), color);
			SDL_BlitSurface(sText, NULL, m_screen, &curItemRect );
			SDL_FreeSurface(sText);
		}
		++curCol;
		if(curCol == m_cols) {
			curItemRect.y += m_itemH+2;
			clearRect.y += m_itemH+2;
			curItemRect.x = x+2;
			clearRect.x = x+2;
			curCol = 0;
		} else  {
			curItemRect.x += 54;
			clearRect.x += 54;
		}
	}

}
