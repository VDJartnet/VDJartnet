//
//  ConfigTableViewDataSource.hpp
//  CppStep
//
//  Copyright � 2018 Jonathan Tanner. All rights reserved.
//
//This file is part of CppStep.
//
//CppStep is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//CppStep is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with CppStep.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ConfigTableViewDataSource_hpp
#define ConfigTableViewDataSource_hpp

#include "Config.hpp"
#include "CppStep/src/CSTableViewDataSource.hpp"

#include <string>
#include <utility>

/** A data source for a CSTableView */
class ConfigTableViewDataSource : public CSTableViewDataSource {
private:
    Config * config;
public:
    ConfigTableViewDataSource(Config* config) : config(config) {}
    virtual int numberOfRows() { return 512; };
    virtual int numberOfColumns() { return 2; };
    virtual std::string getColumnName(int index) {
        switch (index) {
            case 0: return "Channel";
            case 1: return "VDJscript";
            default: throw "Column not recognised";
        }
    }
    virtual bool isReadOnly(std::string col) {
        if (col == "Channel") {
            return true;
        } else if (col == "VDJscript") {
            return false;
        } else {
            throw "Column not recognised";
        }
    }
    virtual std::string getStringValueInCell(std::string col, int row) {
        if (col == "Channel") {
            return std::to_string(row + 1);
        }
        else if (col == "VDJscript") {
            return config->channelCommands[row];
        }
        else {
            throw "Column not recognised";
        }
    }
    virtual void setStringValueInCell(std::string col, int row, std::string value) {
        if (col == "Channel") {
            throw "Cannot set channel - read only";
        }
        else if (col == "VDJscript") {
            config->channelCommands[row] = value;
            config->saveConfig();
        } else {
            throw "Column not recognised";
        }
    }

    virtual bool canDragFromRow(int row) { return true; }
    virtual bool canDropIntoRow(int row) { return true; }

    virtual std::string dragStringValueFromRow(int row) {
        return config->channelCommands[row];
    }
    virtual void dropStringValueInRow(int row, std::string value) {
        config->channelCommands[row] = value;
        config->saveConfig();
    }
};

#endif /* ConfigTableViewDataSource_hpp */
