/*****************************************************************************************

ommpc(One More Music Player Client) - A Music Player Daemon client targetted for the gp2x

Copyright (C) 2007-2008 - Tim Temple(codertimt@gmail.com)

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

#ifndef __ARTBUTTON_H__
#define __ARTBUTTON_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

#include "libmpdclient.h"
#include "config.h"
#include "threadParms.h"
#include <vector>
#include "button.h"

class GuiPos;
class Config;

class ArtButton : public Button
{
public:
	ArtButton(artThreadParms_t& artParms, std::string label, std::string id);
	virtual void init(Config& config, int command);

	bool updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus,
					  int rtmpdStatusChanged, mpd_Status* rtmpdStatus, 
					  bool forceRefresh, mpd_Connection* mpd, Config& config);
	int processCommand(int command, GuiPos& guiPos);
	bool draw2(SDL_Surface* screen, SDL_Surface* bg, bool forceRefresh);
	std::string testCall() { return m_label; };
	void active(bool act) {m_active = act;};
	bool active() {return m_active;};
protected:
	
	bool m_active;
	std::string m_label;
	std::string m_id;
	int m_command;
	TTF_Font* m_font;	
	SDL_Color m_textColor;
	SDL_Color m_itemColor;
	SDL_Surface* m_sText;
	SDL_Rect m_mouseRect;
	SDL_Rect m_destRectB;
	bool m_displayText;
	artThreadParms_t& m_artParms;
	std::string m_artist;
	std::string m_album;
	std::string m_type;
	std::string m_genre;
	std::string m_track;
	std::string m_date;
	bool m_showInfo;
	int m_counter;
	SDL_Rect m_moveRect;
	SDL_Surface* m_artistSurface;
	SDL_Surface* m_albumSurface;
	SDL_Surface* m_typeSurface;
	SDL_Surface* m_genreSurface;
	SDL_Surface* m_trackSurface;
	SDL_Surface* m_dateSurface;
	SDL_Surface* m_rateSurface;
	int m_skipVal;
};

#endif
