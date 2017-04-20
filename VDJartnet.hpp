//
//  VDJartnet.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 09/02/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

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
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include <iostream>

#if (defined(VDJ_WIN))
//#include <windows.h>
//#include <shlobj.h>
#elif (defined(VDJ_MAC))
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#endif

#define commandLength 512

class CVDJartnet : public IVdjPlugin8
{
public:
    HRESULT VDJ_API OnLoad();
    HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
    ULONG VDJ_API Release();
    HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8 *pluginInterface);
    HRESULT VDJ_API OnParameter(int id);
    HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize);

    void updateDMXvalues();

    typedef enum _ID_Interface
    {
    ID_ENABLE_BUTTON,
    ID_REFRESH_BUTTON,
    ID_CONFIG_BUTTON
    } ID_Interface;

private:
    const int noLength = 3;
    const int noChannels = 512;

    typedef struct _ArtNetPacket {
        const uint8_t header[8] = "Art-Net";
        const uint8_t opcodeLo = 0x00;
        const uint8_t opcodeHi = 0x50;
        const uint8_t versionHi = 00;
        const uint8_t versionLo = 14;
        uint8_t sequence = 1;
        uint8_t physical = 0;
        uint8_t universeLo = 0;
        uint8_t universeHi = 0;
        uint8_t lengthHi = 0x02;
        uint8_t lengthLo = 0x00;
        uint8_t data[512];
    } ArtNetPacket;

    int m_Enable;
    int m_Refresh;
    int m_Config;
    char *channelCommands[512];
    ArtNetPacket packet;
    int skippedPackets = 0;

    char host[commandLength] = "127.0.0.1";
    const unsigned short port = 0x1936;
    zed_net_address_t address;
    zed_net_socket_t socket;

    void sendArtnetPacket();

    std::thread *pollThread;
    std::thread *setupThread;
};

#ifdef GLOBALIMPLEMENTATION
CVDJartnet *globalCVDJartnet;
void globalUpdate() {
    for (;;) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        globalCVDJartnet->updateDMXvalues();
        //std::this_thread::sleep_for(10ms);
        std::this_thread::sleep_until(start + 10ms);
    }
}

void globalSetup() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(start + 1s);
    globalCVDJartnet->OnParameter(globalCVDJartnet->ID_REFRESH_BUTTON);
}


#endif

#endif /* VDJartnet_hpp */
