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

ConfigWindow::ConfigWindow(Config* config)
    : window(new CSWindow(CSRect(0, 0, 600, 600),
                          "VDJartnetConfig",
                          true,
                          true)),
      config(config) {

    window->setClosingCallback([this]() {
        this->window->hide();
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
    tableView->setDataSource(new ConfigTableViewDataSource(config));
    tableView->addColumn("VDJscript");
    mainView->addView(tableView, true);

    window->presentView(mainView);
}

void ConfigWindow::show() {
    window->show();
    tableView->setHeaderColumn("Channel"); // Can only do this after the tableView is shown
}

    // for each (DataGridViewRow^ row in tableView->Rows) {
    //     row->HeaderCell->Value = String::Format("{0}", row->Index + 1);
    // }
    // tableView->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);

//void ConfigWindow::didClose(Object^ sender, FormClosedEventArgs^ e) {
//    for each (Form^ child in children) {
//        child->Hide();
//    }
//}

void ConfigWindow::updateIPaddress() {
    config->host = ipAddress->getText();
    config->saveConfig();
}

void ConfigWindow::updateIPport() {
    config->port = (unsigned short)std::stoi(ipPort->getText());
    config->saveConfig();
}


void ConfigWindow::addChildWindow(CSWindow* child) {
    //children->Add(child);
}
