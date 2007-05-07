#ifndef _MTI_TIMER_H_
#define _MTI_TIMER_H_

// Simple little class to time things to micro second precesion (on some
// machines).  Since the plain vanila ascii charater set lacks greek letters, I
// will refer to micros seconds as us or usec.

#include <iostream>
#include <string>

#ifdef __unix__
#include <sys/time.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

class Timer {
    public:
        Timer ()
        {
            start () ;
        }
 
        void start ()
        {
#ifdef __unix__
			if(!m_active) {
				gettimeofday (&m_start, 0) ;
				m_active = true;
			}
#else
            m_start = GetTickCount () ;
#endif
        }

        void stop ()
        {
#ifdef __unix__
            gettimeofday (&m_stop, 0) ;
			m_active = false;
#else
            m_stop = GetTickCount () ;
#endif
        }
 
        long elapsed () const
        {
#ifdef __unix__
            return static_cast <long> (m_stop.tv_usec) +
                (m_stop.tv_sec - m_start.tv_sec) * 1000000L
                - m_start.tv_usec ;
#else
            return static_cast <long> (m_stop - m_start) * 1000L ;
#endif
        }
	
		int elaspedSeconds() const
		{
			return (m_stop.tv_sec - m_start.tv_sec);
		}
	
		long check()
		{
            gettimeofday (&m_stop, 0) ;
			
            long delay = static_cast <long> (m_stop.tv_usec) +
                (m_stop.tv_sec - m_start.tv_sec) * 1000000L
                - m_start.tv_usec ;
			return delay;
		}

		bool active()
		{
			return m_active;
		}

    private:
#ifdef __unix__
        struct timeval  m_start ;
        struct timeval  m_stop ;
		bool m_active;
#else
        DWORD           m_start ;
        DWORD           m_stop ;
#endif
};

#endif
