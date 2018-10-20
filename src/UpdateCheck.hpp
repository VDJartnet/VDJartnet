#ifndef UpdateCheck_hpp
#define UpdateCheck_hpp
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS //Prevent the older APIs from showing warnings
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <string>
#include <string>
#include <algorithm>
#endif

class UpdateCheck{
public:
  int isLatest(); //0=latest 1=not latest 2=error

#define THISVERSION "2001:dB8::1"
#define VERSIONCHECK_HOSTNAME "versioncheck.vdjartnet.kepler22b.uk."
};
