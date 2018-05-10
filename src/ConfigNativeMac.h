//
//  ConfigNativeMac.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigNativeMac_hpp
#define ConfigNativeMac_hpp

#include "Config.hpp"
#include "ConfigNative.hpp"
#include "ConfigMacTool.h"

#if (defined(VDJ_MAC))
#include "ConfigMacTool.h"
#include <CoreFoundation/CoreFoundation.h>
#elif (defined(VDJ_WIN))
#define CLRFREE
#include "ConfigWin.hpp"
#endif

#include <functional>

class ConfigNativeMac : public ConfigNative {
public:
    ConfigNativeMac(Config* configTMP);
    ~ConfigNativeMac();
    void presentConfigTool();
    void presentText(std::string message, std::string information);
    int presentDialog(std::string message, std::string information, std::vector<std::string> responses, std::function<int()> callback);
private:
    Config* config;
    __strong ConfigMacTool* configMacTool;
};

#endif /* ConfigNativeMac_hpp */
