#ifndef __THREADPARMS_H__
#define __THREADPARMS_H__

typedef enum { SONG_CHG=1, PL_CHG=2, RPT_CHG=4, RND_CHG=8, RATE_CHG=16, ELAPSED_CHG=32, VOL_CHG=64, STATE_CHG=128 };

typedef struct {
	mpd_Connection* mpd;
	mpd_Status * mpdStatus;
	int mpdStatusChanged;
	bool pollStatusDone;
	SDL_mutex* lockConnection;
} threadParms_t;

#endif
