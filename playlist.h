#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config.h"
#include "libmpdclient.h"
#include "scroller.h"

class Popup;

class Playlist : public Scroller
{
public:
	typedef std::vector<std::pair<std::string, int> >listing_t;

	Playlist(mpd_Connection* mpd, SDL_Surface* screen, TTF_Font* font, Config& config, SDL_Rect& rect, int skipVal, int numPerScreen);
	void load(std::string dir);
	void updateStatus(int mpdStatusChanged, mpd_Status* mpdStatus, int repeatDelay);
    void processCommand(int event, int delay);
    void draw(bool force);
	std::string currentItemName();
	std::string currentItemPath();

	void makeNowPlayingVisible();
	bool showSaveDialog(Popup& popup);
	int getRand(int max);
	void initRandomPlaylist();
	void initNewPlaylist();
	void initName(std::string name);
	void save();
	std::string nowPlayingText(int song);
	void nowPlaying(int song);
protected:
	std::string m_curDir;

	Config& m_config;
	int m_view;
	int m_curElapsed;
	int m_curState;
	int m_nowPlaying;
	int m_random;
	bool m_otg;
	bool m_modified;	
	std::vector<std::string> m_all;	
	std::string m_name;
	int m_id;

};

#endif
	
