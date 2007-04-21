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

#include <fcntl.h>
#include <sys/mman.h>


//#define GP2X 

#define SYS_CLK_FREQ 7372800 

using namespace std;

int sp = 2;
#ifdef GP2X
int memfd = open("/dev/mem",O_RDWR);
unsigned long *memregs32 = (unsigned long*)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0xc0000000);
unsigned short *memregs16 = (unsigned short*)memregs32;
#endif

void SetClock(unsigned int MHZ) {
#ifdef GP2X
cout << "setting clock to " << MHZ << endl;
	unsigned int v;
	unsigned int mdiv,pdiv=3,scale=0;
	MHZ*=1000000;
	mdiv=(MHZ*pdiv)/SYS_CLK_FREQ;

	mdiv=((mdiv-8)<<8) & 0xff00;
	pdiv=((pdiv-2)<<2) & 0xfc;
	scale&=3;
	v = mdiv | pdiv | scale;

	unsigned int l = memregs32[0x808>>2];// Get interupt flags
	memregs32[0x808>>2] = 0xFF8FFFE7;   //Turn off interrupts
	memregs16[0x910>>1]=v;              //Set frequentie
	while(memregs16[0x0902>>1] & 1);    //Wait for the frequentie to be ajused
	memregs32[0x808>>2] = l;            //Turn on interrupts

#endif
}

bool showMainMenu(SDL_Surface* screen, Popup& popup)
{
	bool show = false;	
	
	Scroller::listing_t items;
	int type = Popup::POPUP_MENU;
	items.push_back(make_pair("Return to player", 0));	
	items.push_back(make_pair("Save Playlist", 1));	
	items.push_back(make_pair("Launch Program", 2));	
	items.push_back(make_pair("Detach Client", 3));	
	items.push_back(make_pair("Exit", 4));	
//	items.push_back(make_pair("", type));	
			
	popup.setItemsText(items, type);
	SDL_Rect popRect;
	popRect.w = 180;
	popRect.h = popup.skipVal()*7+15;
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
			rCommand = CMD_LAUNCH_PROG;
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
	}
	
	return rCommand;
}

int main ( int argc, char** argv )
{
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
/*
		mpd_sendPauseCommand(threadParms.mpd, 1);
		mpd_finishCommand(threadParms.mpd);
		SetClock(100);
		mpd_sendPauseCommand(threadParms.mpd, 0);
		mpd_finishCommand(threadParms.mpd);
*/		
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
					done = 1;
					break;
				case CMD_STOP:
					mpd_sendStopCommand(threadParms.mpd);
					mpd_finishCommand(threadParms.mpd);
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
					if(curMode == 3)
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
			}

			browser.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
			//plBrowser.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);
			playing.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus);

			playing.draw();
			statsBar.draw(threadParms.mpdStatusChanged, threadParms.mpdStatus,
					rtmpdStatusChanged, rtmpdStatus);
			// DRAWING STARTS HERE
			if(forceRefresh)
				SDL_FillRect(screen, &clearRect, SDL_MapRGB(screen->format, 0, 0, 110));

			switch (curMode) {
				case 0:
					browser.processCommand(command);
					if(command != 0 || forceRefresh) 
						browser.draw(forceRefresh);
					break;
				case 1:
					playlist.processCommand(command, rtmpdStatusChanged, rtmpdStatus, repeatDelay);
					playlist.updateStatus(threadParms.mpdStatusChanged, threadParms.mpdStatus, 
							rtmpdStatusChanged, rtmpdStatus, repeatDelay);
				if(command != 0 || forceRefresh) 
						playlist.draw(forceRefresh);
					break;
				case 2:
					plBrowser.processCommand(command);
					if(command != 0 || forceRefresh) 
						plBrowser.draw(forceRefresh);
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


		// all is well ;)
		printf("Exited cleanly\n");
#ifdef GP2X
		// close memory
		close(memfd);
		chdir("/usr/gp2x");
		execl("/usr/gp2x/gp2xmenu", "usr/gp2x/gp2xmenu", NULL);
#endif
		return 0;
		//    }
	} catch (std::exception& e) {
		cout << "Process Exception: " << e.what() << endl;
	}

}
