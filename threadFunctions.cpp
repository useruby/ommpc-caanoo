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
	int prevSongId = -1;
	int curSongId = -1;
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
	int curUpDb = -1;
	int prevUpDb = -1;

	// wait for MPD to become available
	while(!threadParms->mpdReady) {
		SDL_Delay(200);
	}

	// begin polling
	while(!threadParms->pollStatusDone) {
		SDL_mutexP(threadParms->lockConnection);
//	std::cout << "polling status" << std::endl;
		if (threadParms->mpdStatus != NULL) {
			mpd_freeStatus(threadParms->mpdStatus);
			threadParms->mpdStatus = NULL;
		}
		mpd_sendStatusCommand(threadParms->mpd);
		threadParms->mpdStatus = mpd_getStatus(threadParms->mpd);
		mpd_finishCommand(threadParms->mpd);
		if (threadParms->mpdStatus != NULL) {
			curSong = threadParms->mpdStatus->song;
			curSongId = threadParms->mpdStatus->songid;
			curPlaylist = threadParms->mpdStatus->playlist;
			curRpt = threadParms->mpdStatus->repeat;
			curRnd = threadParms->mpdStatus->random;
			curBitRate = threadParms->mpdStatus->bitRate;
			curElapsed = threadParms->mpdStatus->elapsedTime;
			curTotal = threadParms->mpdStatus->totalTime;
			curVol = threadParms->mpdStatus->volume;
			curState = threadParms->mpdStatus->state;
			curUpDb = threadParms->mpdStatus->updatingDb;
		}

		if(prevSongId != curSongId) {
			threadParms->mpdStatusChanged += SONG_CHG;
			prevSong = curSong;
			prevSongId = curSongId;
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
		if(curUpDb != prevUpDb) {
			threadParms->mpdStatusChanged += UPDB_CHG;
			prevUpDb = curUpDb;
		}
		SDL_mutexV(threadParms->lockConnection);
		do {
			SDL_Delay(300);
		} while((threadParms->mpdStatusChanged != 0)
				&& !threadParms->pollStatusDone);
	}

	SDL_mutexP(threadParms->lockConnection);
	if (threadParms->mpdStatus != NULL) {
		mpd_freeStatus(threadParms->mpdStatus);
		threadParms->mpdStatus = NULL;
	}
	SDL_mutexV(threadParms->lockConnection);

	printf("End status polling thread\n");
	return(0);
}

int loadAlbumArt(void* data) 
{
	artThreadParms_t* artParms = (artThreadParms_t*) data;
	//while(config.getItem("showAlbumArt") == "true") {
	while(!artParms->done) {
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
		//	if(tmp->w > artParms->destWidth)
				zoomx = (double)artParms->destWidth/(double)tmp->w;
		//	if(tmp->h > artParms->destHeight)
				zoomy = (double)artParms->destHeight/(double)tmp->h;
			
			artParms->artSurface = SDL_DisplayFormat(zoomSurface(tmp, zoomx,zoomy,1));
			SDL_FreeSurface(tmp);
			artParms->doArtLoad = false;
		}
		SDL_Delay(500);
	}

	printf("End art-loading thread\n");
	return(0);
}

int updateSongDb(void* data) 
{
	songDbThreadParms_t* songDbParms = (songDbThreadParms_t*) data;
	songDbParms->updating = true;
	//while(!artParms->done) {
		songDbParms->songDb->update();
	//	SDL_Delay(1000);
	//}
	songDbParms->updating = false;
	return 0;
}
