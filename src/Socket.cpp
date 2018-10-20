//
//  Socket.cpp
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

#include "Socket.hpp"

Socket::Socket(int sport, int non_blocking) {
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        throw std::runtime_error("Windows Sockets failed to start");
    }
#endif

    // Create the socket
    handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (handle <= 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Bind the socket to the port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(sport);

    if (bind(handle, (const struct sockaddr *) &address, sizeof(struct sockaddr_in)) != 0) {
        throw std::runtime_error("Failed to bind socket");
    }

    // Set the socket to non-blocking if neccessary
    if (non_blocking) {
#ifdef _WIN32
        if (ioctlsocket(handle, FIONBIO, (u_long*)&non_blocking) != 0) {
            throw std::runtime_error("Failed to set socket to non-blocking");
        }
#else
        if (fcntl(handle, F_SETFL, O_NONBLOCK, non_blocking) != 0) {
            throw std::runtime_error("Failed to set socket to non-blocking");
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

void Socket::update_dest(std::string dhost, unsigned int dport){
  if((set_dhost.compare(dhost)==0)&&(dport==set_dport)){
    //the structure is already valid
    return;
  }
  //setup the destination address structure
  dest.sin_family = AF_INET;
  dest.sin_port = htons(dport);

  if (dhost.empty()) {
      dest.sin_addr.s_addr = INADDR_ANY;
  }
  else {
      dest.sin_addr.s_addr = inet_addr(dhost.c_str());
      if (dest.sin_addr.s_addr == INADDR_NONE) {
          struct hostent *hostent = gethostbyname(dhost.c_str());
          if (hostent) {
              memcpy(&(dest.sin_addr.s_addr), hostent->h_addr, (size_t)hostent->h_length);
          }
          else {
              throw std::runtime_error("Invalid host name");
          }
      }
  }

  set_dhost = dhost;
  set_dport = dport;
}

void Socket::send(std::string dhost, unsigned int dport, const void* data, int size) {
#ifdef _WIN32
    typedef int SSize;
    typedef int Size;
#else
    typedef ssize_t SSize;
    typedef size_t Size;
#endif

    update_dest(dhost, dport);

    SSize sent_bytes = sendto(handle, (const char *) data, (Size)size, 0, (const struct sockaddr *) &dest, (Size)sizeof(struct sockaddr_in));
    if (sent_bytes != size) {
        throw std::runtime_error("Failed to send data");
    }
}
