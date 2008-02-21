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

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#ifdef GP2X
#include "tslib.h"
#endif
#include "config.h"
#include "browser.h"
#include "plBrowser.h"
#include "bookmarks.h"
#include "playlist.h"
#include "albumArt.h"
#include "overlay.h"
#include "nowPlaying.h"
#include "libmpdclient.h"
#include "threadFunctions.h"
#include "commandFactory.h"
#include "statsBar.h"
#include "helpBar.h"
#include "timer.h"
#include "popup.h"
#include "gp2xregs.h"
#include "guiPos.h"

#include "unistd.h"
#include <dirent.h>

using namespace std;


bool showMainMenu(SDL_Surface* screen, Popup& popup)
{
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_MENU;
	items.push_back(make_pair("  Return to player", (int)Popup::POPUP_CANCEL));
	items.push_back(make_pair("  Save Playlist", (int)Popup::POPUP_SAVE_PL));
	items.push_back(make_pair("  Create Bookmark", (int)Popup::POPUP_BKMRK));
	items.push_back(make_pair("  Launch Program", (int)Popup::POPUP_LAUNCH));
	items.push_back(make_pair("  Detach Client", (int)Popup::POPUP_DETACH));
	items.push_back(make_pair("  Update Mpd Database", (int)Popup::POPUP_MPD_UPDATE));
	items.push_back(make_pair("  Player Options", (int)Popup::POPUP_SHOW_OPTIONS));
	items.push_back(make_pair("  Exit", (int)Popup::POPUP_EXIT));
//	items.push_back(make_pair("", type));	
			
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 200;
	popRect.h = popup.skipVal()*10+15;
	popRect.x = (screen->w - popRect.w) / 2;
	popRect.y = (screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("  Main Menu      ommpc v0.2.0");
	show = true;

	return show;
}

int processMainMenuItem(int action, std::string item)
{
	int rCommand = 0;
	switch(action) {
		case Popup::POPUP_CANCEL:
			rCommand = 0;
			break;
		case Popup::POPUP_LAUNCH:
			rCommand = CMD_LAUNCH_APP;
			break;
		case Popup::POPUP_DETACH:
			rCommand = CMD_DETACH;
			break;
		case Popup::POPUP_EXIT:
			rCommand = CMD_QUIT;
			break;
		case Popup::POPUP_SAVE_PL:
			rCommand = CMD_SAVE_PL;
			break;
		case Popup::POPUP_SHOW_OPTIONS:
			rCommand = CMD_SHOW_OPTIONS;
			break;
		case Popup::POPUP_SAVE_OPTIONS:
			rCommand = CMD_SAVE_OPTIONS;
			break;
		case Popup::POPUP_MPD_UPDATE:
			rCommand = CMD_MPD_UPDATE;
			break;
		case Popup::POPUP_BKMRK:
			rCommand = CMD_SAVE_BKMRK;
			break;
	}
	
	return rCommand;
}

bool showLaunchMenu(SDL_Surface* screen, Popup& popup, string dir)
{
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_MENU;
	DIR * udir = opendir((dir+"/shortcuts/").c_str());

	items.push_back(make_pair("  Cancel", 0));	
	if(udir != NULL) {
		struct dirent * dirent = readdir(udir);

		bool done = false;
		while(dirent != NULL) {
			string ename = dirent->d_name;
			if(ename[0] != '.' && ename.substr(ename.size() - 2) == "sh") {
				items.push_back(make_pair("  "+ename, 6));	
			}
			dirent = readdir(udir);
		}

	}	
			
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 200;
	popRect.h = popup.skipVal()*8+15;
	popRect.x = (screen->w - popRect.w) / 2;
	popRect.y = (screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("  Launch Program");
	show = true;

	return show;
}

int processLaunchMenuItem(int action)
{
	int rCommand = 0;
	switch(action) {
		case Popup::POPUP_CANCEL:
			rCommand = 0;
			break;
		case Popup::POPUP_DO_LAUNCH:
			rCommand = CMD_LAUNCH_PROCESS;
			break;
	}
	
	return rCommand;
}

bool showOptionsMenu(SDL_Surface* screen, Popup& popup, Config& config)
{
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_OPTIONS;
	int itemType = Popup::POPUP_SAVE_OPTIONS;
	items.push_back(make_pair("  Clock Speed", 21));
	items.push_back(make_pair("  Show Album Art", 22));
	items.push_back(make_pair("  Skin", 23));
	items.push_back(make_pair("  Software Vol.", 24));
	items.push_back(make_pair("  Save", 8));
	items.push_back(make_pair("  Cancel", 0));
	
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 200;
	popRect.h = popup.skipVal()*8+15;
	popRect.x = (screen->w - popRect.w) / 2;
	popRect.y = (screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("       Player Options");
	show = true;

	return show;
}

int processOptionsMenuItem(int action, Popup& popup)
{
	int rCommand = 0;
	switch(action) {
		case Popup::POPUP_CANCEL:
			rCommand = 0;
			break;
		case Popup::POPUP_SAVE_OPTIONS:
			popup.saveOptions();
			break;
	}
	
	return rCommand;
}

void initVolumeScale(vector<int>& volumeScale, bool f200, string softVol)
{
	if(softVol == "on") {
		int softVolumeScale[21] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
		for(int i=0; i<22; ++i)
			volumeScale.push_back(softVolumeScale[i]);	

	} else {
		if(f200) {
			int f200VolumeScale[21] = {0,1,3,5,8,12,14,18,22,26,30,34,38,42,46,50,56,62,68,74,80};
			for(int i=0; i<22; ++i)
				volumeScale.push_back(f200VolumeScale[i]);	

		} else {
			int f100VolumeScale[21] = {0,4,8,12,16,20,25,30,35,40,45,50,55,60,65,70,76,82,88,94,100};

			for(int i=0; i<22; ++i)
				volumeScale.push_back(f100VolumeScale[i]);	
		}
	}

}

int main ( int argc, char** argv )
{
	bool initVolume = true;
	bool f200 = false;
	struct stat stFileInfo;
	if(stat("/dev/touchscreen/wm97xx",&stFileInfo) == 0)
		f200 = true;

	vector<int> volumeScale;
	int pid;
	int wpid;
	int status;

	pid = fork();

	if(pid ==-1) {
		cout << "Fork failed" << endl;
		exit(1);
	} else if(pid == 0) { //child..attempt to launch mpd
		execlp("./mpd/mpd", "./mpd/mpd", "mpd.conf",  NULL);
		cout << errno << " " << strerror(errno) << endl;
		exit(1);
	} else {

		//we wait and then assume mpd is now running or was already running
		if(waitpid(pid,&status,0)<0)
		{
			cout << "waitpid failed" << endl;
			exit(-1);
		}
		try {
			Config config;
			GuiPos guiPos;
	
			initVolumeScale(volumeScale, f200, config.getItem("softwareVolume"));
			// initialize SDL video
			if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0 )
			{
				printf( "Unable to init SDL: %s\n", SDL_GetError() );
				return 1;
			}

			cout << SDL_NumJoysticks() << " joysticks were found." << endl;

			for(int i=0; i < SDL_NumJoysticks(); i++ ) 
			{
				cout << "    " <<  SDL_JoystickName(i) << endl;
				SDL_JoystickOpen(i);	
			}

			if (TTF_Init() == -1)
			{
				printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
				//Put here however you want to do about the error.
				//you could say:
				return true;
				//Or:
				//exit(1);
			}

			// create a new window
			SDL_Surface* screen = SDL_SetVideoMode(320, 240, 32,
					SDL_HWSURFACE|SDL_DOUBLEBUF);
			if ( !screen )
			{
				printf("Unable to set 320x240 video: %s\n", SDL_GetError());
				return 1;
			}
#ifdef GP2X
			SDL_ShowCursor(SDL_DISABLE);
#endif

			SDL_Rect mainRect = { config.getItemAsNum("sk_main_x"),
				config.getItemAsNum("sk_main_y"),
				config.getItemAsNum("sk_main_width"),
				config.getItemAsNum("sk_main_height")
			};
			SDL_Rect artRect = { config.getItemAsNum("sk_art_x"),
				config.getItemAsNum("sk_art_y"),
				config.getItemAsNum("sk_art_width"),
				config.getItemAsNum("sk_art_height")
			};
			SDL_Rect nowPlayingRect = { config.getItemAsNum("sk_nowPlaying_x"),
				config.getItemAsNum("sk_nowPlaying_y"),
				config.getItemAsNum("sk_nowPlaying_width"),
				config.getItemAsNum("sk_nowPlaying_height")
			};
			SDL_Rect statsRect = { config.getItemAsNum("sk_stats_x"),
				config.getItemAsNum("sk_stats_y"),
				config.getItemAsNum("sk_stats_width"),
				config.getItemAsNum("sk_stats_height")
			};
			SDL_Rect helpRect = { config.getItemAsNum("sk_help_x"),
				config.getItemAsNum("sk_help_y"),
				config.getItemAsNum("sk_help_width"),
				config.getItemAsNum("sk_help_height")
			};
			SDL_Rect menuRect = { config.getItemAsNum("sk_menu_x"),
				config.getItemAsNum("sk_menu_y"),
				config.getItemAsNum("sk_menu_width"),
				config.getItemAsNum("sk_menu_height")
			};
			SDL_Rect clearRect = { 0,0,screen->w, screen->h};

			SDL_Rect popRect;
			popRect.w = 150;
			popRect.h = 100;
			popRect.x = (screen->w - 150) / 2;
			popRect.y = (screen->h - 100) / 2;


			threadParms_t threadParms;
			threadParms.mpd = mpd_newConnection(config.getItem("host").c_str(), 
					config.getItemAsNum("port"),
					config.getItemAsNum("timeout"));
			threadParms.mpdStatus = NULL;
			threadParms.mpdStatusChanged = 0;
			threadParms.mpdReady = false;
			threadParms.pollStatusDone = false;
			threadParms.lockConnection = SDL_CreateMutex();

			//enable status checking for interactive commands...
			mpd_Status* rtmpdStatus = NULL;
			int rtmpdState = MPD_STATUS_STATE_UNKNOWN;
			int rtmpdStatusChanged = 0;

			GP2XRegs gp2xRegs;
			mpd_sendStatusCommand(threadParms.mpd);
			rtmpdStatus = mpd_getStatus(threadParms.mpd);
			mpd_finishCommand(threadParms.mpd);
			if(rtmpdStatus != NULL)
				rtmpdState = rtmpdStatus->state;
			mpd_freeStatus(rtmpdStatus);
			rtmpdStatus = NULL;
#ifdef GP2X
			if (rtmpdState == MPD_STATUS_STATE_PLAY) {
				mpd_sendPauseCommand(threadParms.mpd, 1);
				mpd_finishCommand(threadParms.mpd);
			}
			gp2xRegs.setClock(config.getItemAsNum("cpuSpeed"));
#endif
			TTF_Font* font = TTF_OpenFont(config.getItem("sk_font_main").c_str(),
										  config.getItemAsNum("sk_font_main_size"));
			int skipVal = TTF_FontLineSkip( font ) * config.getItemAsFloat("sk_font_main_extra_spacing");
			int numPerScreen = (mainRect.h-(2*skipVal))/skipVal+1;

			string skinName = config.getItem("skin");
			SDL_Surface * tmpbg = IMG_Load(string("skins/"+skinName+"/bg.png").c_str());
			if (!tmpbg)
				printf("Unable to load skin image: %s\n", SDL_GetError());

			SDL_Surface * bg = SDL_DisplayFormat(tmpbg);
			
			int popPerScreen = (popRect.h-(2*skipVal))/skipVal;
cout << "before pop" << endl;
			Popup popup(threadParms.mpd, screen, config, popRect, skipVal, popPerScreen, gp2xRegs);
			Browser browser(threadParms.mpd, screen, bg, font, mainRect, config, skipVal, numPerScreen);
			Playlist playlist(threadParms.mpd, screen, bg, font, config, mainRect, skipVal, numPerScreen);
			PLBrowser plBrowser(threadParms.mpd, screen, bg, font, mainRect, config, skipVal, numPerScreen, playlist);
cout << "before pl" << endl;
			NowPlaying playing(threadParms.mpd, threadParms.lockConnection, screen, bg, config, nowPlayingRect, playlist);
cout << "before stat" << endl;
			StatsBar statsBar(threadParms.mpd, threadParms.lockConnection, screen, bg, config, statsRect, initVolume, playlist, f200, volumeScale);
cout << "before help" << endl;
			HelpBar helpBar(threadParms.mpd, screen, bg, config, helpRect);
			Overlay overlay(threadParms.mpd, screen, config, clearRect, playlist);
			Bookmarks bookmarks(threadParms.mpd, screen, bg, font, mainRect, skipVal, numPerScreen, playlist, config, statsBar);
			CommandFactory commandFactory(threadParms.mpd, volumeScale);

			int curMode = 0;	
			int volume = 10;
			int isPlaying = 0;
			try {
				Config stateConfig(".ommcState");
				curMode = stateConfig.getItemAsNum("mode");
				volume = stateConfig.getItemAsNum("vol");	
				isPlaying = stateConfig.getItemAsNum("playing");

				if(volume == 0 || volume > 20)
					volume = 10;
				mpd_sendSetvolCommand(threadParms.mpd, volumeScale[volume]);
				mpd_finishCommand(threadParms.mpd);
				initVolume = true;
			} catch(exception& e) {
				//carry on
			}

			//polling thread that gets current status from mpd
			SDL_Thread *statusThread;
			statusThread = SDL_CreateThread(pollMpdStatus, &threadParms);
			if(statusThread == NULL) {
				cout << "unable to create status thread" << endl;
				return -1;
			}
			//art loading thread, just so we don't wait  
			SDL_Thread* artThread;
			artThreadParms_t artParms;
			artParms.doArtLoad = false;
			artParms.done = false;
			artParms.artSurface = NULL;	
			artParms.destWidth = config.getItemAsNum("sk_art_image_width");
			artParms.destHeight = config.getItemAsNum("sk_art_image_height");
			artThread = SDL_CreateThread(loadAlbumArt, &artParms);
			if(artThread == NULL) {
				cout << "unable to create album art thread" << endl;
				return -1;
			}


			AlbumArt albumArt(threadParms.mpd, screen, bg, config, artRect, artParms);

			bool done = false;
			bool killMpd = false;
			bool launchProcess = false;
			string launchProcessName = "";
			int command = -1;
			bool keysHeld[401] = {false};
			int repeatDelay = 0;
			bool forceRefresh = true;
			bool processedEvent = false;
			bool repeat = false;
			bool random = false;
			int frame = 0;
			bool popupVisible = false;
			bool overlayVisible = false;
			SDL_Color backColor;
			config.getItemAsColor("sk_screen_color", backColor.r, backColor.g, backColor.b);

			Timer timer;	
			Timer limiter;
			Timer delayTimer;
			delayTimer.stop();
			int fps=0;

#ifdef GP2X
			struct tsdev *ts;
			ts = ts_open("/dev/touchscreen/wm97xx", 0);

			if(!ts) {
				cout << "tsopen errero" << endl;
				return 1;
			}

			int ret = ts_config(ts);
			if(ret< 0) {
				cout << "tsconfig errero   " <<  ret << endl;
				return 1;
			}
			struct ts_sample sample;
#endif

			SDL_Event event;
			bool mouseUp= false;
			bool mouseDown= false;
			while (!done)
			{
 				SDL_mutexP(threadParms.lockConnection);
				// let's start with checking some polled status items
				if(threadParms.mpdStatusChanged & VOL_CHG) {
					//volume = threadParms.mpdStatus->volume;
				}
				if(threadParms.mpdStatusChanged & RND_CHG) {
					random = threadParms.mpdStatus->random;
				}
				if(threadParms.mpdStatusChanged & RPT_CHG) {
					repeat = threadParms.mpdStatus->repeat;
				}
#ifdef GP2X
				if(ts_read(ts, &sample, 1) == 1) {
//					cout << "mouse Down " << sample.x << " x " << sample.y << endl;
//					cout << "flags " << mouseUp << "   " << mouseDown << endl;
					delayTimer.start();
					guiPos.curX = sample.x;
					guiPos.curY = sample.y;
					mouseDown = true;
					mouseUp = false;
					keysHeld[400] = true;
					processedEvent = true;
				} else {
					if(mouseDown && !mouseUp) {
//					cout << "mouse Up" << endl;
//					cout << "flags " << mouseUp << "   " << mouseDown << endl << endl;
						keysHeld[400] = false;
						mouseUp = true;
						mouseDown = false;
						delayTimer.stop();
						repeatDelay = 1;
						processedEvent = true;
					} else {
						mouseUp = false;
						mouseDown = false;
					}
				}
#endif
				if (SDL_PollEvent(&event) && !processedEvent)
				{
					if(event.type == SDL_MOUSEMOTION) {
 						SDL_mutexV(threadParms.lockConnection);
						continue;
					}
					switch (event.type) {
					//this stops the mouse motion event from taking 
					//too much time and screwing sutff up.
						case SDL_KEYDOWN:
							keysHeld[event.key.keysym.sym] = true;
							delayTimer.start();
						break;
						case SDL_KEYUP:
							keysHeld[event.key.keysym.sym] = false;
							repeatDelay = 1;
							delayTimer.stop();
						break;
						case SDL_JOYBUTTONDOWN:
							keysHeld[event.jbutton.button] = true;
							delayTimer.start();
						break;
						case SDL_JOYBUTTONUP:
							keysHeld[event.jbutton.button] = false;
							repeatDelay = 1;
							delayTimer.stop();
						break;
#ifndef GP2X
						case SDL_MOUSEBUTTONDOWN:
							keysHeld[400] = true;
							delayTimer.start();
							mouseDown = true;
							SDL_GetMouseState(&guiPos.curX, &guiPos.curY);
								
						break;
						case SDL_MOUSEBUTTONUP:
							keysHeld[400] = false;
							repeatDelay = 1;
							delayTimer.stop();
							mouseUp = true;
						break;
#endif
						default:
							processedEvent = false;
					}
					processedEvent = true;
				} // end of message processing

				long delayTime = delayTimer.check();
				if(processedEvent || repeatDelay > 0) {	
					if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN
						|| mouseDown == true) {
						++repeatDelay;	
#ifndef GP2X
						mouseDown = false;
#endif
					}
					if(!gp2xRegs.screenIsOff()) 
						command = commandFactory.getCommand(keysHeld, curMode, repeatDelay, popupVisible, overlayVisible, volume, delayTime);
					else
						command = commandFactory.getCommandWhileLocked(keysHeld, curMode, repeatDelay, popupVisible, delayTime);
		
					if (event.type == SDL_KEYUP || event.type == SDL_JOYBUTTONUP
						|| mouseUp == true)  {
						repeatDelay = 0;
						event.type = -1;
#ifndef GP2X
						mouseUp = false;
#endif
					}
					processedEvent = false;
				}

//				if(!gp2xRegs.screenIsOff())  {
					if(overlayVisible) {
						command = overlay.processCommand(command, guiPos, overlayVisible);
						if(command == CMD_SHOW_MENU) {
							overlayVisible = false;
							forceRefresh = true;
						}
					} else {
					command = overlay.processCommand(command, guiPos, overlayVisible);
					}	
					if(popupVisible) {
						command = popup.processCommand(command, guiPos);
						switch(command) {
							case CMD_POP_SELECT:
								int action = popup.selectedAction();
								std::string selText = popup.selectedText();
								if(action == Popup::POPUP_DO_SAVE_PL) {
									if(selText != "Cancel") {
										mpd_sendSaveCommand(threadParms.mpd, selText.c_str());
										mpd_finishCommand(threadParms.mpd);
										plBrowser.updateListing();
										playlist.setNextNumOnSave();
									}
								} else if(action == Popup::POPUP_DO_LAUNCH){
									char pwd[129];
									getcwd(pwd, 128);
									string pwdStr(pwd);
									launchProcessName = pwdStr + "/shortcuts/"+
										selText.substr(2);
									command = processLaunchMenuItem(action);
								} else if(action == Popup::POPUP_SAVE_OPTIONS) {
									command = processOptionsMenuItem(action, popup);
								} else {
									command = processMainMenuItem(action, selText);
								}
								forceRefresh = true;
								popupVisible = false;	
								break;
						}
					}
				//}
				
				switch(command) {
					case CMD_QUIT:
						done = true;
						killMpd = true;
						popupVisible = false;
						break;
					case CMD_DETACH:
						done = true;
						popupVisible = false;
						break;
					case CMD_LAUNCH_PROCESS:
						done = true;
						launchProcess = true;
						popupVisible = false;
						break;
					case CMD_STOP:
						mpd_sendStopCommand(threadParms.mpd);
						mpd_finishCommand(threadParms.mpd);
						break;
					case CMD_TOGGLE_SCREEN:
						gp2xRegs.toggleScreen();
						break;
					case CMD_VOL_UP:
						if(volume < 20) {
							++volume;
							mpd_sendSetvolCommand(threadParms.mpd, volumeScale[volume]);
							mpd_finishCommand(threadParms.mpd);
							rtmpdStatusChanged += VOL_CHG;
							mpd_sendStatusCommand(threadParms.mpd);
							rtmpdStatus = mpd_getStatus(threadParms.mpd);
							mpd_finishCommand(threadParms.mpd);
						}
						break;
					case CMD_VOL_DOWN:
						if(volume > 0) {
							--volume;
							mpd_sendSetvolCommand(threadParms.mpd, volumeScale[volume]);
							mpd_finishCommand(threadParms.mpd);
							rtmpdStatusChanged += VOL_CHG;
							mpd_sendStatusCommand(threadParms.mpd);
							rtmpdStatus = mpd_getStatus(threadParms.mpd);
							mpd_finishCommand(threadParms.mpd);
						}
						break;
					case CMD_MODE_RANDOM:
						random = !random;
						mpd_sendRandomCommand(threadParms.mpd, random);
						mpd_finishCommand(threadParms.mpd);
						rtmpdStatusChanged += RND_CHG;
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						mpd_finishCommand(threadParms.mpd);
						break;
					case CMD_MODE_REPEAT:
						repeat = !repeat;
						mpd_sendRepeatCommand(threadParms.mpd, repeat);
						mpd_finishCommand(threadParms.mpd);
						rtmpdStatusChanged += RPT_CHG;
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						mpd_finishCommand(threadParms.mpd);
						break;
					case CMD_RAND_RPT:
						if(!random && !repeat) {
							random = true;
							mpd_sendRandomCommand(threadParms.mpd, random);
							mpd_finishCommand(threadParms.mpd);
							rtmpdStatusChanged += RND_CHG;
						} else if(random && !repeat) {
							repeat = true;
							mpd_sendRepeatCommand(threadParms.mpd, repeat);
							mpd_finishCommand(threadParms.mpd);
							rtmpdStatusChanged += RPT_CHG;
						} else if(random && repeat) {
							random = false;
							mpd_sendRandomCommand(threadParms.mpd, random);
							mpd_finishCommand(threadParms.mpd);
							rtmpdStatusChanged += RND_CHG;
						} else if(!random && repeat) {
							repeat = false;
							mpd_sendRepeatCommand(threadParms.mpd, repeat);
							mpd_finishCommand(threadParms.mpd);
							rtmpdStatusChanged += RPT_CHG;
						}
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						mpd_finishCommand(threadParms.mpd);
						break;
					case CMD_TOGGLE_MODE:
						++curMode;
						if(curMode == 4)
							curMode = 0;
						forceRefresh = true;
						break;
					case CMD_SAVE_PL:
						popupVisible = playlist.showSaveDialog(popup);
						break;
					case CMD_SAVE_BKMRK:
						bookmarks.doSave();
						break;
					case CMD_POP_CANCEL:
						popupVisible = false;	
						break;
					case CMD_SHOW_MENU:
						popupVisible = showMainMenu(screen, popup);
						break;
					case CMD_SHOW_OVERLAY:
						overlayVisible = !overlayVisible;
						forceRefresh = true;
						break;
					case CMD_LAUNCH_APP:
						char pwd[129];
						getcwd(pwd, 128);
						popupVisible = showLaunchMenu(screen, popup, pwd);
						break;
					case CMD_SHOW_OPTIONS:
						popupVisible = showOptionsMenu(screen, popup, config);
						break;
					case CMD_MPD_UPDATE:
						mpd_sendUpdateCommand(threadParms.mpd, "");
						mpd_finishCommand(threadParms.mpd);
						break;	
				}


				//if(!gp2xRegs.screenIsOff())  {
					// DRAWING STARTS HERE
					if(forceRefresh) {
						//					SDL_SetClipRect(screen, &clearRect);
						//					SDL_FillRect(screen, &clearRect, SDL_MapRGB(screen->format, backColor.r, backColor.g, backColor.b));
					}

					playlist.updateStatus(threadParms.mpdStatusChanged, 
							threadParms.mpdStatus, 
							rtmpdStatusChanged, rtmpdStatus, repeatDelay);
					playing.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus, 
							rtmpdStatusChanged, rtmpdStatus);
					playing.draw(forceRefresh);
					statsBar.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus,
							rtmpdStatusChanged, rtmpdStatus, forceRefresh);
					statsBar.draw(forceRefresh, fps);
					if(config.getItem("showAlbumArt") == "true") {
						albumArt.updateStatus(threadParms.mpdStatusChanged, 
								threadParms.mpdStatus,
								rtmpdStatusChanged, rtmpdStatus);
						albumArt.draw(forceRefresh);
					}
					switch (curMode) {
						case 0:
							browser.updateStatus(threadParms.mpdStatusChanged, 
									threadParms.mpdStatus);
							curMode = browser.processCommand(command, guiPos);
							browser.draw(forceRefresh);
							break;
						case 1:
							playlist.processCommand(command, rtmpdStatusChanged, rtmpdStatus, repeatDelay, volume, delayTime, guiPos);
							playlist.draw(forceRefresh);

							break;
						case 2:
							plBrowser.updateStatus(threadParms.mpdStatusChanged, 
									threadParms.mpdStatus);
							curMode = plBrowser.processCommand(command, curMode, guiPos);
							plBrowser.draw(forceRefresh);
							break;
						case 3:
							bookmarks.updateStatus(threadParms.mpdStatusChanged, 
									threadParms.mpdStatus);
							curMode = bookmarks.processCommand(command, guiPos);
							bookmarks.draw(forceRefresh);
							break;
						default: 
							playlist.updateStatus(threadParms.mpdStatusChanged, 
									threadParms.mpdStatus, 
									rtmpdStatusChanged, rtmpdStatus, repeatDelay);
							playlist.processCommand(command, rtmpdStatusChanged, rtmpdStatus, repeatDelay, volume, delayTime, guiPos);
							playlist.draw(forceRefresh);
							break;
					}
					helpBar.draw(curMode, forceRefresh);

					if(popupVisible)
						popup.draw();
					if(overlayVisible) {
						overlay.draw(forceRefresh);	
						forceRefresh = true;
					} else {
						forceRefresh = false;
					}

					threadParms.mpdStatusChanged = 0;
					rtmpdStatusChanged = 0;
					if(rtmpdStatus != NULL) {
						mpd_freeStatus(rtmpdStatus);
						rtmpdStatus = NULL;
					}
					command = 0;

					if(done) {
						threadParms.pollStatusDone = true;
						artParms.done = true;
					}

#ifdef GP2X
					// start playing, if it was playing when we last exited,
					// or if we had just paused an already-playing MPD daemon
					if((rtmpdState == MPD_STATUS_STATE_PAUSE && isPlaying)
							|| rtmpdState == MPD_STATUS_STATE_PLAY) {
						mpd_sendPauseCommand(threadParms.mpd, 0);
						mpd_finishCommand(threadParms.mpd);
						// only do this once
						rtmpdState = MPD_STATUS_STATE_UNKNOWN;
					}
#endif
					// mark status polling as ready
					threadParms.mpdReady = true;
					SDL_mutexV(threadParms.lockConnection);
					
					if(!gp2xRegs.screenIsOff())  {
						SDL_Flip(screen);
						SDL_Delay(75);
					} else {
						SDL_Delay(165);
					}
					++frame;
					if(timer.check() > 3000000) //5minutes
					{
						timer.stop();
						int elapsed = timer.elapsed()/1000000;
						fps = frame/elapsed;
						//cout << "fps " << fps << endl;
						frame = 0;
						timer.start();
					}
				//} else {
				//	SDL_mutexV(threadParms.lockConnection);
				//	SDL_Delay(150);
			//	}
				// DRAWING ENDS HERE
					
			} // end main loop

			// Quit SDL
			SDL_WaitThread(artThread, NULL);
			SDL_WaitThread(statusThread, NULL);
			SDL_Quit();

			// get playing state
			mpd_sendStatusCommand(threadParms.mpd);
			rtmpdStatus = mpd_getStatus(threadParms.mpd);
			mpd_finishCommand(threadParms.mpd);
			if(rtmpdStatus != NULL) {
				if (rtmpdStatus->state == MPD_STATUS_STATE_PLAY)
					isPlaying = 1;
				else
					isPlaying = 0;
				mpd_freeStatus(rtmpdStatus);
				rtmpdStatus = NULL;
			} else isPlaying = 0;

			// pause MPD
			if (isPlaying && killMpd) {
				mpd_sendPauseCommand(threadParms.mpd, 1);
				mpd_finishCommand(threadParms.mpd);
			}

			//save current state
			ofstream ommcState(".ommcState", ios::out|ios::trunc);
			ommcState << "mode=" << curMode << endl;
			ommcState << "vol=" << volume << endl;
			ommcState << "playing=" << isPlaying << endl;
			ommcState.close();

#ifdef GP2X
			// kill MPD
			if(killMpd) {
				mpd_sendKillCommand(threadParms.mpd);
				mpd_finishCommand(threadParms.mpd);
				sync();
				// Note: This causes at least the playlist file to be
				// written before syncing, because MPD writes it
				// before closing the socket.  It is possible that the
				// PID file is not cleaned up in time for this,
				// though.
			} else {
				sync();
			}
	
			if(launchProcess) {
				cout << "launching " << launchProcessName << endl;
				execlp(launchProcessName.c_str(), launchProcessName.c_str(), NULL);
			}
#endif
			// free connection data
			mpd_closeConnection(threadParms.mpd);

			printf("Exited cleanly\n");
			return 0;
			//    }
	} catch (std::exception& e) {
		cout << "Process Exception: " << e.what() << endl;
	}

}
}
