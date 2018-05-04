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

#include "ConfigWinPresetWindow.hpp"

ConfigPresetWindow::ConfigPresetWindow(CVDJartnet* vdjArtnetTMP) {
	vdjArtnet = vdjArtnetTMP;

	this->Name = "Presets";
	this->Text = "Presets";
	this->Size = System::Drawing::Size(200, 600);

	tableView = gcnew DataGridView();
	tableView->Name = "Presets";
	tableView->AutoGenerateColumns = false;
	tableView->RowHeadersVisible = false;
	tableView->AllowDrop = false;
	tableView->AllowUserToAddRows = false;
	tableView->ClipboardCopyMode = DataGridViewClipboardCopyMode::Disable;

	ConfigPresetDataSource^ dataSource = gcnew ConfigPresetDataSource(512);
	for (int i = 0; i < vdjArtnet->config->getPresets().size(); i++) {
		dataSource->Add(gcnew ConfigPresetRowString(vdjArtnet->config->getPresets()[i]));
	}
	tableView->DataSource = dataSource;

	DataGridViewTextBoxColumn^ presetColumn = gcnew DataGridViewTextBoxColumn();
	presetColumn->Name = "Presets";
	presetColumn->DataPropertyName = "Name";
	presetColumn->ReadOnly = true;
	tableView->Columns->Add(presetColumn);
	tableView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

	this->Layout += gcnew LayoutEventHandler(this, &ConfigPresetWindow::reLayout);
	tableView->MouseDown += gcnew MouseEventHandler(this, &ConfigPresetWindow::tableViewMouseDown);
	tableView->MouseMove += gcnew MouseEventHandler(this, &ConfigPresetWindow::tableViewMouseMove);
	this->Controls->Add(tableView);

	this->Show();
}

void ConfigPresetWindow::reLayout(Object^ sender, LayoutEventArgs^ e) {
	tableView->Location = System::Drawing::Point(0, 0);
	tableView->Size = this->ClientSize;
}

void ConfigPresetWindow::tableViewMouseDown(Object^ sender, MouseEventArgs^ e) {
	DataGridView::HitTestInfo^ hit = tableView->HitTest(e->X, e->Y);

	if (hit->Type == DataGridViewHitTestType::Cell) {
		rowToDrag = tableView->Rows[hit->RowIndex];

		System::Drawing::Size dragSize = SystemInformation::DragSize;
		dragBoxFromMouseDown = System::Drawing::Rectangle(System::Drawing::Point(e->X - (dragSize.Width / 2), e->Y - (dragSize.Height / 2)), dragSize);
	}
	else {
		rowToDrag = nullptr;
		dragBoxFromMouseDown = System::Drawing::Rectangle::Empty;
	}
}

void ConfigPresetWindow::tableViewMouseMove(Object^ sender, MouseEventArgs^ e) {
	if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
		if (dragBoxFromMouseDown != System::Drawing::Rectangle::Empty && !dragBoxFromMouseDown.Contains(e->X, e->Y)) {
			if (rowToDrag != nullptr) {
				tableView->DoDragDrop(((ConfigPresetRowString^)(rowToDrag->DataBoundItem))->Preset, DragDropEffects::Copy);
			}
		}
	}
}
