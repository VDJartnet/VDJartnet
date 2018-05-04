//
//  VDJartnet.hpp
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

#ifndef VDJartnet_hpp
#define VDJartnet_hpp

#include "vdjPlugin8.h"

#include "Config.hpp"
#include "Artnet.hpp"

#include <string>
#include <fstream>
#include <chrono>
//Don't include thread here because thread is incompatible with CLR

#define commandLength 512

class CVDJartnet : public IVdjPlugin8 {
public:
    static CVDJartnet* getInstance() {
		static CVDJartnet* instance = new CVDJartnet();
		return instance;
    }

    int m_Enable;
    int m_Refresh;
    int m_Config;

    HRESULT VDJ_API OnLoad();
    HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
    ULONG VDJ_API Release();
    HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8 *pluginInterface);
    HRESULT VDJ_API OnParameter(int id);
    HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize);

    void init();

    void updateDMXvalues();

    typedef enum _ID_Interface
    {
    ID_ENABLE_BUTTON,
    ID_REFRESH_BUTTON,
    ID_CONFIG_BUTTON,
    ID_SETUP,
    ID_SAVE
    } ID_Interface;

    Config* config;

    void* configTool;

private:
    Artnet artnet;

    int skippedPackets = 0;
    int skipPacketLimit = 10;

    void* setupThread; //std::thread but can't be of thread type as thread is incompatible with CLR
    void* pollThread; //std::thread but can't be of thread type as thread is incompatible with CLR
};

void globalSetup();
void globalUpdate();

#endif /* VDJartnet_hpp */
