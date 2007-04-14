#ifndef __COMMANDS_H__ 
#define __COMMANDS_H__

	typedef enum {	CMD_PLAY_PAUSE=1, CMD_STOP, CMD_PREV, CMD_NEXT, CMD_FF, CMD_RW,
					CMD_UP, CMD_DOWN, CMD_VOL_UP, CMD_VOL_DOWN,  
					CMD_ADD_TO_PL, CMD_NEW_PL, CMD_APPEND_PL, CMD_LOAD_PL, CMD_DEL_FROM_PL, 
					CMD_IMMEDIATE_PLAY, CMD_SAVE_PL, CMD_POP_SELECT,
					CMD_TOGGLE_MODE, CMD_TOGGLE_VIEW, 
			   		CMD_MODE_RANDOM, CMD_MODE_REPEAT, CMD_QUIT } cmdTypes_t;

class CommandFactory
{
public:
					
	CommandFactory();
	int getCommand(bool keysHeld[], int curMode, int& timer);

protected:
	int m_timer;	
	bool m_next;
	bool m_prev;
};

#endif
