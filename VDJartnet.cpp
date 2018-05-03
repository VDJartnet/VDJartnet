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

		setupThread = new std::thread(globalSetup);
    }
    return S_OK;
}
//-----------------------------------------------------------------------------
void CVDJartnet::init() {
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
    }

	if (pollThread == nullptr) {
		pollThread = new std::thread(globalUpdate);
	}
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

        case ID_SAVE:
            config->saveConfig();
            break;

        case ID_CONFIG_BUTTON:
            if (m_Config == 1) {
#if (defined(VDJ_WIN))
                configWindow = createConfigWindow(this);
#elif (defined(VDJ_MAC))
                if (configWindow != nullptr) {
                    CFRelease(configWindow);
                    configWindow = nullptr;
                }
                configWindow = (__bridge_retained void*)[[ConfigWindow alloc] initWithVDJartnet: this];
#endif
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

        for (int i = 0; i < 512; i++) {
            if (!config->channelCommands[i].empty()) {
                double resultDouble = -1;
                SendCommand("set $VDJartnetSend 256");
                SendCommand(config->channelCommands[i].c_str());
                GetInfo("get_var $VDJartnetSend", &resultDouble);
                int resultInt = (int)round(resultDouble);
                if (resultInt >= 0 && resultInt <= 255) {
					updated = artnet.setChannel(i, resultInt);
                }
            }
        }

        if (updated || skippedPackets > skipPacketLimit) {
            artnet.sendArtnetPacket(config->host, config->port);
            skippedPackets = 0;
        } else {
            skippedPackets += 1;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void globalSetup() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(start + std::chrono::seconds(1));
    CVDJartnet::getInstance()->init();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void globalUpdate() {
    for (;;) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        CVDJartnet::getInstance()->updateDMXvalues();
		std::this_thread::sleep_until(start + CVDJartnet::getInstance()->config->getCheckRate());
    }
}
