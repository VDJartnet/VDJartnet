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

#include "Artnet.hpp"
#include "Config.hpp"

// #if (defined(VDJ_MAC))
// #include "ConfigNativeMac.h"
// #elif (defined(VDJ_WIN))
// #define CLRFREE
// #include "ConfigNativeWin.hpp"
// #endif
#define CLRFREE
#include "CppStep/src/CSApp.hpp"
#include "ConfigWindow.hpp"

#include <string>
#include <fstream>
#include <chrono>
//Don't include thread here because thread is incompatible with CLR

#define commandLength 512

/** A singleton class representing the plugin */
class CVDJartnet : public IVdjPlugin8 {
public:
    /** Get the singleton instance of the plugin */
    static CVDJartnet* getInstance() {
        static CVDJartnet* instance = new CVDJartnet();
        return instance;
    }

    int m_Enable; /**< Whether the plugin is enabled */
    int m_Refresh; /**< Reload the config data */
    int m_Config; /**< Open the config tool */
    int m_About; /**< Open the about window */

    HRESULT VDJ_API OnLoad(); /**< The config has been loaded by VirtualDJ. VirtualDJ APIs cannot be called from this function */
    HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos); /**< Give the plugin info to VirtualDJ */
    ULONG VDJ_API Release(); /**< Release the plugin memory */
    HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8 *pluginInterface); /**< Give the plugin interface to VirtualDJ */
    HRESULT VDJ_API OnParameter(int id); /**< A button has been pressed */
    HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize); /**< Give the property label string to VirtualDJ */

    void init(); /**< Initialise the plugin once VirtualDJ APIs can be called */

    void updateDMXvalues(); /**< Get the DMX values from VirtualDJ and send them over Art-Net */

    /** The properties that can be passed to OnParameter(int id) */
    enum ID_Interface {
    ID_ENABLE_BUTTON,
    ID_REFRESH_BUTTON,
    ID_CONFIG_BUTTON,
    ID_ABOUT_BUTTON
    };

    Config* config; /**< The config parser */

    // ConfigNative* configTool; /**< A pointer to the config tool. */
    ConfigWindow* configTool; /**< A pointer to the config tool. */

private:
    Artnet artnet; /**< The object that sends the Art-Net data */

    int skippedPackets = 0; /**< The number of packets that have been skipped since the last sent packet */
    int skipPacketLimit = 10; /**< The maximum number of packets that can be skipped before one is sent */

    void* setupThread; /**< The thread used to initialise the plugin. Of type std::thread* but can't be of thread type as thread is incompatible with CLR. */
    void* pollThread; /**< The thread used to poll VirtualDJ and send the data over Art-Net. Of type std::thread* but can't be of thread type as thread is incompatible with CLR. */

    static void setup(); /**< Setup the singleton instance of the plugin */
    static void update(); /**< Poll VirtualDJ and send the data over Art-Net */
};


#endif /* VDJartnet_hpp */
