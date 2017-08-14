//
//  ConfigWin.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
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
#include "ConfigWin.hpp"

ConfigWindow::ConfigWindow(CVDJartnet* vdjArtnetTMP) {
	_vdjArtnet = vdjArtnetTMP;
	
    window = gcnew Form();
    window->Name = "VDJartnetConfig";
    window->Text = "VDJartnetConfig";
	window->Size = System::Drawing::Size(600, 600);
    window->StartPosition = FormStartPosition::CenterScreen;
	window->KeyPreview = true;
	
	ipLabel = gcnew Label();
	ipLabel->Text = "IP Address:";
	ipLabel->AutoSize = true;
	window->Controls->Add(ipLabel);

	ipAddress = gcnew TextBox();
	ipAddress->Text = gcnew String(_vdjArtnet->host.c_str());
	ipAddress->Leave += gcnew EventHandler(this, &ConfigWindow::updateIPaddress);
	ipAddress->KeyDown += gcnew KeyEventHandler(this, &ConfigWindow::ipKeyDown);
	window->Controls->Add(ipAddress);
	
    tableView = gcnew ConfigTableView();
    tableView->Name = "VDJscript";
	tableView->AutoGenerateColumns = false;
	tableView->RowHeadersVisible = true;
	tableView->AllowDrop = true;
	tableView->AllowUserToAddRows = false;
	tableView->ClipboardCopyMode = DataGridViewClipboardCopyMode::Disable;

    dataSource = gcnew ConfigDataSource(_vdjArtnet);
    tableView->DataSource = dataSource->DataSource;

    DataGridViewTextBoxColumn^ vdjScriptColumn = gcnew DataGridViewTextBoxColumn();
    vdjScriptColumn->Name = "VDJscript";
    vdjScriptColumn->DataPropertyName = "Value";
    vdjScriptColumn->ReadOnly = false;
    tableView->Columns->Add(vdjScriptColumn);
	tableView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

	window->Layout += gcnew LayoutEventHandler(this, &ConfigWindow::reLayout);
	//tableView->ConfigTableViewKeyDown += gcnew ConfigTableViewKeyEventHandler(this, &ConfigWindow::tableViewKeyDown);
	tableView->KeyDown += gcnew KeyEventHandler(this, &ConfigWindow::tableViewKeyDown);
	tableView->EditingControlShowing += gcnew DataGridViewEditingControlShowingEventHandler(this, &ConfigWindow::tableViewEditingControlShowing);
	tableView->MouseDown += gcnew MouseEventHandler(this, &ConfigWindow::tableViewMouseDown);
	tableView->MouseMove += gcnew MouseEventHandler(this, &ConfigWindow::tableViewMouseMove);
	tableView->DragEnter += gcnew DragEventHandler(this, &ConfigWindow::tableViewDragEnter);
	tableView->DragDrop += gcnew DragEventHandler(this, &ConfigWindow::tableViewDragDrop);
    window->Controls->Add(tableView);
	
    window->Show();
	
	presetWindow = gcnew Form();
    presetWindow->Name = "Presets";
    presetWindow->Text = "Presets";
	presetWindow->Size = System::Drawing::Size(200, 600);
	
    presetTableView = gcnew DataGridView();
    presetTableView->Name = "Presets";
	presetTableView->AutoGenerateColumns = false;
	presetTableView->RowHeadersVisible = false;
	presetTableView->AllowDrop = false;
	presetTableView->AllowUserToAddRows = false;
	presetTableView->ClipboardCopyMode = DataGridViewClipboardCopyMode::Disable;

	presetDataSource = gcnew ConfigPresetDataSource(_vdjArtnet);
    presetTableView->DataSource = presetDataSource->DataSource;

    DataGridViewTextBoxColumn^ presetColumn = gcnew DataGridViewTextBoxColumn();
    presetColumn->Name = "Presets";
    presetColumn->DataPropertyName = "Name";
    presetColumn->ReadOnly = true;
    presetTableView->Columns->Add(presetColumn);
	presetTableView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

	presetWindow->Layout += gcnew LayoutEventHandler(this, &ConfigWindow::reLayoutPreset);
	presetTableView->MouseDown += gcnew MouseEventHandler(this, &ConfigWindow::presetTableViewMouseDown);
	presetTableView->MouseMove += gcnew MouseEventHandler(this, &ConfigWindow::presetTableViewMouseMove);
    presetWindow->Controls->Add(presetTableView);
	
	presetWindow->Show();
	presetWindow->Location = System::Drawing::Point(window->Location.X + window->Width, window->Location.Y);

	window->FormClosed += gcnew FormClosedEventHandler(this, &ConfigWindow::didClose);
}

void ConfigWindow::reLayout(Object^ sender, LayoutEventArgs^ e) {
	ipLabel->Location = System::Drawing::Point(0, 0);
	
	ipAddress->Location = System::Drawing::Point(ipLabel->Width, 0);
	ipAddress->Size = System::Drawing::Size(window->ClientSize.Width - ipLabel->Width, 20);	

    tableView->Location = System::Drawing::Point(0, 20);
    tableView->Size = System::Drawing::Size(window->ClientSize.Width, window->ClientSize.Height - ipAddress->Height);	
	
    for each (DataGridViewRow^ row in tableView->Rows) {
        row->HeaderCell->Value = String::Format("{0}", row->Index + 1);
    }
	tableView->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);
}

