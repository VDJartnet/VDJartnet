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
//
//If you modify this Program, or any covered work, by linking or
//combining it with the Visual C++ Runtime, the licensors of this Program grant you
//additional permission to convey the resulting work.

#ifndef VDJartnet_hpp
#define VDJartnet_hpp

#include "vdjPlugin8.h"

#include "networking.h"

#include <string>
#include <fstream>
#include <chrono>
#include <thread>

//#include <iostream>

#define commandLength 512

#if (defined(VDJ_WIN))
#define pluginPath "\\Plugins\\AutoStart\\"
#elif (defined(VDJ_MAC))
#define pluginPath "/Plugins64/AutoStart/"
#endif

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
    ID_SETUP,
    ID_SAVE
    } ID_Interface;

    std::string host = "127.0.0.1";
    const unsigned short port = 0x1936;

    std::string channelCommands[512];

    std::ifstream* presetFin;

    void* configWindow;

private:
    const int noLength = 3;
    const int noChannels = 512;

    typedef struct _ArtNetPacket {
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


    Address* address;

    void sendArtnetPacket();
    void parseConfigLine(std::string line);
    void parseCommandConfigLine(std::string line);
    void loadConfigNoHost(std::string path);

    void* pollThread; //std::thread
    void* setupThread; //std::thread
};

extern CVDJartnet *globalCVDJartnet;
extern Socket* globalCVDJartnetSocket;
extern bool globalCVDJartnetLoaded;
extern void globalUpdate();
extern void globalSetup();
extern std::istream& safeGetline(std::istream& is, std::string& t);
extern std::string toStringOfLength(size_t len, int value);
#endif /* VDJartnet_hpp */
