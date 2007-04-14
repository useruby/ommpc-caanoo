#include "scroller.h"
#include "commandFactory.h"

Scroller::Scroller(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, 
		SDL_Rect& rect,int skipVal, int numPerScreen)
	: m_mpd(mpd)
	, m_screen(screen)
	, m_font(font)
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

void Scroller::draw() 
{
	SDL_Surface *sText;
    SDL_Color color = { 255,255,255, 0 };
	int numProcessed = 0;
	int numDisplayed = 0;
	for(listing_t::iterator vIter = m_listing.begin();
		vIter != m_listing.end() && (numDisplayed <= m_numPerScreen);
		++vIter) {
		if(numProcessed >= m_topItemNum) {
			SDL_Surface *sText;
			if(numProcessed == m_curItemNum) {
    			SDL_Color curColor = { 255,0,255, 0 };
				sText = TTF_RenderText_Solid(m_font, (*vIter).first.c_str(), curColor);
				m_curItemName = (*vIter).first;
				m_curItemType = (*vIter).second;
			} else {
				sText = TTF_RenderText_Solid(m_font, (*vIter).first.c_str(), color);
			}

			SDL_BlitSurface(sText,NULL, m_screen, &m_destRect );
			SDL_FreeSurface(sText);
			m_destRect.y += m_skipVal;
			++numDisplayed;
		}
			++numProcessed;
	}
}
