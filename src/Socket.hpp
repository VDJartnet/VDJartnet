//
//  Socket.hpp
//  VDJartnet
//
//  Copyright © 2017-18 Jonathan Tanner. All rights reserved.
//
//This file is part of VDJartnet.
//
//VDJartnet is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//VDJartnet is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with VDJartnet.  If not, see <http://www.gnu.org/licenses/>.
//
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with VirtualDJ, the licensors of this Program grant you
//additional permission to convey the resulting work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with the Visual C++ Runtime, the licensors of this Program grant you
//additional permission to convey the resulting work.
//Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of the Visual C++ Runtime used as well as that of the covered work.

#ifndef Socket_hpp
#define Socket_hpp

#include <stdlib.h>
#include <string>
#include <time.h>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS //Prevent the older APIs from showing warnings
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif

/** A UDP socket */
class Socket {
public:
    Socket(int sport, int non_blocking); /**< Construct a socket with the given source port and value of non_blocking */
    ~Socket(); /**< Destruct the socket */
    void send(std::string dhost, unsigned int dport, const void* data, int size); /**< Send the given data to the given host and port */
private:
    int handle; /**< The handle to the socket */
    int non_blocking; /**< Whether the socket is blocking */
    struct sockaddr_in dest;
    std::string set_dhost;
    unsigned int set_dport;
    void update_dest(std::string dhost, unsigned int dport);
};

#endif /* Socket_hpp */
