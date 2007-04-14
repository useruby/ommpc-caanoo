#ifndef __THREADFUNCS_H__
#define __THREADFUNCS_H__

#include <SDL.h>
#include "threadParms.h"

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

#endif
