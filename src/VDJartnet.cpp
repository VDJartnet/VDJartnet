//
//  VDJartnet.cpp
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

#include "VDJartnet.hpp"

#include <thread> //Include here because thread is incompatible with CLR

HRESULT VDJ_API CVDJartnet::OnLoad() {
    // ADD YOUR CODE HERE WHEN THE PLUGIN IS CALLED
    static bool isLoaded = false;
    if (!isLoaded) {
        isLoaded = true;

        m_Enable = 1;
        m_Refresh = 0;

        DeclareParameterSwitch(&m_Enable,ID_ENABLE_BUTTON,"Enable","E", true);
        DeclareParameterButton(&m_Refresh,ID_REFRESH_BUTTON,"Refresh","R");
        DeclareParameterButton(&m_Config,ID_CONFIG_BUTTON,"Config","C");
        DeclareParameterButton(&m_About, ID_ABOUT_BUTTON, "About", "A");

        setupThread = new std::thread(CVDJartnet::setup);
    }
    return S_OK;
}
//-----------------------------------------------------------------------------
void CVDJartnet::init() {
    CSApp::Init();

    if (config == nullptr) {
        char pathC[256];
        GetStringInfo("get_vdj_folder", pathC, 256);
        std::string path(pathC);
#if (defined(VDJ_WIN))
        path += std::string("\\Plugins\\AutoStart\\VDJartnet.conf");
#elif (defined(VDJ_MAC))
        path += std::string("/Plugins64/AutoStart/VDJartnet.conf");
#endif

        config = new Config(path);
// #if (defined(VDJ_MAC))
//         configTool = new ConfigNativeMac(config);
// #elif (defined(VDJ_WIN))
//         configTool = new ConfigNativeWin(config);
// #endif
        configTool = new ConfigWindow(config);
    }

    if (pollThread == nullptr) {
        pollThread = new std::thread(CVDJartnet::update);
    }

    CSApp::Run(false);
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

ULONG VDJ_API CVDJartnet::Release() {
    delete config;
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
            config->loadConfig();
            break;

        case ID_CONFIG_BUTTON:
            if (m_Config == 1) {
                configTool->show();
                //configTool->presentConfigTool();
            }
            break;

        case ID_ABOUT_BUTTON:
            if (m_About == 1) {
                //configTool->presentText("About VDJartnet",
//#include "about.txt"
                                        //);
            }
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
        case ID_ABOUT_BUTTON:
            break;
    }

    return S_OK;
}

void CVDJartnet::updateDMXvalues() {
    if (m_Enable == 1) {
        bool updated = false;

        for (std::size_t i = 0; i < 512; i++) {
            if (!config->channelCommands[i].command.empty()) {
                double resultDouble = -1;
                SendCommand("set $VDJartnetSend 0");
                SendCommand(config->channelCommands[i].command.c_str());
                GetInfo("get_var $VDJartnetSend", &resultDouble);
                int resultInt = (int)round(resultDouble);
                if (resultInt < 0) {
                    resultInt = 0;
                }
                if (resultInt > 255) {
                    resultInt = 255;
                }
                updated = artnet.setChannel(i, (uint8_t)resultInt);
            } else {
                updated = artnet.setChannel(i, 0);
            }
        }

        if (updated || skippedPackets > config -> getSkipPacketLimit()) {
            artnet.sendArtnetPacket(config->host, config->port);
            skippedPackets = 0;
        } else {
            skippedPackets += 1;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void CVDJartnet::setup() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(start + std::chrono::seconds(1));
    CSThread::dispatchMain([](){CVDJartnet::getInstance()->init();});
}
//-------------------------------------------------------------------------------------------------------------------------------------
void CVDJartnet::update() {
    for (;;) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        CVDJartnet::getInstance()->updateDMXvalues();
        std::this_thread::sleep_until(start + CVDJartnet::getInstance()->config->getCheckRate());
    }
}
