//
//  ConfigWin.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#include "ConfigWin.hpp"

ConfigWindow::ConfigWindow(CVDJartnet* vdjArtnet) {
    window = gcnew Form;
    window->Name = "VDJartnetConfig";
    window->Text = "VDJartnetConfig";
    window->StartPosition = FormStartPosition::CenterScreen;

    tableView = gcnew DataGridView;
    tableView->Name = "VDJscript";
    tableView->Location = System::Drawing::Point( 96, 71 );
    tableView->Size = System::Drawing::Size( 321, 286 );

    dataSource = gcnew ConfigDataSource(vdjArtnet);
    tableView->DataSource = dataSource->DataSource;

    DataGridViewTextBoxColumn^ vdjScriptColumn = gcnew DataGridViewTextBoxColumn();
    vdjScriptColumn->Name = "VDJscript";
    vdjScriptColumn->DataPropertyName = "Value";
    vdjScriptColumn->ReadOnly = false;
    tableView->Columns->Add(vdjScriptColumn);

    for each (DataGridViewRow^ row in tableView->Rows) {
        row->HeaderCell->Value = (row->Index + 1).ToString();
    }

    window->Controls->Add(tableView);
    window->Show();
}
