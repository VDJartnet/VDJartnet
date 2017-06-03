//
//  VDJartnet.cpp
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

#define ZED_NET_IMPLEMENTATION
#define VDJartnet_GLOBALIMPLEMENTATION
#include "VDJartnet.hpp"

#if (defined(VDJ_MAC))
#include "ConfigMac.h"
#endif

//-----------------------------------------------------------------------------
HRESULT VDJ_API CVDJartnet::OnLoad() {
    // ADD YOUR CODE HERE WHEN THE PLUGIN IS CALLED
    m_Enable = 0;
    m_Refresh = 0;

    for (int i = 0; i < 512; i++) {
        channelCommands[i] = "";
    }

    zed_net_init();
    zed_net_get_address(&address, host.c_str(), port);
    zed_net_udp_socket_open(&socket, 0, 0);

    globalCVDJartnet = this;

    DeclareParameterSwitch(&m_Enable,ID_ENABLE_BUTTON,"Enable","E", true);
    DeclareParameterButton(&m_Refresh,ID_REFRESH_BUTTON,"Refresh","R");
    DeclareParameterButton(&m_Config,ID_CONFIG_BUTTON,"Config","C");

    //OnParameter(ID_REFRESH_BUTTON);

    pollThread = new std::thread(globalUpdate);
    setupThread = new std::thread(globalSetup);

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
    switch(id) {
    case ID_ENABLE_BUTTON:
        break;

    case ID_REFRESH_BUTTON:
    do {
        char path[256];
        GetStringInfo("get_vdj_folder", path, 256);
        for (int i = 0; i < 512; i++) {
            channelCommands[i] = "";
        }
#if (defined(VDJ_WIN))
        strcat(path, "\\Plugins\\AutoStart\\VDJartnet\\config.txt");
#elif (defined(VDJ_MAC))
        strcat(path, "/Plugins64/AutoStart/VDJartnet/config.txt");
#endif

        std::ifstream fin (path);

        if (fin.is_open()) {
            safeGetline(fin, host);
            zed_net_get_address(&address, host.c_str(), port);

            std::string line;
            safeGetline(fin, line);
            while (line != "") {
                parseConfigLine(line);
                safeGetline(fin, line);
            }

            fin.close();
        }

    } while (0);
    do {
        char path[256];
        GetStringInfo("get_vdj_folder", path, 256);
#if (defined(VDJ_WIN))
        strcat(path, "\\Plugins\\AutoStart\\VDJartnet\\presets.txt");
#elif (defined(VDJ_MAC))
        strcat(path, "/Plugins64/AutoStart/VDJartnet/presets.txt");
#endif

        presetFin = new std::ifstream(path);
    } while (0);
    break;

    case ID_SAVE:
    do {
        char path[256];
        GetStringInfo("get_vdj_folder", path, 256);
#if (defined(VDJ_WIN))
        strcat(path, "\\Plugins\\AutoStart\\VDJartnet\\config.txt");
#elif (defined(VDJ_MAC))
        strcat(path, "/Plugins64/AutoStart/VDJartnet/config.txt");
#endif

        std::ofstream fout (path);

        if (fout.is_open()) {
            fout << host << std::endl;

            for (int i = 0; i < noChannels; i++) {
                if (channelCommands[i] != "") {
                    fout << std::string(3 - (floor(log10(i + 1)) + 1),'0') << std::to_string(i + 1) << '~' << channelCommands[i] << std::endl;
                }
            }

            fout.close();
        }
    } while (0);
    break;

    case ID_CONFIG_BUTTON:
    if (m_Config == 1) {
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

            strcat(path, "\\Plugins\\AutoStart\\VDJartnet\\config.winapp\\config.exe");

            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory( &si, sizeof(si) );
            si.cb = sizeof(si);
            ZeroMemory( &pi, sizeof(pi) );

            CreateProcess(path, nullptr, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);

            #elif (defined(VDJ_MAC))

//            if (![[configWindow window] isVisible]) {
            if (configWindow != nullptr) {
                CFRelease(configWindow);
                configWindow = nullptr;
            }
            configWindow = (__bridge_retained void*)[[ConfigWindow alloc] initWithVDJartnet: this];
//            }

/*
            //strcat(path, getenv("HOME"));
            //strcat(path, "/Documents/VirtualDJ/Plugins64/AutoStart/VDJartnet/config.txt");

            strcat(path, "/Plugins64/AutoStart/VDJartnet/config.app");


            //system(path);

            //CFURLRef url = CFURLCreateWithString(nullptr, CFStringCreateWithCString(nullptr, path, kCFStringEncodingASCII), nullptr);
            //CFArrayRef urls = CFArrayCreate(kCFAllocatorDefault, [url], 1, nullptr);

            //LSOpenCFURLRef(url, nullptr);
            //LSOpenURLsWithRole(urls, kLSRolesShell, nullptr, nullptr, nullptr, 0);

            //NSWorkspace.sharedWorkspace->launchApplication([NSString stringWithUTF8String:path]);
            [[NSWorkspace sharedWorkspace] launchApplication: [NSString stringWithUTF8String:path]];
*/
    #endif
        } while (0);
    }
    break;

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
            if (channelCommands[i] != "") {
                double resultDouble = -1;
                SendCommand("set $VDJartnetSend 256");
                SendCommand(channelCommands[i].c_str());
                GetInfo("get_var $VDJartnetSend", &resultDouble);
                //GetInfo(channelCommands[i].c_str(), &resultDouble);
                int resultInt = (int)round(resultDouble);
                if (resultInt >= 0 && resultInt <= 255) {
                    if (packet.data[i] != resultInt) {
                        packet.data[i] = resultInt;
                        updated = true;
                    }
                }
            }
        }

        if (updated || skippedPackets > skipPacketLimit) {
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
//-------------------------------------------------------------------------------------------------------------------------------------
void CVDJartnet::parseCommandConfigLine(std::string line){
  int posOfDelim = (int)line.find('~'); //Convert unsigned long to int explicitly to stop compiler complaining
  std::string channelNoS = line.substr(0, posOfDelim);
  if ((channelNoS.find_first_not_of("0123456789") == std::string::npos)) {
    int channelNo = stoi(channelNoS) - 1;

    if (channelNo < noChannels && channelNo >= 0) {
        channelCommands[channelNo] = line.substr(posOfDelim + 1, std::string::npos);
    }
  }
}

void CVDJartnet::parseConfigLine(std::string line){
  if(line.at(0) == '@'){
    //include statement
    //load a second fine and begin a parse on that
    //included file cannot include hostname
    std::string path = line.substr(1, std::string::npos);

    loadConfigNoHost(path);
    //finished with new file
    return;
  }

  if(line.substr(0,2).compare("+T") == 0){
    std::string rateS = line.substr(2, std::string::npos);
    skipPacketLimit = stoi(rateS);
  }

  if(line.substr(0,2).compare("+C") == 0){
    std::string rateS = line.substr(2, std::string::npos);
    checkRate = std::chrono::milliseconds(stoi(rateS));
  }

  //line does not match any special command line so assume it is a channel definition
  parseCommandConfigLine(line);
}

void CVDJartnet::loadConfigNoHost(std::string path){
  std::ifstream fin (path.c_str());

  if (fin.is_open()) {
      std::string line;

      safeGetline(fin, line);
      while (line != "") {
          parseConfigLine(line);
          safeGetline(fin, line);
      }

      fin.close();
  }
}
