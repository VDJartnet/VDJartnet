//
//  Socket.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 09/02/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
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

#ifndef Socket_hpp
#define Socket_hpp

#include <stdlib.h>
#include <string>
#include <time.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif

class Socket {
public:
    Socket(unsigned int port, int non_blocking);
    ~Socket();

    void send(std::string hostS, unsigned short port, const void* data, int size);

private:
    int handle;
    int non_blocking;
};

#endif /* Socket_hpp */
