#ifndef UpdateCheck_hpp
#define UpdateCheck_hpp

#include <stdlib.h>
#include <string>
#include <algorithm>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS //Prevent the older APIs from showing warnings
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#ifndef CLRFREE
#include "CppStep/src/CSDialog.hpp"
#endif

namespace UpdateCheck{
    enum class Result { latest, notLatest, error };
    Result isLatest();
    void check();
};
#endif
