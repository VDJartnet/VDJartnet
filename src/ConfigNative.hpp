//
//  ConfigNative.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigNative_h
#define ConfigNative_h

#include <string>

class ConfigNative {
public:
    virtual ~ConfigNative() {}
    virtual void presentConfigTool() = 0;
    virtual void presentText(std::string message, std::string information) = 0;
    virtual int presentDialog(std::string message, std::string information, std::vector<std::string> responses, std::function<int()> callback) = 0;
};

#endif /* ConfigNative_h */
