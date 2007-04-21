#ifndef __THREADPARMS_H__
#define __THREADPARMS_H__

#include "libmpdclient.h"
#include <SDL.h>

#ifndef GP2X
#define GP2X_VK_FY  SDLK_d
#define GP2X_VK_FX  SDLK_s
#define GP2X_VK_FB  SDLK_p
#define GP2X_VK_FA  SDLK_h
#define GP2X_VK_FR  SDLK_n
#define GP2X_VK_FL  SDLK_b
#define GP2X_VK_UP  SDLK_k
#define GP2X_VK_DOWN  SDLK_j
#define GP2X_VK_VOL_UP  SDLK_0
#define GP2X_VK_VOL_DOWN  SDLK_9
#define GP2X_VK_SELECT  SDLK_m
#define GP2X_VK_START  SDLK_c
#endif


typedef enum { SONG_CHG=1, PL_CHG=2, RPT_CHG=4, RND_CHG=8, RATE_CHG=16, ELAPSED_CHG=32, VOL_CHG=64, STATE_CHG=128 };

typedef struct {
	mpd_Connection* mpd;
	mpd_Status * mpdStatus;
	int mpdStatusChanged;
	bool pollStatusDone;
	SDL_mutex* lockConnection;
} threadParms_t;

#endif