void ConfigWindow::reLayoutPreset(Object^ sender, LayoutEventArgs^ e) {
	presetTableView->Location = System::Drawing::Point(0, 0);
    presetTableView->Size = presetWindow->ClientSize;	
}

void ConfigWindow::didClose(Object^ sender, FormClosedEventArgs^ e) {
	presetWindow->Hide();
	
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

void ConfigWindow::tableViewKeyDown(Object^ sender, KeyEventArgs^ e) {
//void ConfigWindow::tableViewKeyDown(Message% msg, Keys keyData) {
//	if (tableView->Focused) {
		//if ((int)(keyData & Keys::Control)) {
		//if (e->Control) {
			//switch (keyData & ~Keys::Control) {
			switch (e->KeyCode) {
			case Keys::D:
				if (tableView->SelectedRows->Count >= 1) {
					Clipboard::SetText(((ConfigRowString^)(tableView->SelectedRows[0]->DataBoundItem))->Value);
				}
				break;
			case Keys::F:
				if (tableView->SelectedRows->Count >= 1) {
					((ConfigRowString^)(tableView->SelectedRows[0]->DataBoundItem))->Value = Clipboard::GetText();
					tableView->Refresh();
				}
				break;
			case Keys::Z:
				if (!e->Shift) {
					break;
				}
			//case Keys::Z | Keys::Shift:
			case Keys::Y:
				break;
			}
		//}
//	}
}

void ConfigWindow::tableViewEditingControlShowing(Object^ sender, DataGridViewEditingControlShowingEventArgs^ e) {
    DataGridViewTextBoxEditingControl^ tb = dynamic_cast<DataGridViewTextBoxEditingControl^>(e->Control);
	if (tb != nullptr) {
        tb->KeyDown -= gcnew KeyEventHandler(this, &ConfigWindow::tableViewKeyDown);
        tb->KeyDown += gcnew KeyEventHandler(this, &ConfigWindow::tableViewKeyDown);
    }
}

void ConfigWindow::tableViewMouseDown(Object^ sender, MouseEventArgs^ e) {
    DataGridView::HitTestInfo^ hit = tableView->HitTest(e->X, e->Y);
    
	if (hit->Type == DataGridViewHitTestType::Cell) {
		rowToDrag = tableView->Rows[hit->RowIndex];
		
        System::Drawing::Size dragSize = SystemInformation::DragSize;
        dragBoxFromMouseDown = System::Drawing::Rectangle(System::Drawing::Point(e->X - (dragSize.Width / 2),e->Y - (dragSize.Height / 2)),dragSize);
	} else {
		rowToDrag = nullptr;
        dragBoxFromMouseDown = System::Drawing::Rectangle::Empty;
	}
}

void ConfigWindow::tableViewMouseMove(Object^ sender, MouseEventArgs^ e) {
    if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left ) {
		if ( dragBoxFromMouseDown != System::Drawing::Rectangle::Empty &&  !dragBoxFromMouseDown.Contains( e->X, e->Y ) ) {
			if (rowToDrag != nullptr) {
				tableView->DoDragDrop(((ConfigRowString^)(rowToDrag->DataBoundItem))->Value, DragDropEffects::Copy);
			}
		}
	}
}

void ConfigWindow::presetTableViewMouseDown(Object^ sender, MouseEventArgs^ e) {
    DataGridView::HitTestInfo^ hit = presetTableView->HitTest(e->X, e->Y);
    
	if (hit->Type == DataGridViewHitTestType::Cell) {
		rowToDrag = presetTableView->Rows[hit->RowIndex];
		
        System::Drawing::Size dragSize = SystemInformation::DragSize;
        dragBoxFromMouseDown = System::Drawing::Rectangle(System::Drawing::Point(e->X - (dragSize.Width / 2),e->Y - (dragSize.Height / 2)),dragSize);
	} else {
		rowToDrag = nullptr;
        dragBoxFromMouseDown = System::Drawing::Rectangle::Empty;
	}
}

void ConfigWindow::presetTableViewMouseMove(Object^ sender, MouseEventArgs^ e) {
    if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left ) {
		if ( dragBoxFromMouseDown != System::Drawing::Rectangle::Empty &&  !dragBoxFromMouseDown.Contains( e->X, e->Y ) ) {
			if (rowToDrag != nullptr) {
				presetTableView->DoDragDrop(((ConfigPresetRowString^)(rowToDrag->DataBoundItem))->Preset, DragDropEffects::Copy);
			}
		}
	}
}

void ConfigWindow::tableViewDragEnter(Object^ sender, DragEventArgs^ e) {
	if (e->Data->GetDataPresent(System::String::typeid)) {
		e->Effect = DragDropEffects::Copy;
	}
}

void ConfigWindow::tableViewDragDrop(Object^ sender, DragEventArgs^ e) {
	System::Drawing::Point coords = tableView->PointToClient(System::Drawing::Point(e->X, e->Y));
	DataGridView::HitTestInfo^ hit = tableView->HitTest(coords.X, coords.Y);
	if (hit->Type == DataGridViewHitTestType::Cell) {
		if (e->Data->GetDataPresent(System::String::typeid)) {
			if (e->Effect == DragDropEffects::Copy) {
				((ConfigRowString^)(tableView->Rows[hit->RowIndex]->DataBoundItem))->Value = dynamic_cast<String^>(e->Data->GetData(System::String::typeid));
				tableView->Refresh();
			}
		}
	}
}

void ConfigWindow::hide() {
	presetWindow->Hide();
	window->Hide();
	presetWindow = nullptr;
	window = nullptr;
}
