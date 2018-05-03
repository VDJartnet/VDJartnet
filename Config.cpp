//
//  Config.cpp
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

#include "Config.hpp"

Config::Config(std::string configPathTMP) {
    configPath = configPathTMP;
    loadConfig();
}


void Config::loadConfig() {
    std::ifstream fin(configPath);
    if (fin.is_open()) {
        for (int i = 0; i < 512; i++) {
            channelCommands[i] = "";
        }

        std::string line;
        safeGetLine(fin, line);
        size_t delimPos = line.find(":");
        host = line.substr(0, delimPos);
        if (delimPos != std::string::npos) {
            std::string portS = line.substr(delimPos + 1, std::string::npos);
            if (portS.find_first_not_of("0123456789") == std::string::npos) {
                port = (unsigned short)std::stoi(portS);
            }
        }
    }
    presets.clear();
    parseConfigFile(fin);
    loadPresetPresets();
}

void Config::loadPresetPresets() {

}

void Config::parsePresetsFile(std::ifstream& fin) {
    if (fin.is_open()) {
        std::string line;
        safeGetLine(fin, line);
        size_t delimPos = line.find("~");
        Preset preset;
        preset.name = line.substr(0, delimPos);
        preset.preset = line.substr(delimPos + 1, std::string::npos);
        presets.push_back(preset);
        fin.close();
    }
}

void Config::saveConfig() {
    std::ofstream fout(configPath);
    if (fout.is_open()) {
        fout << host << std::endl;
        for (int i = 0; i < 512; i++) {
            if (channelCommands[i] != "") {
                fout << std::string(3 - ((int)floor(std::log10(i + 1)) + 1),'0') << std::to_string(i + 1) << '~' << channelCommands[i] << std::endl;
            }
        }
        fout.close();
    }
}

void Config::parseConfigFile(std::ifstream& fin){
    if (fin.is_open()) {
        std::string line;
        safeGetLine(fin, line);
        while (line != "") {
            parseConfigLine(line);
            safeGetLine(fin, line);
        }
        fin.close();
    }
}

void Config::parseConfigLine(std::string line){
    if (line.at(0) == '@'){
        //include statement
        //load a second file and begin a parse on that
        //included file cannot include hostname
        std::string path = line.substr(2, std::string::npos);
        std::ifstream fin(path);
        if (line.at(1) == 'c') {
            parseConfigFile(fin);
        } else if (line.at(1) == 'p') {
            parsePresetsFile(fin);
        }
        //finished with new file
        return;
    }

    if( line.substr(0,2).compare("+T") == 0){
        std::string rateS = line.substr(2, std::string::npos);
        if (rateS.find_first_not_of("0123456789") == std::string::npos) {
            skipPacketLimit = stoi(rateS);
        }
        return;
    }

    if(line.substr(0,2).compare("+C") == 0){
        std::string checkS = line.substr(2, std::string::npos);
        if (checkS.find_first_not_of("0123456789") == std::string::npos) {
            checkRate = std::chrono::milliseconds(stoi(checkS));
        }
        return;
    }

    //line does not match any special command line so assume it is a channel definition
    parseCommandConfigLine(line);
}

void Config::parseCommandConfigLine(std::string line){
    size_t delimPos = line.find('~');
    std::string channelNoS = line.substr(0, delimPos);
    if ((channelNoS.find_first_not_of("0123456789") == std::string::npos)) {
        int channelNo = stoi(channelNoS) - 1;
        if (channelNo < 512 && channelNo >= 0) {
            channelCommands[channelNo] = line.substr(delimPos + 1, std::string::npos);
        }
    }
}

std::istream& safeGetLine(std::istream& is, std::string& t) {
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                // Also handle the case when the last line has no line ending
                if(t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}

