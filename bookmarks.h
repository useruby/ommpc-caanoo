#ifndef __BOOKMARKS_H__
#define __BOOKMARKS_H__

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "libmpdclient.h"
#include "scroller.h"

class Playlist;
class Config;
class StatsBar;

class Bookmarks : public Scroller
{
public:
	typedef std::vector<std::pair<std::string, int> >listing_t;
    Bookmarks(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, SDL_Rect& rect, int skipVal, int numPerScreen, Playlist& pl, Config& config, StatsBar& sb);
    void ls(std::string dir="");

	void updateListing();
	void updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus);
	void processCommand(int command);
	void draw(bool forceRefresh);
	std::string currentItemName();
	std::string currentItemPath();

protected:
	void doSave();
	std::string m_curDir;

	int m_nowPlaying;
	Playlist& m_playlist;
	StatsBar& m_sb;
};

#endif
