//
//  ConfigPresetDataSource.hpp
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

#ifndef ConfigPresetDataSource_hpp
#define ConfigPresetDataSource_hpp

#include "Config.hpp"
#include "CppStep/src/CSTableViewDataSource.hpp"

#include <string>
#include <utility>

/** A data source for a CSTableView */
class ConfigPresetDataSource : public CSTableViewDataSource {
private:
    Config* config;
public:
    ConfigPresetDataSource(Config* config) : config(config) {}
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
    virtual std::string getStringValueInCell(std::string col, std::size_t row, bool editing) {
        if (col == "Presets") {
            return config->getPresets()[row].toShow();
        } else {
            throw "Column not recognised";
        }
    }

    virtual bool canDragFromRow(std::size_t row) { return true; }
    virtual bool canDropIntoRow(std::size_t row) { return false; }

    virtual std::string dragStringValueFromRow(std::size_t row) {
        return config->getPresets()[(std::size_t)row].command;
    }
};

#endif /* ConfigPresetDataSource_hpp */
