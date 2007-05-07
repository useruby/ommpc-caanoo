#include "statsBar.h"
#include "threadParms.h"
#include "commandFactory.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
using namespace std;

StatsBar::StatsBar(mpd_Connection* mpd, SDL_mutex* lock, SDL_Surface* screen, Config& config, SDL_Rect& rect, bool& initVolume)
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
, m_cols(config.getItemAsNum("sk_stats_numCols"))
, m_firstPass(initVolume)
{
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_origY = m_destRect.y;
	m_font = TTF_OpenFont( "Vera.ttf", 8 );
	m_itemH = TTF_FontLineSkip( m_font );
	
	m_config.getItemAsColor("sk_stats_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
	m_config.getItemAsColor("sk_stats_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_stats_curItemBackColor", m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b);
	m_config.getItemAsColor("sk_stats_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
}

string StatsBar::formattedElapsedTime()
{
	return m_elapsed;
}

int StatsBar::elapsedTime()
{
	return m_elapsedSeconds;
}

void StatsBar::updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus,
							int rtmpdStatusChanged, mpd_Status* rtmpdStatus, bool forceRefresh)
{
	mpd_Status * status;
	int statusChanged;
	bool doVol = false;
	if(rtmpdStatusChanged > 0) {
		status = rtmpdStatus;
		statusChanged = rtmpdStatusChanged;
		doVol = true;
	} else {
		status = mpdStatus;
		statusChanged = mpdStatusChanged;
	}

	if(statusChanged > 0) {
		ostringstream out;
		if(statusChanged & VOL_CHG) { 
			if(doVol || m_firstPass) {
				out << "Vol: " << status->volume/5;
				m_items[0] = out.str();
				m_curVol = status->volume;
				m_firstPass = false;
			} /*else {
				cout << "resetting" << endl;
				mpd_sendSetvolCommand(m_mpd, -100);
				mpd_finishCommand(m_mpd);
				mpd_sendSetvolCommand(m_mpd, m_curVol);
				mpd_finishCommand(m_mpd);

			} */
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
			m_items[3] = playType;
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
			m_elapsedSeconds = status->elapsedTime;
			int mins = m_elapsedSeconds/60;
			int seconds = m_elapsedSeconds - (mins*60);
			out << setfill('0') << setw(2) << mins << ":" << setw(2)<< seconds;
			m_elapsed = out.str();
			out << "/" << m_total;
			m_items[1] = out.str();
		} 

		if(statusChanged & RATE_CHG) { 
			out.str("");
			out << status->bitRate << " kbps";
			m_items[2] = out.str();
		}
	}
}

void StatsBar::draw(bool forceRefresh)
{
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));
	
	int x = m_destRect.x;
	int y = m_destRect.y;
	int curCol = 0;	
	SDL_Rect clearRect = {x+2, y+2, 50, m_itemH}; 
	SDL_Rect curItemRect = {x+2, y+2, 50, m_itemH}; 

	
	for(int i=0; i<4; ++i) {
		if(!m_items[0].empty()) {
			SDL_FillRect(m_screen, &clearRect, SDL_MapRGB(m_screen->format, m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b));
			SDL_Surface *sText;
			sText = TTF_RenderText_Blended(m_font, m_items[i].c_str(), m_itemColor);
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
