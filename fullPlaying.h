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

#ifndef __FULLPLAYING_H__
#define __FULLPLAYING_H__

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "libmpdclient.h"
#include "menuButton.h"
#include "scroller.h"

class Config;
class GuiPos;
class Keyboard;

class FullPlaying  : public Scroller
{
public:
    	FullPlaying(mpd_Connection* mpd, SDL_Surface* bg, SDL_Surface* screen, SDL_Rect& rect, Config& config, Keyboard& kb);
	
	void updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus, bool updatingSongDb);
	int processCommand(int command, GuiPos& guiPos);
	void draw(bool forceRefresh, long timePerFrame, bool inBack);

protected:
	Keyboard& m_keyboard;
	bool m_drawIcons;
	std::vector<MenuButton> m_buttons;
	
};

#endif
