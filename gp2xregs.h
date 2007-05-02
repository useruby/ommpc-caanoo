#ifndef __GP2XREGS_H__
#define __GP2XREGS_H__

#include <fcntl.h>
#include <sys/mman.h>

#define OFF_GPIOH          (0x0837)
#define PWR_TFT_BIT        (2)
#define SYS_CLK_FREQ 7372800 

#define GP2X 

class GP2XRegs
{
public:
	GP2XRegs();
	~GP2XRegs();

	void setClock(unsigned int mhz);
	void toggleScreen();	
	inline bool screenIsOff() {return m_screenIsOff;}
protected:

	bool m_screenIsOff;
#ifdef GP2X
	int m_memfd;
	unsigned long* m_memregs32;
	unsigned short* m_memregs16;
#endif


};


#endif
