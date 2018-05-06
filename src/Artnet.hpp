//
//  Artnet.hpp
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

#ifndef Artnet_hpp
#define Artnet_hpp

#include "Socket.hpp"

/** A class to send Art-Net data */
class Artnet {
public:
    bool setChannel(int channel, uint8_t value); /**< Set a given DMX channel to a given value */
    void sendArtnetPacket(std::string host, unsigned short port); /**< Send an Art-Net packet to the given address */

private:
    /** An Art-Net packet */
    struct ArtNetPacket {
        uint8_t header0 = 'A'; /**< The 'A' character of the Art-Net header */
        uint8_t header1 = 'r'; /**< The 'r' character of the Art-Net header */
        uint8_t header2 = 't'; /**< The 't' character of the Art-Net header */
        uint8_t header3 = '-'; /**< The '-' character of the Art-Net header */
        uint8_t header4 = 'N'; /**< The 'N' character of the Art-Net header */
        uint8_t header5 = 'e'; /**< The 'e' character of the Art-Net header */
        uint8_t header6 = 't'; /**< The 't' character of the Art-Net header */
        uint8_t header7 = 0; /**< The NULL terminator of the Art-Net header */
        uint8_t opcodeLo = 0x00; /**< The low byte of the 0x5000 Art-Net opcode */
        uint8_t opcodeHi = 0x50; /**< The high byte of the Art-Net opcode (0x5000) */
        uint8_t versionHi = 00; /**< The high byte of the Art-Net version (14) */
        uint8_t versionLo = 14; /**< The low byte of the Art-Net version (14) */
        uint8_t sequence = 1; /**< The Art-Net sequence field. Used to reorder out of order packets. 0 means unused, otherwise increase with sequential packets from 1 to 255 then loop to 1 again. */
        uint8_t physical = 0; /**< The Art-Net physical field. Shows the original physical universe of the data. Always set to 0. */
        uint8_t universeLo = 0; /**< The low byte of the Art-Net universe field */
        uint8_t universeHi = 0; /**< The high byte of the Art-Net universe field */
        uint8_t lengthHi = 0x02; /**< The high byte of the Art-Net length field. This field is always set to 255. */
        uint8_t lengthLo = 0x00; /**< The low byte of the Art-Net length field. This field is always set to 255. */
        uint8_t data[512]; /**< The DMX data to be sent */
    };

    ArtNetPacket packet; /**< The packet to be sent */

    /** Get the shared Socket */
    Socket* getSocket() {
        static Socket* sock = new Socket(0, 0);
        return sock;
    }
};

#endif /* Artnet_hpp */
