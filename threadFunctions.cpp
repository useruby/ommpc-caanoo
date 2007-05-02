#include "threadFunctions.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include "threadParms.h"

#include "unistd.h"
#include <dirent.h>

using namespace std;

int pollMpdStatus(void *data)
{
	threadParms_t* threadParms = (threadParms_t*) data;
	int prevSong = -1;
	int curSong = -1;
	long long  prevPlaylist = -1;
	long long  curPlaylist = -1;
	int prevRpt = -1;
	int curRpt = -1;
	int prevRnd = -1;
	int curRnd = -1;
	int curBitRate = -1;
	int prevBitRate = -1;
	int curElapsed = -1;
	int prevElapsed = -1;
	int curTotal = -1;
	int curVol = -1;
	int prevVol = -1;
	int curState = -1;
	int prevState = -1;

	while(!threadParms->pollStatusDone) {
		SDL_mutexP(threadParms->lockConnection);
//	std::cout << "polling status" << std::endl;
		mpd_sendStatusCommand(threadParms->mpd);
		threadParms->mpdStatus = mpd_getStatus(threadParms->mpd);
		mpd_finishCommand(threadParms->mpd);
		curSong = threadParms->mpdStatus->song;
		curPlaylist = threadParms->mpdStatus->playlist;
		curRpt = threadParms->mpdStatus->repeat;
		curRnd = threadParms->mpdStatus->random;
		curBitRate = threadParms->mpdStatus->bitRate;
		curElapsed = threadParms->mpdStatus->elapsedTime;
		curTotal = threadParms->mpdStatus->totalTime;
		curVol = threadParms->mpdStatus->volume;		
		curState = threadParms->mpdStatus->state;	

		if(prevSong != curSong) {
			threadParms->mpdStatusChanged += SONG_CHG;
			prevSong = curSong;
		}
		if(prevPlaylist != curPlaylist) {
			threadParms->mpdStatusChanged += PL_CHG;
			prevPlaylist = curPlaylist;
		}
		if(prevRpt != curRpt) {
			threadParms->mpdStatusChanged += RPT_CHG;
			prevRpt = curRpt;
		}
		if(prevRnd != curRnd) {
			threadParms->mpdStatusChanged += RND_CHG;
			prevRnd = curRnd;
		}
		if(prevBitRate != curBitRate) {
			threadParms->mpdStatusChanged += RATE_CHG;
			prevBitRate = curBitRate;
		}	
		if(prevElapsed != curElapsed) {
			threadParms->mpdStatusChanged += ELAPSED_CHG;
			prevElapsed = curElapsed;
		}	
		if(prevVol != curVol) {
			threadParms->mpdStatusChanged += VOL_CHG;
			prevVol = curVol;
		}
		if(prevState != curState) {
			threadParms->mpdStatusChanged += STATE_CHG;
			prevState = curState;
		}	
		SDL_mutexV(threadParms->lockConnection);
		while(threadParms->mpdStatusChanged != 0) {
			SDL_Delay(300);
		}
		mpd_freeStatus(threadParms->mpdStatus);
		SDL_Delay(700);
	
	}

    printf("End status polling thread\n");
    return(0);
}

int loadAlbumArt(void* data) 
{
	artThreadParms_t* artParms = (artThreadParms_t*) data;
	while(true) {
		if(artParms->doArtLoad) {
			SDL_FreeSurface(artParms->artSurface);
			SDL_Surface* tmp;
			string file = artParms->songFile;
			string dir;
			int pos = file.rfind(".");;
			if(pos != string::npos) {
				file = file.substr(0, pos+1) + "jpg";
			}
			pos = file.rfind("/");;
			if(pos != string::npos) {
				 dir = file.substr(0, pos+1);
			}
				
			//AlbumArt.jpg
			//AlbumArtSmall.jpg
			//Folder.jpg
		
			bool goodArt = false;	
			if(!goodArt) {
				tmp  = IMG_Load((dir + "AlbumArt.jpg").c_str());	
				if(!tmp)
					cout << "Can't find " << dir << "AlbumArt.jpg" << endl;
				else 
					goodArt = true;
			}
			if(!goodArt) {
				tmp  = IMG_Load((dir + "AlbumArtSmall.jpg").c_str());	
				if(!tmp)
					cout << "Can't find " << dir << "AlbumArtSmall.jpg" << endl;
				else 
					goodArt = true;
			}
			if(!goodArt) {
				tmp  = IMG_Load((dir + "Folder.jpg").c_str());	
				if(!tmp)
					cout << "Can't find " << dir << "Folder.jpg" << endl;
				else 
					goodArt = true;
			}
			if(!goodArt) {
				tmp  = IMG_Load(file.c_str());	
				if(!tmp)
					cout << "Can't find "  << file << endl;
				else 
					goodArt = true;
			}
			//last resort...just pull the first jpg in the folder...
			if(!goodArt && !file.empty()) {
				DIR * udir = opendir(dir.c_str());

				if(udir != NULL) {
					struct dirent * dirent = readdir(udir);
						
					bool done = false;
					while(dirent != NULL && !done) {
						string ename = dir + dirent->d_name;
						if(ename[0] != '.' && ename.substr(ename.size() - 3) == "jpg") {
							tmp = IMG_Load(ename.c_str());
							if(!tmp)
								cout << "Can't find any jpg in the directory" << endl;
							else  {
								cout << "loaded " << ename << endl;
								goodArt = true;
							}
		
							done = true;
						}
						dirent = readdir(udir);
					}

				}	
			}
			if(!goodArt) {
				cout << "no art found, loading default" << endl;
				tmp  = IMG_Load("default.jpg");	
			}

			double zoomx = 1;
			double zoomy = 1;	
			if(tmp->w > artParms->destWidth)
				zoomx = (double)artParms->destWidth/(double)tmp->w;
			if(tmp->h > artParms->destHeight)
				zoomy = (double)artParms->destHeight/(double)tmp->h;
			
			artParms->artSurface = zoomSurface(tmp, zoomx,zoomy,1);
			SDL_FreeSurface(tmp);
			artParms->doArtLoad = false;
		}

		SDL_Delay(200);
	}
}
