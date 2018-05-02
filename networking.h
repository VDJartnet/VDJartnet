/////////////////////////////////////////////////////////////////////////////////////////
//
// zed_net - v0.18 - public domain networking library
// (inspired by the excellent stb libraries: https://github.com/nothings/stb)
//
// This library is intended primarily for use in games and provides a simple wrapper
// around BSD sockets (Winsock 2.2 on Windows). Sockets can be set to be blocking or
// non-blocking.
//
// Only UDP sockets are supported at this time, but this may later expand to include TCP.
//
// VERSION HISTORY
//
//    0.19 (3/4/2016) TCP added and malloc/free calls removed.
//                     Not backwards compatible. - Ian T. Jacobsen (itjac.me)
//    0.18 (9/13/2015) minor polishing
//    0.17 (8/8/2015) initial release
//
// LICENSE
//
//    This software is in the public domain. Where that dedication is not recognized, you
//    are granted a perpetual, irrevocable license to copy, distribute, and modify this
//    file as you see fit.
//
// USAGE
//
//    #define the symbol ZED_NET_IMPLEMENTATION in *one* C/C++ file before the #include
//    of this file; the implementation will be generated in that file.
//
//    If you define the symbol ZED_NET_STATIC, then the implementation will be private to
//    that file.
//
//    Immediately after this block comment is the "header file" section. This section
//    includes documentation for each API function.
//

#ifndef INCLUDE_ZED_NET_H
#define INCLUDE_ZED_NET_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "wsock32.lib")
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif

struct Address {
public:
    unsigned int host;
    unsigned short port;
    
    // 'host' may contain a decimal formatted IP (such as "127.0.0.1"), a human readable
    // name (such as "localhost"), or NULL for the default address
    Address(const char *host, unsigned short port);
    
    // Converts an address's host name into a decimal formatted string
    //
    // Returns NULL on failure (call 'zed_net_get_error' for more info)
    const char* to_str();
};

class Socket {
    int handle;
    bool non_blocking;
    int ready;
    
public:
    // Opens a UDP socket and binds it to a specified port
    // (use 0 to select a random open port)
    //
    // Socket will not block if 'non-blocking' is true
    Socket(unsigned int port, bool non_blocking);
    ~Socket();

    // Sends a specific amount of data to 'destination'
    void send(Address* destination, const void* data, size_t size);

    // Receives a specific amount of data from 'sender'
    //
    // Returns the number of bytes received, -1 otherwise (call 'zed_net_get_error' for more info)
    ssize_t receive(Address* sender, void* data, size_t size);
};

#endif // INCLUDE_ZED_NET_H

// vim: tabstop=4 shiftwidth=4 expandtab
