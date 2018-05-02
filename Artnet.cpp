//
//  Artnet.cpp
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

#include "Artnet.hpp"

bool Artnet::setChannel(int channel, uint8_t value) {
    if (channel >= 0 && channel < 512) {
        if (packet.data[channel] != value) {
            packet.data[channel] = value;
            return true;
        }
    }
    return false;
}

void Artnet::sendArtnetPacket(std::string host, unsigned short port) {
    getSocket()->send(host, port, &packet, sizeof(packet));
    if (packet.sequence == 0xFF) {
        packet.sequence = 1;
    } else {
        packet.sequence += 1;
    }
}
