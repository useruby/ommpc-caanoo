#ifndef __PLBROWSER_H__
#define __PLBROWSER_H__

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "libmpdclient.h"
#include "scroller.h"

class Playlist;
class Config;

class PLBrowser : public Scroller
{
public:
	typedef std::vector<std::pair<std::string, int> >listing_t;
    PLBrowser(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, SDL_Rect& rect, 
				Config& config, int skipVal, int numPerScreen, Playlist& pl);
    void ls(std::string dir="");

	void updateListing();
	void updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus);
	void processCommand(int command);
	void draw(bool forceRefresh);
	std::string currentItemName();
	std::string currentItemPath();

protected:
	std::string m_curDir;

	Playlist& m_playlist;
};

#endif
