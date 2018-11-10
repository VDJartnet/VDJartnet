//
//  ConfigDataSource.hpp
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

#ifndef ConfigDataSource_hpp
#define ConfigDataSource_hpp

#include "Config.hpp"
#include "CppStep/src/CSTableViewDataSource.hpp"
#include "CppStep/src/CSUndoManager.hpp"

#include <string>
#include <utility>
#include <functional>

/** A data source for a CSTableView */
class ConfigDataSource : public CSTableViewDataSource {
private:
    Config* config;
    CSUndoManager* undoManager;
public:
    std::function<void()> reloadTable = [](){};
    ConfigDataSource(Config* config,
                     CSUndoManager* undoManager) :
                     config(config),
                     undoManager(undoManager) {}
    virtual int numberOfRows() { return 512; };
    virtual int numberOfColumns() { return 3; };
    virtual std::string getColumnName(int index) {
        switch (index) {
            case 0: return "Channel";
            case 1: return "Name";
            case 2: return "VDJscript";
            default: throw "Column not recognised";
        }
    }
    virtual std::optional<std::string> headerColumn() {
        return "Channel";
    }
    virtual bool isReadOnly(std::string col) {
        if (col == "Channel") {
            return true;
        } else if (col == "Name") {
            return false;
        } else if (col == "VDJscript") {
            return false;
        } else {
            throw "Column not recognised";
        }
    }
    Config::Command getChannelCommand(std::size_t row) {
        return config->channelCommands[row];
    }
    void setChannelCommand(std::size_t row, Config::Command value) {
        if (undoManager != nullptr) {
            Config::Command oldValue = config->channelCommands[row];
            if (value != oldValue) {
                undoManager->registerUndoFunc([this, row, oldValue](){
                    this->setChannelCommand(row, oldValue);
                    this->reloadTable();
                });
            }
        }
        config->channelCommands[row] = value;
        config->saveConfig();
    }
    virtual std::string getStringValueInCell(std::string col, std::size_t row) {
        if (col == "Channel") {
            return std::to_string(row + 1);
        } else if (col == "Name") {
            return getChannelCommand(row).name;
        } else if (col == "VDJscript") {
            return getChannelCommand(row).command;
        } else {
            throw "Column not recognised";
        }
    }
    virtual void setStringValueInCell(std::string col, std::size_t row, std::string value) {
        if (col == "Channel") {
            throw "Cannot set channel - read only";
        } else if (col == "Name") {
            setChannelCommand(row, Config::Command(value, getStringValueInCell("VDJscript", row)));
        } else if (col == "VDJscript") {
            setChannelCommand(row, Config::Command(getStringValueInCell("Name", row), value));
        } else {
            throw "Column not recognised";
        }
    }

    virtual bool canDragFromRow(std::size_t row) { return true; }
    virtual bool canDropIntoRow(std::size_t row) { return true; }

    virtual std::string dragStringValueFromRow(std::size_t row) {
        return getChannelCommand(row).toLine();
    }
    virtual void dropStringValueInRow(std::size_t row, std::string value) {
        setChannelCommand(row, Config::Command(value));
    }
};

#endif /* ConfigDataSource_hpp */
