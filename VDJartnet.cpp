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
//
//If you modify this Program, or any covered work, by linking or
//combining it with the Visual C++ Runtime, the licensors of this Program grant you
//additional permission to convey the resulting work.

#include "VDJartnet.hpp"

#if (defined(VDJ_WIN))
#define CLRFREE
#include "ConfigWin.hpp"
#elif (defined(VDJ_MAC))
#include "ConfigMac.h"
#endif

HRESULT VDJ_API CVDJartnet::OnLoad() {
    // ADD YOUR CODE HERE WHEN THE PLUGIN IS CALLED
    if (!globalCVDJartnetLoaded) {
        globalCVDJartnetLoaded = true;

        m_Enable = 1;
        m_Refresh = 0;

        for (int i = 0; i < 512; i++) {
            channelCommands[i] = "";
        }

        address = new Address(host.c_str(), port);
        delete globalCVDJartnetSocket;
        globalCVDJartnetSocket = new Socket(64444, 0);

        DeclareParameterSwitch(&m_Enable,ID_ENABLE_BUTTON,"Enable","E", true);
        DeclareParameterButton(&m_Refresh,ID_REFRESH_BUTTON,"Refresh","R");
        DeclareParameterButton(&m_Config,ID_CONFIG_BUTTON,"Config","C");

        pollThread = new std::thread(globalUpdate);
        setupThread = new std::thread(globalSetup);
    }
    return S_OK;
}

HRESULT VDJ_API CVDJartnet::OnGetPluginInfo(TVdjPluginInfo8 *infos) {
    infos->PluginName = "VDJartnet";
    infos->Author = "Jonathan Tanner";
    infos->Description = "Send Artnet Commands from VirtualDJ";
    infos->Version = "1.0";
    infos->Flags = 0x00;
    infos->Bitmap = NULL;
    
    return S_OK;
}

ULONG VDJ_API CVDJartnet::Release() {
#if (defined(VDJ_WIN))
    closeConfigWindow(configWindow);
    delete configWindow;
    configWindow = nullptr;
#elif (defined(VDJ_MAC))
    if (configWindow != nullptr) {
        CFRelease(configWindow);
        configWindow = nullptr;
    }
#endif

    delete globalCVDJartnetSocket;
    
    delete this;
    return 0;
}

HRESULT VDJ_API CVDJartnet::OnGetUserInterface(TVdjPluginInterface8 *pluginInterface) {
    
    pluginInterface->Type = VDJINTERFACE_DEFAULT;
    
    return S_OK;
}

HRESULT VDJ_API CVDJartnet::OnParameter(int id) {
    switch(id) {
        case ID_ENABLE_BUTTON:
            break;

        case ID_REFRESH_BUTTON:
            do {
                char cPath[256];
                GetStringInfo("get_vdj_folder", cPath, 256);
                for (int i = 0; i < 512; i++) {
                    channelCommands[i] = "";
                }
                std::string path = std::string(cPath) + pluginPath + "config.txt";

                std::ifstream fin(path);

                if (fin.is_open()) {
                    safeGetline(fin, host);
                    address = new Address(host.c_str(), port);

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
                char cPath[256];
                GetStringInfo("get_vdj_folder", cPath, 256);
                std::string path = std::string(cPath) + pluginPath + "presets.txt";

                presetFin = new std::ifstream(path);
            } while (0);
            break;

        case ID_SETUP:
            do {
                char cPath[256];
                GetStringInfo("get_vdj_folder", cPath, 256);
                std::string path = std::string(cPath);
                std::string configPath = path + pluginPath + "config.txt";
                std::string presetsPath = path + pluginPath + "presets.txt";

                std::ofstream configFout(configPath);
                std::ofstream presetsFout(presetsPath);

                configFout << "127.0.0.1" << std::endl;
                configFout.close();
                
                presetsFout << "hi" << std::endl;
                presetsFout.close();
            } while (0);
            break;

        case ID_SAVE:
            do {
                address = new Address(host.c_str(), port);

                char cPath[256];
                GetStringInfo("get_vdj_folder", cPath, 256);
                std::string path = std::string(cPath) + pluginPath + "config.txt";

                std::ofstream fout (path);

                if (fout.is_open()) {
                    fout << host << std::endl;

                    for (int i = 0; i < noChannels; i++) {
                        if (channelCommands[i] != "") {
                            fout << toStringOfLength(4, i + 1) << '~' << channelCommands[i] << std::endl;
                        }
                    }

                    fout.close();
                }
            } while (0);
            break;

        case ID_CONFIG_BUTTON:
            if (m_Config == 1) {
                do {
#if (defined(VDJ_WIN))
                    configWindow = createConfigWindow(this);
#elif (defined(VDJ_MAC))
                    if (configWindow != nullptr) {
                        CFRelease(configWindow);
                        configWindow = nullptr;
                    }
                    configWindow = (__bridge_retained void*)[[ConfigWindow alloc] initWithVDJartnet: this];
#endif
                } while (0);
            }
            break;

    }
    
    return S_OK;
}

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
                int resultInt = (int) round(resultDouble);
                if (resultInt >= 0 && resultInt <= 255) {
                    uint8_t resultInt8 = (uint8_t) resultInt;
                    if (packet.data[i] != resultInt8) {
                        packet.data[i] = resultInt8;
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

void CVDJartnet::sendArtnetPacket() {
    if (globalCVDJartnetSocket != nullptr) {
        globalCVDJartnetSocket->send(address, &packet, sizeof(packet));
        if (packet.sequence == 0xFF) {
            packet.sequence = 1;
        } else {
            packet.sequence += 1;
        }
        skippedPackets = 0;
    }
}

void CVDJartnet::parseCommandConfigLine(std::string line) {
    size_t posOfDelim = line.find('~');
    std::string channelNoS = line.substr(0, posOfDelim);
    if ((channelNoS.find_first_not_of("0123456789") == std::string::npos)) {
        int channelNo = stoi(channelNoS) - 1;

        if (channelNo < noChannels && channelNo >= 0) {
            channelCommands[channelNo] = line.substr(posOfDelim + 1, std::string::npos);
        }
    }
}

void CVDJartnet::parseConfigLine(std::string line) {
    if(line.at(0) == '@'){
        //include statement
        //load a second file and begin a parse on that
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
        std::string checkS = line.substr(2, std::string::npos);
        checkRate = std::chrono::milliseconds(stoi(checkS));
    }

    //line does not match any special command line so assume it is a channel definition
    parseCommandConfigLine(line);
}

void CVDJartnet::loadConfigNoHost(std::string path) {
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

CVDJartnet* globalCVDJartnet = new CVDJartnet();//(CVDJartnet*)malloc(sizeof(CVDJartnet));
Socket* globalCVDJartnetSocket;
bool globalCVDJartnetLoaded = false;

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
    globalCVDJartnet->OnParameter(globalCVDJartnet->ID_SETUP);
    globalCVDJartnet->OnParameter(globalCVDJartnet->ID_REFRESH_BUTTON);
}

std::istream& safeGetline(std::istream& is, std::string& t) {
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

std::string toStringOfLength(size_t len, int valueInt) {
    std::string valueStr = std::to_string(valueInt);
    std::string padding = std::string(len - valueStr.length(), '0');
    return padding + valueStr;
}

