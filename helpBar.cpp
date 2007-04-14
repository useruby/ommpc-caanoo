#include "helpBar.h"
#include <iostream>
#include <stdexcept>

using namespace std;

HelpBar::HelpBar(mpd_Connection* mpd, SDL_Surface* screen, Config& config, SDL_Rect& rect)
: m_mpd(mpd)
, m_screen(screen)
, m_config(config)
, m_clearRect(rect)
, m_pos(0)
{
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_origY = m_destRect.y;
	m_font = TTF_OpenFont( "Vera.ttf", 8 );
	
	m_modeCmdText.push_back("p-Select/Play i-Add");
	m_modeCmdText.push_back("p-Play/Pause s-Stop n-Next p-Prev d-Del");
	m_modeCmdText.push_back("p-Select d-Del");

}

void HelpBar::updateStatus(bool mpdStatusChanged, mpd_Status* mpdStatus)
{

}

void HelpBar::draw(int curMode, bool forceUpdate)
{
	//clear this portion of the screen 
	SDL_SetClipRect(m_screen, &m_clearRect);
	SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 200, 10));
	//SDL_FillRect(m_screen, &m_clearRect, SDL_MapRGB(m_screen->format, 0, 0, 0));

	SDL_Color color = { 255,255,255, 0 };
	
	SDL_Surface *sText;
	sText = TTF_RenderText_Solid(m_font, m_modeCmdText[curMode].c_str(), color);
	SDL_BlitSurface(sText, NULL, m_screen, &m_destRect );
	SDL_FreeSurface(sText);


}
