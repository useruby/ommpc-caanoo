#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <time.h>
#include <sys/time.h>
#include <string>

class TimeStamp
{
public:
        TimeStamp();

        std::string currentTimeAsString(int format, bool getu=false);
        std::string timeAsString1(struct timeval tnow, bool getu=false);
        std::string timeAsString2(struct timeval tnow, bool getu=false);
        std::string timeAsString3(struct timeval tnow, bool getu=false);

};



#endif

