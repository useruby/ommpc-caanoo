#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "browser.h"
#include "plBrowser.h"
#include "playlist.h"
//#include "albumArt.h"
#include "nowPlaying.h"
#include "libmpdclient.h"
#include "threadFunctions.h"
#include "commandFactory.h"
#include "statsBar.h"
#include "helpBar.h"
#include "timer.h"
#include "popup.h"

using namespace std;

int main ( int argc, char** argv )
{
	try {
		Config config;

		// initialize SDL video
		if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			printf( "Unable to init SDL: %s\n", SDL_GetError() );
			return 1;
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
		SDL_Surface* screen = SDL_SetVideoMode(320, 240, 16,
				SDL_HWSURFACE|SDL_DOUBLEBUF);
		if ( !screen )
		{
			printf("Unable to set 320x240 video: %s\n", SDL_GetError());
			return 1;
		}

		// load an image
		/*SDL_Surface* bmp = SDL_LoadBMP("cb.bmp");
		if (!bmp)
		{
			printf("Unable to load bitmap: %s\n", SDL_GetError());
			return 1;
		}

		// centre the bitmap on screen
		SDL_Rect dstrect;
		dstrect.x = (screen->w - bmp->w) / 2;
		dstrect.y = (screen->h - bmp->h) / 2;

		*/
		
    	SDL_Rect mainRect = { config.getItemAsNum("main_x"),
							  config.getItemAsNum("main_y"),
							  config.getItemAsNum("main_width"),
							  config.getItemAsNum("main_height")
							};
    	SDL_Rect artRect = { config.getItemAsNum("art_x"),
							  config.getItemAsNum("art_y"),
							  config.getItemAsNum("art_width"),
							  config.getItemAsNum("art_height")
							};
    	SDL_Rect nowPlayingRect = { config.getItemAsNum("nowPlaying_x"),
							  config.getItemAsNum("nowPlaying_y"),
							  config.getItemAsNum("nowPlaying_width"),
							  config.getItemAsNum("nowPlaying_height")
							};
    	SDL_Rect statsRect = { config.getItemAsNum("stats_x"),
							  config.getItemAsNum("stats_y"),
							  config.getItemAsNum("stats_width"),
							  config.getItemAsNum("stats_height")
							 };
    	SDL_Rect helpRect = { config.getItemAsNum("help_x"),
							  config.getItemAsNum("help_y"),
							  config.getItemAsNum("help_width"),
							  config.getItemAsNum("help_height")
							 };
    	SDL_Rect menuRect = { config.getItemAsNum("menu_x"),
							  config.getItemAsNum("menu_y"),
							  config.getItemAsNum("menu_width"),
							  config.getItemAsNum("menu_height")
							 };
		SDL_Rect clearRect = { 0,0,screen->w, screen->h};
	
		SDL_Rect popRect;
		popRect.w = 150;
		popRect.h = 100;
		popRect.x = (screen->w - 150) / 2;
		popRect.y = (screen->h - 100) / 2;

	

		/*

		   int pid;
		   int wpid;
		   int status;

		   pid = fork();

		   if(pid ==-1) {
		   cout << "Fork failed" << endl;
		   } else if(pid == 0) { //child


		   string file = "/home/tim/c/tivo/jordin.mp3";
		   execlp("/usr/bin/mplayer", "/usr/bin/mplayer", "-slave", "-quiet", file.c_str(), NULL);
		   cout << errno << " " << strerror(errno) << " " << file << endl;
		   } else {

*/
		threadParms_t threadParms;
		threadParms.mpd = mpd_newConnection(config.getItem("host").c_str(), 
											config.getItemAsNum("port"),
											config.getItemAsNum("timeout"));
		threadParms.mpdStatus;
		threadParms.mpdStatusChanged = 0;
		threadParms.pollStatusDone = false;
		threadParms.lockConnection = SDL_CreateMutex();

		TTF_Font* font = TTF_OpenFont( "Vera.ttf", 10 );
		int skipVal = TTF_FontLineSkip( font );
		int numPerScreen = (mainRect.h-(2*skipVal))/skipVal;
		
		Popup popup(threadParms.mpd, screen, config, popRect, skipVal, numPerScreen);
		Browser browser(threadParms.mpd, screen, font, mainRect, skipVal, numPerScreen);
		Playlist playlist(threadParms.mpd, screen, font, config, mainRect, skipVal, numPerScreen);
		PLBrowser plBrowser(threadParms.mpd, screen, font, mainRect, skipVal, numPerScreen, playlist);
		NowPlaying playing(threadParms.mpd, threadParms.lockConnection, screen, config, nowPlayingRect, playlist);
		StatsBar statsBar(threadParms.mpd, threadParms.lockConnection, screen, config, statsRect);
		HelpBar helpBar(threadParms.mpd, screen, config, helpRect);
//		HelpBar helpBar(threadParms.mpd, threadParms.lockConnection, screen, config, nowPlayingRect, playlist);
		CommandFactory commandFactory;

		SDL_Thread *statusThread;
		statusThread = SDL_CreateThread(pollMpdStatus, &threadParms);
		if(statusThread == NULL) {
			cout << "unable to create status thread" << endl;
			return -1;
		}
 
		//enable status checking for interactive commands...
		mpd_Status* rtmpdStatus = NULL;
		int rtmpdStatusChanged = 0;
	
		bool done = false;
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
				if(event.type == SDL_JOYBUTTONDOWN)
					keysHeld[event.jbutton.button] = true;
				if(event.type == SDL_JOYBUTTONUP) {
					keysHeld[event.jbutton.button] = false;
					repeatDelay = 0;
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
				if (event.type == SDL_KEYDOWN) 
					++repeatDelay;
				command = commandFactory.getCommand(keysHeld, curMode, repeatDelay);
				if (event.type == SDL_KEYUP) 
					repeatDelay = 0;
				processedEvent = false;
			}
			
			if(popupVisible) {
				command = popup.processCommand(command);
				if(command != 0)
					popupVisible = false;	
			}
			
			if(command == CMD_QUIT) {
				done = 1;
			} else if(command == CMD_STOP) {
				mpd_sendStopCommand(threadParms.mpd);
				mpd_finishCommand(threadParms.mpd);
			} else if(command == CMD_VOL_UP) {
				mpd_sendSetvolCommand(threadParms.mpd, volume+5);
				mpd_finishCommand(threadParms.mpd);
				rtmpdStatusChanged += VOL_CHG;
				mpd_sendStatusCommand(threadParms.mpd);
				rtmpdStatus = mpd_getStatus(threadParms.mpd);
				volume = rtmpdStatus->volume;
			} else if(command == CMD_VOL_DOWN) {
				mpd_sendSetvolCommand(threadParms.mpd, volume-5);
				mpd_finishCommand(threadParms.mpd);
				rtmpdStatusChanged += VOL_CHG;
				mpd_sendStatusCommand(threadParms.mpd);
				rtmpdStatus = mpd_getStatus(threadParms.mpd);
				volume = rtmpdStatus->volume;
			} else if(command == CMD_MODE_RANDOM) {
				random = !random;
				mpd_sendRandomCommand(threadParms.mpd, random);
				mpd_finishCommand(threadParms.mpd);
				rtmpdStatusChanged += RND_CHG;
				mpd_sendStatusCommand(threadParms.mpd);
				rtmpdStatus = mpd_getStatus(threadParms.mpd);
			} else if(command == CMD_MODE_REPEAT) {
				repeat = !repeat;
				mpd_sendRepeatCommand(threadParms.mpd, repeat);
				mpd_finishCommand(threadParms.mpd);
				rtmpdStatusChanged += RPT_CHG;
				mpd_sendStatusCommand(threadParms.mpd);
				rtmpdStatus = mpd_getStatus(threadParms.mpd);
			} else if(command == CMD_TOGGLE_MODE) {
				++curMode;
				if(curMode == 3)
					curMode = 0;
			} else if(command == CMD_SAVE_PL) {
				popupVisible = playlist.showSaveDialog(popup);
			} else if(command == CMD_POP_SELECT) {
				int action = popup.selectedAction();
				std::string selText = popup.selectedText();
				if(action == Popup::POPUP_SAVE_PL) {
					if(selText != "Cancel") {
						mpd_sendSaveCommand(threadParms.mpd, selText.c_str());
						mpd_finishCommand(threadParms.mpd);
						plBrowser.updateListing();
					}
				}
				forceRefresh = true;
			}

			browser.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
			//plBrowser.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
			playlist.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus, repeatDelay);
			playing.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
			
			playing.draw();
			statsBar.draw(threadParms.mpdStatusChanged, threadParms.mpdStatus,
							rtmpdStatusChanged, rtmpdStatus);
			// DRAWING STARTS HERE
			if(forceRefresh)
				SDL_FillRect(screen, &clearRect, SDL_MapRGB(screen->format, 0, 0, 110));
//			if(command == -1)
//				SDL_BlitSurface(bmp, 0, screen, &artRect);
//			if(command != 0 || firstPass) {
				switch (curMode) {
					case 0:
						browser.processCommand(command);
						browser.draw(forceRefresh);
					break;
					case 1:
						playlist.processCommand(command, repeatDelay);
						playlist.draw(forceRefresh);
					break;
					case 2:
						plBrowser.processCommand(command);
						plBrowser.draw(forceRefresh);
					break;
					default: 
						playlist.processCommand(command, repeatDelay);
						playlist.draw(forceRefresh);
				}
				helpBar.draw(curMode, forceRefresh);
				forceRefresh = false;
//			}

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
			SDL_Flip(screen);
			SDL_Delay(40);
	/*
			++frame;
			if(frame == 100)
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

		// free loaded bitmap
//		SDL_FreeSurface(bmp);

		// all is well ;)
		printf("Exited cleanly\n");
		return 0;
		//    }
	} catch (std::exception& e) {
		cout << "Process Exception: " << e.what() << endl;
	}

}
