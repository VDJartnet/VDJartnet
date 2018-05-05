//
//  ConfigWinWindow.cpp
//  VDJartnet
//
//  Copyright © 2017-18 Jonathan Tanner. All rights reserved.
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

#include "ConfigWin.hpp"

ConfigWindow::ConfigWindow(CVDJartnet* vdjArtnetTMP) {
    vdjArtnet = vdjArtnetTMP;

    this->Name = "VDJartnetConfig";
    this->Text = "VDJartnetConfig";
    this->Size = System::Drawing::Size(600, 600);
    this->StartPosition = FormStartPosition::CenterScreen;
    this->KeyPreview = true;

    ipLabel = gcnew Label();
    ipLabel->Text = "IP Address:";
    ipLabel->AutoSize = true;
    this->Controls->Add(ipLabel);

    ipAddress = gcnew TextBox();
    ipAddress->Text = gcnew String(vdjArtnet->config->host.c_str());
    ipAddress->Leave += gcnew EventHandler(this, &ConfigWindow::updateIPaddress);
    ipAddress->KeyDown += gcnew KeyEventHandler(this, &ConfigWindow::ipKeyDown);
    this->Controls->Add(ipAddress);

    ipPort = gcnew TextBox();
    ipPort->Text = vdjArtnet->config->port.ToString();
    ipPort->Leave += gcnew EventHandler(this, &ConfigWindow::updateIPport);
    ipPort->KeyDown += gcnew KeyEventHandler(this, &ConfigWindow::ipKeyDown);
    this->Controls->Add(ipPort);

    tableView = gcnew ConfigTableView(vdjArtnet);
    this->Controls->Add(tableView);

    this->Layout += gcnew LayoutEventHandler(this, &ConfigWindow::reLayout);

    this->Show();

    this->FormClosed += gcnew FormClosedEventHandler(this, &ConfigWindow::didClose);
}

void ConfigWindow::reLayout(Object^ sender, LayoutEventArgs^ e) {
    ipLabel->Location = System::Drawing::Point(0, 0);

    ipPort->Size = System::Drawing::Size(50, 20);
    ipAddress->Size = System::Drawing::Size(this->ClientSize.Width - ipLabel->Width - ipPort->Width, 20);

    ipPort->Location = System::Drawing::Point(this->ClientSize.Width - ipPort->Width, 0);
    ipAddress->Location = System::Drawing::Point(ipLabel->Width, 0);

    tableView->Location = System::Drawing::Point(0, 20);
    tableView->Size = System::Drawing::Size(this->ClientSize.Width, this->ClientSize.Height - ipAddress->Height);

    for each (DataGridViewRow^ row in tableView->Rows) {
        row->HeaderCell->Value = String::Format("{0}", row->Index + 1);
    }
    tableView->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);
}

void ConfigWindow::didClose(Object^ sender, FormClosedEventArgs^ e) {
    ((ConfigToolNative*)(vdjArtnet->configTool))->configTool->hide();
    delete ((ConfigToolNative*)(vdjArtnet->configTool));
    vdjArtnet->configTool = nullptr;
}

void ConfigWindow::updateIPaddress(Object^ sender, EventArgs^ e) {
    vdjArtnet->config->host = msclr::interop::marshal_as<std::string>(ipAddress->Text);
    vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
}

void ConfigWindow::updateIPport(Object^ sender, EventArgs^ e) {
    vdjArtnet->config->port = (unsigned short)int::Parse(ipPort->Text);
    vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
}

void ConfigWindow::ipKeyDown(Object^ sender, KeyEventArgs^ e) {
    if (e->KeyCode == Keys::Enter) {
        updateIPaddress(sender, e);
        updateIPport(sender, e);
    }
}
