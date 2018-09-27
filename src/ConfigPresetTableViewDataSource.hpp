//
//  ConfigPresetTableViewDataSource.hpp
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

#ifndef ConfigPresetTableViewDataSource_hpp
#define ConfigPresetTableViewDataSource_hpp

#include "Config.hpp"
#include "CppStep/src/CSTableViewDataSource.hpp"

#include <string>
#include <utility>

/** A data source for a CSTableView */
class ConfigPresetTableViewDataSource : public CSTableViewDataSource {
private:
    Config* config;
public:
    ConfigPresetTableViewDataSource(Config* config) : config(config) {}
    virtual int numberOfRows() { return (int)config->getPresets().size(); };
    virtual int numberOfColumns() { return 1; };
    virtual std::string getColumnName(int index) {
        switch (index) {
            case 0: return "Presets";
            default: throw "Column not recognised";
        }
    }
    virtual bool isReadOnly(std::string col) {
        if (col == "Presets") {
            return true;
        } else {
            throw "Column not recognised";
        }
    }
    virtual std::string getStringValueInCell(std::string col, int row) {
        if (col == "Presets") {
            return config->getPresets()[(unsigned long)row].name;
        } else {
            throw "Column not recognised";
        }
    }

    virtual bool canDragFromRow(int row) { return true; }
    virtual bool canDropIntoRow(int row) { return false; }

    virtual std::string dragStringValueFromRow(int row) {
        return config->getPresets()[(unsigned long)row].preset;
    }
};

#endif /* ConfigPresetTableViewDataSource_hpp */
