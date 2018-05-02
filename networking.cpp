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

#include "networking.h"

Address::Address(const char *host, unsigned short port) {
    if (host == NULL) {
        this->host = INADDR_ANY;
    } else {
        this->host = inet_addr(host);
        if (this->host == INADDR_NONE) {
            struct hostent *hostent = gethostbyname(host);
            if (hostent) {
                memcpy(&this->host, hostent->h_addr, (size_t) hostent->h_length);
            } else {
                throw "Invalid host name";
            }
        }
    }
    
    this->port = port;
}

const char* Address::to_str() {
    struct in_addr in;
    in.s_addr = host;
    
    return inet_ntoa(in);
}

Socket::Socket(unsigned int port, bool non_blocking) {
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        throw "Windows Sockets failed to start";
    }
#endif
    
    // Create the socket
    handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (handle <= 0) {
        //zed_net_socket_close(sock);
        throw "Failed to create socket";
    }
    
    // Bind the socket to the port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(handle, (const struct sockaddr *) &address, sizeof(struct sockaddr_in)) != 0) {
        //zed_net_socket_close(sock);
        throw "Failed to bind socket";
    }
    
    // Set the socket to non-blocking if neccessary
    if (non_blocking) {
#ifdef _WIN32
        if (ioctlsocket(handle, FIONBIO, (u_long*)&non_blocking) != 0) {
            //zed_net_socket_close(sock);
            throw "Failed to set socket to non-blocking";
        }
#else
        if (fcntl(handle, F_SETFL, O_NONBLOCK, non_blocking) != 0) {
            //zed_net_socket_close(sock);
            throw "Failed to set socket to non-blocking";
        }
#endif
    }
    
    this->non_blocking = non_blocking;
}

Socket::~Socket() {
    if (handle) {
#ifdef _WIN32
        closesocket(handle);
#else
        close(handle);
#endif
    }
    
#ifdef _WIN32
    WSACleanup();
#endif
}

void Socket::send(Address* destination, const void* data, size_t size) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = destination->host;
    address.sin_port = htons(destination->port);
    
    ssize_t sent_bytes = sendto(handle, (const char *) data, size, 0, (const struct sockaddr *) &address, sizeof(struct sockaddr_in));
    if (sent_bytes != (ssize_t) size) {
        throw "Failed to send data";
    }
}

ssize_t Socket::receive(Address* sender, void* data, size_t size) {
#ifdef _WIN32
    typedef int socklen_t;
#endif
    
    struct sockaddr_in from;
    socklen_t from_length = sizeof(from);
    
    ssize_t received_bytes = recvfrom(handle, (char *) data, size, 0, (struct sockaddr *) &from, &from_length);
    if (received_bytes <= 0) {
        return 0;
    }
    
    sender->host = from.sin_addr.s_addr;
    sender->port = ntohs(from.sin_port);
    
    return received_bytes;
}

// vim: tabstop=4 shiftwidth=4 expandtab
