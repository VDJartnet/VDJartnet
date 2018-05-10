//
//  ConfigTool.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#import "ConfigTool.hpp"

#if (defined(VDJ_MAC))

ConfigTool::ConfigTool(CVDJartnet* vdjArtnet) {
    configMacTool = [[ConfigMacTool alloc] initWithVDJartnet: vdjArtnet];
}

ConfigTool::~ConfigTool() {
    [configMacTool dealloc];
}

#elif (defined(VDJ_WIN))

ConfigTool::ConfigTool(CVDJartnet vdjArtnet) {
    if (configTool != nullptr) {
        closeConfigWinTool(configTool);
        delete configTool;
        configTool = nullptr;
    }
    configTool = createConfigWinTool(this);
}

#endif
