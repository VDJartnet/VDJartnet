//
//  Artnet.hpp
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

#ifndef Artnet_hpp
#define Artnet_hpp

#include "Socket.hpp"

class Artnet {
public:
    bool setChannel(int channel, uint8_t value);
    void sendArtnetPacket(std::string host, unsigned short port);

private:
    typedef struct _ArtNetPacket {
        //uint8_t header[8] = "Art-Net";
        uint8_t header0 = 'A';
        uint8_t header1 = 'r';
        uint8_t header2 = 't';
        uint8_t header3 = '-';
        uint8_t header4 = 'N';
        uint8_t header5 = 'e';
        uint8_t header6 = 't';
        uint8_t header7 = 0;
        uint8_t opcodeLo = 0x00;
        uint8_t opcodeHi = 0x50;
        uint8_t versionHi = 00;
        uint8_t versionLo = 14;
        uint8_t sequence = 1;
        uint8_t physical = 0;
        uint8_t universeLo = 0;
        uint8_t universeHi = 0;
        uint8_t lengthHi = 0x02;
        uint8_t lengthLo = 0x00;
        uint8_t data[512];
    } ArtNetPacket;

    ArtNetPacket packet;

    Socket* getSocket() {
        static Socket* sock = new Socket(0, 0);
        return sock;
    }
};

#endif /* Artnet_hpp */
