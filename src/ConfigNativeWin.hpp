//
//  ConfigNativeMac.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigNativeMac_hpp
#define ConfigNativeMac_hpp

#include "ConfigNative.hpp"

#ifndef CLRFREE
#include "ConfigWinTool.hpp"
#include <msclr\gcroot.h>
#endif

#include <string>
#include <vector>
#include <functional>

class CVDJartnet;

class ConfigNativeWin : public ConfigNative {
public:
    ConfigNativeWin(Config* config);
    void presentConfigTool();
    void presentText(std::string message, std::string information);
    int presentDialog(std::string message, std::string information, std::vector<std::string> responses, std::function<int()> callback);
private:
    Config* config;
#ifndef CLRFREE
    msclr::gcroot<ConfigWinTool^> configTool; /**< The managed ConfigWinTool */
#endif
};

#endif /* ConfigNativeMac_hpp */
