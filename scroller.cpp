#include "scroller.h"
#include "commandFactory.h"

#include <iostream>
using namespace std;

Scroller::Scroller(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, 
		SDL_Rect& rect, Config& config, int skipVal, int numPerScreen)
	: m_mpd(mpd)
	, m_screen(screen)
	, m_font(font)
	, m_config(config)
	, m_skipVal(skipVal)
	, m_numPerScreen(numPerScreen)
	, m_topItemNum(0)
	, m_curItemNum(0)
	, m_clearRect(rect)
	, m_lastItemNum(0)
{
	m_destRect.x = rect.x;
	m_destRect.y = rect.y;
	m_origY = m_destRect.y;
	m_curItemClearRect = m_destRect;
}

bool Scroller::processCommand(int command)
{
	bool done = false;
	if(command == CMD_DOWN) {
		++m_curItemNum;
		if(m_curItemNum > m_lastItemNum) {
			m_topItemNum = m_curItemNum = 0;
		} else 	if(m_curItemNum >= m_numPerScreen+m_topItemNum) {
			++m_topItemNum;
		}
		done = true;
	} else if(command == CMD_UP) {
		if(m_curItemNum > 0) {
			--m_curItemNum;
			if(m_curItemNum <= m_topItemNum && m_topItemNum >0)
				--m_topItemNum;
		} else if(m_curItemNum == 0) {
			m_curItemNum = m_lastItemNum;
			m_topItemNum = m_curItemNum - m_numPerScreen+1;			

		}
		done = true;
	}

	return done;
}

int Scroller::skipVal()
{
	return m_skipVal;
}
	
void Scroller::draw() 
{
	SDL_Surface *sText;
	int numProcessed = 0;
	int numDisplayed = 0;
	for(listing_t::iterator vIter = m_listing.begin();
		vIter != m_listing.end() && (numDisplayed <= m_numPerScreen);
		++vIter) {
		if(numProcessed >= m_topItemNum) {
			string str = (*vIter).first;
			if(numProcessed == m_curItemNum) {
				sText = TTF_RenderText_Blended(m_font, str.c_str(), m_curItemColor);
				m_curItemClearRect.w = sText->w;
				m_curItemClearRect.h = sText->h;
				SDL_FillRect(m_screen, &m_curItemClearRect, SDL_MapRGB(m_screen->format, m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b));
				m_curItemName = (*vIter).first;
				m_curItemType = (*vIter).second;
			} else {
				//sText = TTF_RenderText_Solid(m_font, (*vIter).first.c_str(), color);
				sText = TTF_RenderText_Blended(m_font, str.c_str(), m_itemColor);
				//sText = TTF_RenderText_Shaded(m_font, (*vIter).first.c_str(), color, bgcolor);
			}

			SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
			SDL_FreeSurface(sText);
			m_destRect.y += m_skipVal;
			m_curItemClearRect.y += m_skipVal;
			++numDisplayed;
		}
			++numProcessed;
	}
	m_destRect.y = m_origY;
	m_curItemClearRect.y = m_origY;

}

void Scroller::draw(vector<string>& selectedOptions) 
{
	SDL_Surface *sText;
	int numProcessed = 0;
	int numDisplayed = 0;
	for(listing_t::iterator vIter = m_listing.begin();
		vIter != m_listing.end() && (numDisplayed <= m_numPerScreen);
		++vIter) {
		if(numProcessed >= m_topItemNum) {
			string str = (*vIter).first;
			if(numProcessed == m_curItemNum) {
				sText = TTF_RenderText_Blended(m_font, str.c_str(), m_curItemColor);
				m_curItemClearRect.w = sText->w;
				m_curItemClearRect.h = sText->h;
				SDL_FillRect(m_screen, &m_curItemClearRect, SDL_MapRGB(m_screen->format, m_curItemBackColor.r, m_curItemBackColor.g, m_curItemBackColor.b));
				m_curItemName = (*vIter).first;
				m_curItemType = (*vIter).second;
			} else {
				//sText = TTF_RenderText_Solid(m_font, (*vIter).first.c_str(), color);
				sText = TTF_RenderText_Blended(m_font, str.c_str(), m_itemColor);
				//sText = TTF_RenderText_Shaded(m_font, (*vIter).first.c_str(), color, bgcolor);
			}

			SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
			SDL_FreeSurface(sText);
			if(numDisplayed < selectedOptions.size()) {
				sText = TTF_RenderText_Blended(m_font, selectedOptions[numDisplayed].c_str(), m_itemColor);
				m_destRect.x += 130;	
				SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );

				m_destRect.x -= 130;	
				SDL_FreeSurface(sText);
			}
			m_destRect.y += m_skipVal;
			m_curItemClearRect.y += m_skipVal;
			++numDisplayed;
		}
			++numProcessed;
	}
	m_destRect.y = m_origY;
	m_curItemClearRect.y = m_origY;

}
