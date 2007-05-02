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
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "browser.h"
#include "plBrowser.h"
#include "bookmarks.h"
#include "playlist.h"
#include "albumArt.h"
#include "nowPlaying.h"
#include "libmpdclient.h"
#include "threadFunctions.h"
#include "commandFactory.h"
#include "statsBar.h"
#include "helpBar.h"
#include "timer.h"
#include "popup.h"
#include "gp2xregs.h"

#include "unistd.h"
#include <dirent.h>

#define GP2X 


using namespace std;


bool showMainMenu(SDL_Surface* screen, Popup& popup)
{
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_MENU;
	items.push_back(make_pair("Return to player", 0));	
	items.push_back(make_pair("Save Playlist", 1));	
	items.push_back(make_pair("Launch Program", 2));	
	items.push_back(make_pair("Detach Client", 3));	
	items.push_back(make_pair("Update Mpd Database", 9));	
	items.push_back(make_pair("Player Options", 7));	
	items.push_back(make_pair("Exit", 4));	
//	items.push_back(make_pair("", type));	
			
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 180;
	popRect.h = popup.skipVal()*9+15;
	popRect.x = (screen->w - popRect.w) / 2;
	popRect.y = (screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("Main Menu");
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
	}
	
	return rCommand;
}

bool showLaunchMenu(SDL_Surface* screen, Popup& popup, string dir)
{
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_MENU;
	DIR * udir = opendir((dir+"shortcuts/").c_str());

	items.push_back(make_pair("Cancel", 0));	
	if(udir != NULL) {
		struct dirent * dirent = readdir(udir);

		bool done = false;
		while(dirent != NULL) {
			string ename = dirent->d_name;
			if(ename[0] != '.' && ename.substr(ename.size() - 2) == "sh") {
				items.push_back(make_pair(ename, 6));	
			}
			dirent = readdir(udir);
		}

	}	
			
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 180;
	popRect.h = popup.skipVal()*7+15;
	popRect.x = (screen->w - popRect.w) / 2;
	popRect.y = (screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("Launch Program");
	show = true;

	return show;
}

int processLaunchMenuItem(int action, std::string script)
{
	int rCommand = 0;
	switch(action) {
		case Popup::POPUP_CANCEL:
			rCommand = 0;
			break;
		case Popup::POPUP_DO_LAUNCH:
			SDL_Quit();
			execlp(script.c_str(), script.c_str());
			break;
	}
	
	return rCommand;
}

bool showOptionsMenu(SDL_Surface* screen, Popup& popup, Config& config)
{
	string dir = config.getItem("programRoot");
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_OPTIONS;
	int itemType = Popup::POPUP_SAVE_OPTIONS;
	items.push_back(make_pair("Clock Speed", 21));
	items.push_back(make_pair("Show Album Art", 22));
	items.push_back(make_pair("Skin", 23));
	items.push_back(make_pair("Save", 8));
	items.push_back(make_pair("Cancel", 0));
	
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 180;
	popRect.h = popup.skipVal()*7+15;
	popRect.x = (screen->w - popRect.w) / 2;
	popRect.y = (screen->h - popRect.h) / 2;
	popup.setSize(popRect);
	popup.setTitle("Player Options");
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


int main ( int argc, char** argv )
{
	int pid;
	int wpid;
	int status;

	pid = fork();

	if(pid ==-1) {
		cout << "Fork failed" << endl;
	} else if(pid == 0) { //child..attempt to launch mpd
		execlp("./mpd", "./mpd", "mpd.conf",  NULL);
		cout << errno << " " << strerror(errno) << endl;
	} else {

		//we wait and then assume mpd is now running or was already running
		if(waitpid(pid,&status,0)<0)
		{
			cout << "waitpid failed" << endl;
			exit(-1);
		}

		try {
			Config config;

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

			// make sure SDL cleans up before exit
			atexit(SDL_Quit);

			// create a new window
			SDL_Surface* screen = SDL_SetVideoMode(320, 240, 32,
					SDL_HWSURFACE|SDL_DOUBLEBUF);
			if ( !screen )
			{
				printf("Unable to set 320x240 video: %s\n", SDL_GetError());
				return 1;
			}
			SDL_ShowCursor(SDL_DISABLE);

			// load an image
			SDL_Surface* bmp = IMG_Load("bg.jpg");
			if (!bmp)
			{
				printf("Unable to load bitmap: %s\n", SDL_GetError());
				return 1;
			}

			// centre the bitmap on screen
			SDL_Rect dstrect;
			dstrect.x = (screen->w - bmp->w) / 2;
			dstrect.y = (screen->h - bmp->h) / 2;

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
			threadParms.mpdStatus;
			threadParms.mpdStatusChanged = 0;
			threadParms.pollStatusDone = false;
			threadParms.lockConnection = SDL_CreateMutex();
			
			GP2XRegs gp2xRegs;
#ifdef GP2X
			mpd_sendPauseCommand(threadParms.mpd, 1);
			mpd_finishCommand(threadParms.mpd);
			//gp2xRegs.setClock(config.getItemAsNum("cpuSpeed"));
			mpd_sendPauseCommand(threadParms.mpd, 0);
			mpd_finishCommand(threadParms.mpd);
#endif	
			TTF_Font* font = TTF_OpenFont( "Vera.ttf", 10 );
			int skipVal = TTF_FontLineSkip( font );
			int numPerScreen = (mainRect.h-(2*skipVal))/skipVal;

			Popup popup(threadParms.mpd, screen, config, popRect, skipVal, numPerScreen, gp2xRegs);
			Browser browser(threadParms.mpd, screen, font, mainRect, config, skipVal, numPerScreen);
			Playlist playlist(threadParms.mpd, screen, font, config, mainRect, skipVal, numPerScreen);
			PLBrowser plBrowser(threadParms.mpd, screen, font, mainRect, config, skipVal, numPerScreen, playlist);
			NowPlaying playing(threadParms.mpd, threadParms.lockConnection, screen, config, nowPlayingRect, playlist);
			StatsBar statsBar(threadParms.mpd, threadParms.lockConnection, screen, config, statsRect);
			HelpBar helpBar(threadParms.mpd, screen, config, helpRect);
			Bookmarks bookmarks(threadParms.mpd, screen, font, mainRect, skipVal, numPerScreen, playlist, config, statsBar);
			CommandFactory commandFactory;

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
			artParms.artSurface = NULL;	
			artParms.destWidth = artRect.w;
			artParms.destHeight = artRect.h;
			artThread = SDL_CreateThread(loadAlbumArt, &artParms);
			if(artThread == NULL) {
				cout << "unable to create album art thread" << endl;
				return -1;
			}

			AlbumArt albumArt(threadParms.mpd, screen, config, artRect, artParms);

			//enable status checking for interactive commands...
			mpd_Status* rtmpdStatus = NULL;
			int rtmpdStatusChanged = 0;

			bool done = false;
			bool killMpd = false;
			int command = -1;
			bool keysHeld[323] = {false};
			int curMode = 1;	
			int repeatDelay = 0;
			bool forceRefresh = true;
			bool processedEvent = false;
			int volume = 50;
			bool repeat = false;
			bool random = false;
			int frame = 0;
			bool popupVisible = false;

			try {
				SDL_mutexP(threadParms.lockConnection);
				Config stateConfig(".ommcState");
				curMode = stateConfig.getItemAsNum("mode");
				volume = stateConfig.getItemAsNum("vol");	
				mpd_sendSetvolCommand(threadParms.mpd, -100);
				mpd_finishCommand(threadParms.mpd);
				mpd_sendSetvolCommand(threadParms.mpd, volume);
				mpd_finishCommand(threadParms.mpd);

				playlist.loadState(stateConfig);
				SDL_mutexV(threadParms.lockConnection);
			} catch(exception& e) {
				SDL_mutexV(threadParms.lockConnection);
				//carry on
			}

			Timer timer;	
			while (!done)
			{
				SDL_mutexP(threadParms.lockConnection);
				// let's start with checking some polled status items
				if(threadParms.mpdStatusChanged & VOL_CHG) {
					volume = threadParms.mpdStatus->volume;
				}
				if(threadParms.mpdStatusChanged & RND_CHG) {
					random = threadParms.mpdStatus->random;
				}
				if(threadParms.mpdStatusChanged & RPT_CHG) {
					repeat = threadParms.mpdStatus->repeat;
				}
				// message processing loop
				SDL_Event event;
				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_KEYDOWN) {
						keysHeld[event.key.keysym.sym] = true;
					}
					if (event.type == SDL_KEYUP) {
						keysHeld[event.key.keysym.sym] = false;
						repeatDelay = 1;
					}
					if(event.type == SDL_JOYBUTTONDOWN)  {
						keysHeld[event.jbutton.button] = true;
					}
					if(event.type == SDL_JOYBUTTONUP) {
						keysHeld[event.jbutton.button] = false;
						repeatDelay = 1;
					}
					/*
					   if(event.type == SDL_MOUSEBUTTONDOWN)
					   keysHeld[event.button.button] = true;
					   if(event.type == SDL_MOUSEBUTTONUP)
					   keysHeld[event.button.button] = false;
					   */
					processedEvent = true;
				} // end of message processing

				if(processedEvent || repeatDelay > 0) {	
					if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN) 
						++repeatDelay;
					command = commandFactory.getCommand(keysHeld, curMode, repeatDelay, popupVisible);
					if (event.type == SDL_KEYUP || event.type == SDL_JOYBUTTONUP) 
						repeatDelay = 0;
					processedEvent = false;
				}

				if(popupVisible) {
					command = popup.processCommand(command);
					switch(command) {
						case CMD_POP_SELECT:
							int action = popup.selectedAction();
							std::string selText = popup.selectedText();
							if(action == Popup::POPUP_DO_SAVE_PL) {
								if(selText != "Cancel") {
									mpd_sendSaveCommand(threadParms.mpd, selText.c_str());
									mpd_finishCommand(threadParms.mpd);
									plBrowser.updateListing();
								}
							} else if(action == Popup::POPUP_DO_LAUNCH){
								command = processLaunchMenuItem(action, config.getItem("programRoot") + "shortcuts/" + selText);
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

				switch(command) {
					case CMD_QUIT:
						done = true;
						killMpd = true;
						break;
					case CMD_DETACH:
						done = true;
						break;
					case CMD_STOP:
						mpd_sendStopCommand(threadParms.mpd);
						mpd_finishCommand(threadParms.mpd);
						break;
					case CMD_TOGGLE_SCREEN:
						gp2xRegs.toggleScreen();
						break;
					case CMD_VOL_UP:
						mpd_sendSetvolCommand(threadParms.mpd, volume+5);
						mpd_finishCommand(threadParms.mpd);
						rtmpdStatusChanged += VOL_CHG;
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						volume = rtmpdStatus->volume;
						break;
					case CMD_VOL_DOWN:
						mpd_sendSetvolCommand(threadParms.mpd, volume-5);
						mpd_finishCommand(threadParms.mpd);
						rtmpdStatusChanged += VOL_CHG;
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						volume = rtmpdStatus->volume;
						break;
					case CMD_MODE_RANDOM:
						random = !random;
						mpd_sendRandomCommand(threadParms.mpd, random);
						mpd_finishCommand(threadParms.mpd);
						rtmpdStatusChanged += RND_CHG;
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						break;
					case CMD_MODE_REPEAT:
						repeat = !repeat;
						mpd_sendRepeatCommand(threadParms.mpd, repeat);
						mpd_finishCommand(threadParms.mpd);
						rtmpdStatusChanged += RPT_CHG;
						mpd_sendStatusCommand(threadParms.mpd);
						rtmpdStatus = mpd_getStatus(threadParms.mpd);
						break;
					case CMD_TOGGLE_MODE:
						++curMode;
						if(curMode == 4)
							curMode = 0;
						break;
					case CMD_SAVE_PL:
						popupVisible = playlist.showSaveDialog(popup);
						break;
					case CMD_POP_CANCEL:
						popupVisible = false;	
						break;
					case CMD_SHOW_MENU:
						popupVisible = showMainMenu(screen, popup);
						break;
					case CMD_LAUNCH_APP:
						popupVisible = showLaunchMenu(screen, popup, config.getItem("programRoot"));
						break;
					case CMD_SHOW_OPTIONS:
						popupVisible = showOptionsMenu(screen, popup, config);
						break;
					case CMD_MPD_UPDATE:
						cout << "should be updateing" << endl;
						mpd_sendUpdateCommand(threadParms.mpd, "");
						mpd_finishCommand(threadParms.mpd);
						break;	

				}

				browser.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
				playlist.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus, 
								rtmpdStatusChanged, rtmpdStatus, repeatDelay);
				//plBrowser.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
				playing.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
				albumArt.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus,
						rtmpdStatusChanged, rtmpdStatus);
				statsBar.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus,
						rtmpdStatusChanged, rtmpdStatus, forceRefresh);
				bookmarks.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);

				// DRAWING STARTS HERE

//				SDL_BlitSurface(bmp, NULL,screen, &dstrect );
				playing.draw();
				statsBar.draw(forceRefresh);

				albumArt.draw(forceRefresh);
//				if(forceRefresh)
					SDL_FillRect(screen, &clearRect, SDL_MapRGB(screen->format, 255, 255, 255));
				switch (curMode) {
					case 0:
						browser.processCommand(command);
						if(command != 0 || forceRefresh) 
							browser.draw(forceRefresh);
						break;
					case 1:
						playlist.processCommand(command, rtmpdStatusChanged, rtmpdStatus, repeatDelay);
						playlist.draw(forceRefresh);
						break;
					case 2:
						plBrowser.processCommand(command);
						if(command != 0 || forceRefresh) 
							plBrowser.draw(forceRefresh);
						break;
					case 3:
						bookmarks.processCommand(command);
						if(command != 0 || forceRefresh) 
							bookmarks.draw(forceRefresh);
						break;
					default: 
						playlist.processCommand(command, rtmpdStatusChanged, rtmpdStatus, repeatDelay);
						playlist.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus, 
								rtmpdStatusChanged, rtmpdStatus, repeatDelay);
						if(command != 0 || forceRefresh) 
							playlist.draw(forceRefresh);
						break;
				}
				helpBar.draw(curMode, forceRefresh);
				forceRefresh = false;

				if(popupVisible) {
					popup.draw();
				}

				threadParms.mpdStatusChanged = 0;
				rtmpdStatusChanged = 0;
				if(rtmpdStatus != NULL) {
					mpd_freeStatus(rtmpdStatus);
					rtmpdStatus = NULL;
				}
				command = 0;
				SDL_mutexV(threadParms.lockConnection);
				// DRAWING ENDS HERE

				// finally, update the screen :)
				if(!gp2xRegs.screenIsOff()) 
					SDL_Flip(screen);
				SDL_Delay(75);
				++frame;
				/*	
					++frame;
					if(frame == 200)
					{
					timer.stop();
					int elapsed = timer.elapsed()/1000000;
					int fps = frame/elapsed;
					cout << "fps " << fps << endl;
					frame = 0;
					timer.start();
					}
					*/
			} // end main loop

			//save current state
			ofstream ommcState(".ommcState", ios::out|ios::trunc);
			ommcState << "mode=" << curMode << endl;
			ommcState << "vol=" << volume << endl;
			SDL_mutexP(threadParms.lockConnection);
			playlist.saveState(ommcState);
			SDL_mutexV(threadParms.lockConnection);
			// all is well ;)
#ifdef GP2X
			if(killMpd) {
				SDL_Quit();
			cout << "killit" << endl;
				execlp("./mpd", "./mpd", "--kill", "mpd.conf", NULL);
			}
#endif
			printf("Exited cleanly\n");
			return 0;
			//    }
	} catch (std::exception& e) {
		cout << "Process Exception: " << e.what() << endl;
	}

}
}
