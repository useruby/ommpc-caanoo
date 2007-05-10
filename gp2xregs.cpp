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

#include "gp2xregs.h"
#include <iostream>

using namespace std;

GP2XRegs::GP2XRegs()
	: m_screenIsOff(false)
{
#ifdef GP2X
	m_memfd = open("/dev/mem",O_RDWR);
	m_memregs32 = (unsigned long*)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, m_memfd, 0xc0000000);
	m_memregs16 = (unsigned short*)m_memregs32;
#endif
}

GP2XRegs::~GP2XRegs()
{
#ifdef GP2X
	close(m_memfd);
#endif
}

void GP2XRegs::setClock(unsigned int MHZ) {
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

	unsigned int l = m_memregs32[0x808>>2];// Get interupt flags
	m_memregs32[0x808>>2] = 0xFF8FFFE7;   //Turn off interrupts
	m_memregs16[0x910>>1]=v;              //Set frequentie
	while(m_memregs16[0x0902>>1] & 1);    //Wait for the frequentie to be ajused
	m_memregs32[0x808>>2] = l;            //Turn on interrupts
#else
cout << "setting clock to " << MHZ << endl;
#endif
}

void GP2XRegs::toggleScreen()
{
#ifdef GP2X
	unsigned short tftState;
	if(m_screenIsOff) {
		tftState = m_memregs16[OFF_GPIOH] | (1<<PWR_TFT_BIT);
		m_memregs16[OFF_GPIOH] = tftState;
		m_screenIsOff = false;
	} else {
		tftState = m_memregs16[OFF_GPIOH] | (1<<PWR_TFT_BIT);
		tftState = tftState & (~(1<<PWR_TFT_BIT));

		m_memregs16[OFF_GPIOH] = tftState;
		m_screenIsOff = true;
	}
#else
	cout << "We would be toggleing the screen now" << endl;	
	m_screenIsOff = !m_screenIsOff;
#endif

}
