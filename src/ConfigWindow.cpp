//
//  ConfigWindow.cpp
//  VDJartnet
//
//  Copyright � 2017-18 Jonathan Tanner. All rights reserved.
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
//Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of the Visual C++ Runtime used as well as that of the covered work.

#include "ConfigWindow.hpp"

ConfigWindow::ConfigWindow(Config* config) :
    window(new CSWindow(CSRect(0, 0, 600, 600),
                        "VDJartnetConfig",
                        true,
                        true)),
    config(config) {
    
    undoManager = new CSUndoManager;
    dataSource = new ConfigDataSource(config, nullptr);
    presetWindow = new ConfigPresetWindow(config);

    window->setClosingCallback([this]() {
        this->window->hide();
        this->presetWindow->hide();
        return false;
    });

    mainView = new CSAlignView(CSAlignView::Direction::Vertical);
    ipFields = new CSAlignView(CSAlignView::Direction::Horizontal);
    ipFields->setHeight(50);

    ipLabel = new CSLabel("IP Address:");
    ipFields->addView(ipLabel, false);

    ipAddress = new CSTextField();
    ipAddress->setText(config->host);
    ipAddress->setCallback([this](std::string str) { this->updateIPaddress(); return true; });
    ipFields->addView(ipAddress, true);

    ipPort = new CSTextField();
    ipPort->setText(std::to_string(config->port));
    ipPort->setCallback([this](std::string str) { this->updateIPport(); return true; });
    ipPort->setWidth(50);
    ipFields->addView(ipPort, false);

    mainView->addView(ipFields, false);

    tableView = new CSTableView();
    tableView->setDataSource(dataSource);
    tableView->addColumn("VDJscript");
    mainView->addView(tableView, true);

    CSMenuItem* undoItem = new CSMenuItem("Undo", [this]() { this->undo(); }, CSKeyCode("Z", true, false, false));
    CSMenuItem* redoItem = new CSMenuItem("Redo", [this]() { this->redo(); }, CSKeyCode("Z", true, false, true));
    CSMenuItem* copyItem = new CSMenuItem("Copy", [this]() { this->copyRow(); }, CSKeyCode("E", true, false, false));
    CSMenuItem* pasteItem = new CSMenuItem("Paste", [this]() { this->pasteRow(); }, CSKeyCode("R", true, false, false));
    CSMenuItem* deleteItem = new CSMenuItem("Delete", [this]() { this->deleteRow(); }, CSKeyCode("D", true, false, false));

    CSSubMenu* editMenu = new CSSubMenu("Edit");
    editMenu->addItems(undoItem,
                       redoItem,
                       copyItem,
                       pasteItem,
                       deleteItem);

    CSMenuBar* menuBar = new CSMenuBar();
    menuBar->addSubMenu(editMenu);

    window->presentView(mainView, menuBar);
}

void ConfigWindow::show() {
    window->show();
    tableView->setHeaderColumn("Channel"); // Can only do this after the tableView is shown
    presetWindow->show();
}

void ConfigWindow::updateIPaddress() {
    config->host = ipAddress->getText();
    config->saveConfig();
}

void ConfigWindow::updateIPport() {
    config->port = (unsigned short)std::stoi(ipPort->getText());
    config->saveConfig();
}

void ConfigWindow::undo() {
    if (undoManager->canUndo()) {
        undoManager->undo();
        tableView->reload();
    }
}

void ConfigWindow::redo() {
    if (undoManager->canRedo()) {
        undoManager->redo();
        tableView->reload();
    }
}

void ConfigWindow::copyRow() {
    int row = tableView->getSelectedRow();
    if (row >= 0) {
        CSClipboard::clear();
        CSClipboard::setStringValue(dataSource->getChannelCommand(row));
    }
}

void ConfigWindow::pasteRow() {
    int row = tableView->getSelectedRow();
    if (row >= 0) {
        dataSource->setChannelCommand(row, CSClipboard::getStringValue());
        tableView->reload();
    }
}

void ConfigWindow::deleteRow() {
    int row = tableView->getSelectedRow();
    if (row >= 0) {
        config->channelCommands[row] = "";
        config->saveConfig();
        tableView->reload();
    }
}
