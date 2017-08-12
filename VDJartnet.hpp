//
//  VDJartnet.hpp
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
//{Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of VirtualDJ used as well as that of the covered work.}
//
//If you modify this Program, or any covered work, by linking or
//combining it with the Visual C++ Runtime, the licensors of this Program grant you
//additional permission to convey the resulting work.
//{Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of the Visual C++ Runtime used as well as that of the covered work.}

#ifndef VDJartnet_hpp
#define VDJartnet_hpp

// we include stdio.h only to use the sprintf() function
// we define _CRT_SECURE_NO_WARNINGS for the warnings of the sprintf() function
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "vdjPlugin8.h"

#define noTCP
#include "zed_net-master/zed_net.h"

#include <string>
#include <fstream>
#include <chrono>
//using namespace std::chrono_literals;

#include <iostream>

#if (defined(VDJ_WIN))
//#include <windows.h>
//#include <shlobj.h>
#elif (defined(VDJ_MAC))
#include <CoreFoundation/CoreFoundation.h>
//#include <CoreServices/CoreServices.h>
//#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#endif

#define commandLength 512

class CVDJartnet : public IVdjPlugin8 {
public:
    int m_Enable;
    int m_Refresh;
    int m_Config;

    HRESULT VDJ_API OnLoad();
    HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
    ULONG VDJ_API Release();
    HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8 *pluginInterface);
    HRESULT VDJ_API OnParameter(int id);
    HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize);

    std::chrono::milliseconds checkRate = std::chrono::milliseconds(10);

    void updateDMXvalues();

    typedef enum _ID_Interface
    {
    ID_ENABLE_BUTTON,
    ID_REFRESH_BUTTON,
    ID_CONFIG_BUTTON,
    ID_SAVE
    } ID_Interface;

    std::string host = "127.0.0.1";
    const unsigned short port = 0x1936;
    
    std::string channelCommands[512];

    std::ifstream* presetFin;

#ifdef VDJ_MAC
    void* configWindow;
#endif
#ifdef VDJ_WIN
    void* configWindow;
#endif

private:
    const int noLength = 3;
    const int noChannels = 512;

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
    int skippedPackets = 0;
    int skipPacketLimit = 10;


    zed_net_address_t address;
    zed_net_socket_t* socket = (zed_net_socket_t*)malloc(sizeof(zed_net_socket_t));

    void sendArtnetPacket();
    void parseConfigLine(std::string line);
    void parseCommandConfigLine(std::string line);
    void loadConfigNoHost(std::string path);

    void* pollThread; //std::thread
    void* setupThread; //std::thread
};

#ifdef VDJartnet_GLOBALIMPLEMENTATION
CVDJartnet* globalCVDJartnet = new CVDJartnet();

void globalUpdate() {
    for (;;) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        globalCVDJartnet->updateDMXvalues();
        //std::this_thread::sleep_for(10ms);
        std::this_thread::sleep_until(start + globalCVDJartnet->checkRate);
    }
}

void globalSetup() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(start + std::chrono::seconds(1));
    globalCVDJartnet->OnParameter(globalCVDJartnet->ID_REFRESH_BUTTON);
}

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                // Also handle the case when the last line has no line ending
                if(t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}
#else
extern CVDJartnet *globalCVDJartnet;
extern void globalUpdate();
extern void globalSetup();
extern std::istream& safeGetline(std::istream& is, std::string& t);
#endif

#endif /* VDJartnet_hpp */
