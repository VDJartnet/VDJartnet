//
//  ConfigNativeMac.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigNativeMac_hpp
#define ConfigNativeMac_hpp

#include "ConfigMacTool.h"

#if (defined(VDJ_MAC))
#include "ConfigMacTool.h"
#include <CoreFoundation/CoreFoundation.h>
#elif (defined(VDJ_WIN))
#define CLRFREE
#include "ConfigWin.hpp"
#endif

class CVDJartnet;

class ConfigNativeMac {
public:
    ConfigNativeMac(CVDJartnet* vdjArtnet);
    ~ConfigNativeMac();
private:
#if (defined(VDJ_MAC))
    __strong ConfigMacTool* configMacTool;
#elif (defined(VDJ_WIN))
#endif
};

#endif /* ConfigNativeMac_hpp */
