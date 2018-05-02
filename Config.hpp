//
//  Config.hpp
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

#ifndef Config_hpp
#define Config_hpp

#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include <iostream>

struct Preset {
    std::string name;
    std::string preset;
}

class Config {
public:
    std::string channelCommands[512];
    std::string host = "127.0.0.1";
    unsigned short port = 0x1936;

    std::vector<Preset> const& getPresets() { return &presets }

    int getSkipPacketLimit() { return skipPacketLimit; }
    int getCheckRate() { return checkRate; }

    Config(std::string configPathTMP, std::string presetsPathTMP)
    void loadConfig();
    void saveConfig();

private:
    std::string configPath;

    std::vector<Preset> presets;

    int skipPacketLimit;
    std::chrono::milliseconds checkRate = std::chrono::milliseconds(10);

    void loadPresetPresets();
    void parsePresetsFile(std::string path);

    void parseConfigFile(std::ifstream fin);
    void parseConfigLine(std::string line);
    void parseCommandConfigLine(std::string line);
};

std::istream& safeGetLine(std::istream& is, std::string& t);

#endif /* VDJartnet_hpp */
