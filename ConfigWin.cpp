//
//  ConfigWin.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#include "ConfigWin.hpp"

ConfigWindow::ConfigWindow(CVDJartnet* vdjArtnetTMP) {
	_vdjArtnet = vdjArtnetTMP;
	
    window = gcnew Form;
    window->Name = "VDJartnetConfig";
    window->Text = "VDJartnetConfig";
	window->Size = System::Drawing::Size(600, 600);
    window->StartPosition = FormStartPosition::CenterScreen;
	
	ipLabel = gcnew Label();
	ipLabel->Text = "IP Address:";
	ipLabel->AutoSize = true;
	window->Controls->Add(ipLabel);

	ipAddress = gcnew TextBox();
	ipAddress->Text = gcnew String(_vdjArtnet->host.c_str());
	ipAddress->Leave += gcnew EventHandler(this, &ConfigWindow::updateIPaddress);
	ipAddress->KeyDown += gcnew KeyEventHandler(this, &ConfigWindow::ipKeyDown);
	window->Controls->Add(ipAddress);
	
    tableView = gcnew DataGridView;
    tableView->Name = "VDJscript";

    dataSource = gcnew ConfigDataSource(_vdjArtnet);
    tableView->DataSource = dataSource->DataSource;

    DataGridViewTextBoxColumn^ vdjScriptColumn = gcnew DataGridViewTextBoxColumn();
    vdjScriptColumn->Name = "VDJscript";
    vdjScriptColumn->DataPropertyName = "Value";
    vdjScriptColumn->ReadOnly = false;
    tableView->Columns->Add(vdjScriptColumn);
	tableView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

	window->Layout += gcnew LayoutEventHandler(this, &ConfigWindow::reLayout);
	window->FormClosed += gcnew FormClosedEventHandler(this, &ConfigWindow::didClose);
    window->Controls->Add(tableView);
	
    window->Show();
}

void ConfigWindow::reLayout(Object^ sender, LayoutEventArgs^ e) {
	ipLabel->Location = System::Drawing::Point(0, 0);
	
	ipAddress->Location = System::Drawing::Point(ipLabel->Width, 0);
	ipAddress->Size = System::Drawing::Size(window->Size.Width - ipLabel->Width, 20);	

    tableView->Location = System::Drawing::Point(0, 20);
    tableView->Size = System::Drawing::Size(window->Size.Width - 20, window->Size.Height - 70);	
	
    for each (DataGridViewRow^ row in tableView->Rows) {
        row->HeaderCell->Value = String::Format("{0}", row->Index + 1);
    }
	tableView->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);
}

void ConfigWindow::didClose(Object^ sender, FormClosedEventArgs^ e) {
	delete ((ConfigWindowNative*)(_vdjArtnet->configWindow));
	_vdjArtnet->configWindow = nullptr;
}

void ConfigWindow::updateIPaddress(Object^ sender, EventArgs^ e) {
	_vdjArtnet->host = msclr::interop::marshal_as<std::string>(ipAddress->Text);
    _vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
}

void ConfigWindow::ipKeyDown(Object^ sender, KeyEventArgs^ e) {
	if (e->KeyCode == Keys::Enter) {
		updateIPaddress(sender, e);
	}
}

