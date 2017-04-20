//
//  VDJartnet.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 09/02/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#define ZED_NET_IMPLEMENTATION
#define GLOBALIMPLEMENTATION
#include "VDJartnet.hpp"

//-----------------------------------------------------------------------------
HRESULT VDJ_API CVDJartnet::OnLoad() {
    // ADD YOUR CODE HERE WHEN THE PLUGIN IS CALLED
    m_Enable = 0;
    m_Refresh = 0;

    zed_net_init();
    zed_net_get_address(&address, host, port);
    zed_net_udp_socket_open(&socket, 0, 0);

    globalCVDJartnet = this;

    DeclareParameterSwitch(&m_Enable,ID_ENABLE_BUTTON,"Enable","E", true);
    DeclareParameterButton(&m_Refresh,ID_REFRESH_BUTTON,"Refresh","R");
    DeclareParameterButton(&m_Config,ID_CONFIG_BUTTON,"Config","C");

    //OnParameter(ID_REFRESH_BUTTON);

    pollThread = new std::thread(globalUpdate);
    //setupThread = new std::thread(globalSetup);

    return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CVDJartnet::OnGetPluginInfo(TVdjPluginInfo8 *infos) {
    infos->PluginName = "VDJartnet";
    infos->Author = "Jonathan Tanner";
    infos->Description = "Send Artnet Commands from VirtualDJ";
    infos->Version = "1.0";
    infos->Flags = 0x00;
    infos->Bitmap = NULL;

    return S_OK;
}
//---------------------------------------------------------------------------
ULONG VDJ_API CVDJartnet::Release() {
    zed_net_socket_close(&socket);

    delete this;
    return 0;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CVDJartnet::OnGetUserInterface(TVdjPluginInterface8 *pluginInterface) {

    pluginInterface->Type = VDJINTERFACE_DEFAULT;

    return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CVDJartnet::OnParameter(int id) {
    //updateDMXvalues();

    switch(id) {
    case ID_ENABLE_BUTTON:
        if(m_Enable == 1) {
            //HRESULT hr;
            //hr = SendCommand("effect_slider 1 50%");
            }
        break;

    case ID_REFRESH_BUTTON:
    do {
        char path[256];
        GetStringInfo("get_vdj_folder", path, 256);

#if (defined(VDJ_WIN))

        //strcat(path, getenv("USERPROFILE"));
        //strcat(path, "\\artnet.cfg");
        //strcat(path, ".\\Documents\\VirtualDJ\\artnet.cfg");

        //wchar_t* pathW = nullptr;
        //SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, (PWSTR*)pathW);
        //wcstombs(path, pathW, 256);

        //strcat(path, "S:\\Documents\\VirtualDJ\\Plugins\\AutoStart\\VDJartnet\\config.txt");

        strcat(path, "\\Plugins\\AutoStart\\VDJartnet\\config.txt");

#elif (defined(VDJ_MAC))

        //strcat(path, getenv("HOME"));
        //strcat(path, "/Documents/VirtualDJ/Plugins64/AutoStart/VDJartnet/config.txt");

        strcat(path, "/Plugins64/AutoStart/VDJartnet/config.txt");

#endif


        FILE* file = fopen(path, "r");

        if (file != nullptr) {
            fgets(host, commandLength, file);
            zed_net_get_address(&address, host, port);

            char line[commandLength + 4];

            while (fgets(line, commandLength + 4, file)) {
                int channelNo = (int)strtol(strtok(line, "~"), nullptr, 0) - 1;
                if (channelNo < noChannels && channelNo >= 0) {
                    if (channelCommands[channelNo] == nullptr) {
                        channelCommands[channelNo] = (char*)malloc(512);
                    }
                    strcpy(channelCommands[channelNo], strtok(nullptr, ""));
                }
            }
            
            fclose(file);
        }
    } while (0);
    break;
/*
    case ID_CONFIG_BUTTON:
    do {
        char path[256];

#if (defined(VDJ_WIN))

        //strcat(path, getenv("USERPROFILE"));
        //strcat(path, "\\artnet.cfg");
        //strcat(path, ".\\Documents\\VirtualDJ\\artnet.cfg");

        wchar_t* pathW = nullptr;
        SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, (PWSTR*)pathW);
        wcstombs(path, pathW, 256);
        strcat(path, "\\VirtualDJ\\Plugins\\AutoStart\\VDJartnet\\config.exe");

#elif (defined(VDJ_MAC))

        strcat(path, "open ");
        strcat(path, getenv("HOME"));
        strcat(path, "/Documents/VirtualDJ/Plugins64/AutoStart/VDJartnet/config.app");

        //system(path);

        //CFURLRef url = CFURLCreateWithString(nullptr, CFStringCreateWithCString(nullptr, (char*)path + 1, kCFStringEncodingASCII), nullptr);

        //LSOpenCFURLRef(url, nullptr);
#endif
    } while (0);
    break;
s*/
    }

    return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CVDJartnet::OnGetParameterString(int id, char *outParam, int outParamSize) {
    switch(id) {
        case ID_ENABLE_BUTTON:
            break;
        case ID_REFRESH_BUTTON:
            break;
        case ID_CONFIG_BUTTON:
            break;
    }

    return S_OK;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void CVDJartnet::updateDMXvalues() {
    if (m_Enable == 1) {
        bool updated = false;

        for (int i = 0; i < noChannels; i++) {
            if (channelCommands[i] != nullptr) {
                double resultDouble = -1;
                GetInfo(channelCommands[i], &resultDouble);
                int resultInt = (int)round(resultDouble);
                if (resultInt >= 0 && resultInt <= 255) {
                    if (packet.data[i] != resultInt) {
                        packet.data[i] = resultInt;
                        updated = true;
                    }
                }
            }
        }

        if (updated || skippedPackets > 100) {
            sendArtnetPacket();
        } else {
            skippedPackets += 1;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void CVDJartnet::sendArtnetPacket() {
    zed_net_udp_socket_send(&socket, address, &packet, sizeof(packet));
    if (packet.sequence == 0xFF) {
        packet.sequence = 1;
    } else {
        packet.sequence += 1;
    }
    skippedPackets = 0;
}
