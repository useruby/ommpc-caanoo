#include "helpBar.h"
#include <iostream>
#include <stdexcept>
#define GP2X
using namespace std;

HelpBar::HelpBar(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect)
: m_mpd(mpd)
, m_screen(screen)
, m_config(config)
, m_clearRect(rect)
, m_pos(0)
{
	m_destRect.x = rect.x;
	if(m_clearRect.h > 20) {
		m_destRect.x += 5;	
	}
	m_destRect.y = rect.y;
	m_origY = m_destRect.y;
	m_font = TTF_OpenFont( "Vera.ttf", 8 );
	m_skipVal = TTF_FontLineSkip( m_font );

	vector<string> tmp;
#ifdef GP2X	
	//browser
	tmp.push_back("B-Play/Pause");	
	tmp.push_back("A-Append");	
	tmp.push_back("X-Stop");	
	m_modeCmdText.push_back(tmp);
	tmp.clear();
	//playlist	
	tmp.push_back("B-Play/Pause");	
	tmp.push_back("X-Stop");	
	tmp.push_back("R-Next");	
	tmp.push_back("L-Prev");	
	tmp.push_back("A-Delete");	
	tmp.push_back("Y-Move");	
	m_modeCmdText.push_back(tmp);
	tmp.clear();
	//pl browser
	tmp.push_back("B-Load");	
	tmp.push_back("A-Append");	
	tmp.push_back("Y-Delete");	
	m_modeCmdText.push_back(tmp);
	tmp.clear();
	//bookmarks
	tmp.push_back("B-Load");	
	tmp.push_back("Y-Delete");	
	m_modeCmdText.push_back(tmp);
#else
	tmp.push_back("p-Play/Pause");	
	tmp.push_back("i-Append");	
	tmp.push_back("s-Stop");	
	m_modeCmdText.push_back(tmp);
	tmp.clear();
	tmp.push_back("p-Play/Pause");	
	tmp.push_back("s-Stop");	
	tmp.push_back("n-Next");	
	tmp.push_back("p-Prev");	
	tmp.push_back("d-Delete");	
	tmp.push_back("h-Move");	
#endif
	
	m_config.getItemAsColor("sk_help_backColor", m_backColor.r, m_backColor.g, m_backColor.b);
	m_config.getItemAsColor("sk_help_itemColor", m_itemColor.r, m_itemColor.g, m_itemColor.b);
	m_config.getItemAsColor("sk_help_curItemBackColor", m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b);
	m_config.getItemAsColor("sk_help_curItemColor", m_curItemColor.r, m_curItemColor.g, m_curItemColor.b);
}

void HelpBar::updateStatus(bool mpdStatusChanged, mpd_Status* mpdStatus)
{

}

void HelpBar::draw(int curMode, bool forceUpdate)
{
	m_destRect.x = m_clearRect.x;
	if(m_clearRect.h > 20) {
		m_destRect.x += 5;	
	}
	m_destRect.y = m_clearRect.y;
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, m_backColor.r, m_backColor.g, m_backColor.b));

	
	SDL_Surface *sText;
	for(vector<string>::iterator vvIter = m_modeCmdText[curMode].begin();
	vvIter != m_modeCmdText[curMode].end();
	++vvIter) {
		sText = TTF_RenderText_Blended(m_font, (*vvIter).c_str(), m_itemColor);
		SDL_BlitSurface(sText, NULL, m_screen, &m_destRect );
		if(m_clearRect.h < 20) {
			m_destRect.x += sText->w+2;
		} else {
			m_destRect.y += m_skipVal;
		}
		SDL_FreeSurface(sText);
	}


}
