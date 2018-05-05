//
//  ConfigWinTableView.cpp
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

#include "ConfigWinTableView.hpp"

ConfigTableView::ConfigTableView(CVDJartnet* vdjArtnet) {
	this->Name = "VDJscript";
	this->AutoGenerateColumns = false;
	this->RowHeadersVisible = true;
	this->AllowDrop = true;
	this->AllowUserToAddRows = false;
	this->ClipboardCopyMode = DataGridViewClipboardCopyMode::EnableWithoutHeaderText;
	this->SelectionMode = DataGridViewSelectionMode::FullRowSelect;

	undoManager = new CSUndoManager();

	ConfigDataSource^ dataSource = gcnew ConfigDataSource(512);
	for (int row = 0; row < 512; row++) {
		dataSource->Add(gcnew ConfigRowString(vdjArtnet, row, undoManager));
	}
	this->DataSource = dataSource;

	DataGridViewTextBoxColumn^ vdjScriptColumn = gcnew DataGridViewTextBoxColumn();
	vdjScriptColumn->Name = "VDJscript";
	vdjScriptColumn->DataPropertyName = "Value";
	vdjScriptColumn->ReadOnly = false;
	this->Columns->Add(vdjScriptColumn);
	this->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

	this->KeyDown += gcnew KeyEventHandler(this, &ConfigTableView::tableViewKeyDown);
	this->EditingControlShowing += gcnew DataGridViewEditingControlShowingEventHandler(this, &ConfigTableView::tableViewEditingControlShowing);
	this->MouseDown += gcnew MouseEventHandler(this, &ConfigTableView::tableViewMouseDown);
	this->MouseMove += gcnew MouseEventHandler(this, &ConfigTableView::tableViewMouseMove);
	this->DragEnter += gcnew DragEventHandler(this, &ConfigTableView::tableViewDragEnter);
	this->DragDrop += gcnew DragEventHandler(this, &ConfigTableView::tableViewDragDrop);
}

void ConfigTableView::tableViewKeyDown(Object^ sender, KeyEventArgs^ e) {
	switch (e->KeyCode) {
	case Keys::V:
		if (this->SelectedRows->Count >= 1) {
			((ConfigRowString^)(this->SelectedRows[0]->DataBoundItem))->Value = Clipboard::GetText();
			this->Refresh();
		}
		break;
	case Keys::Delete:
	case Keys::Back:
		if (this->SelectedRows->Count >= 1) {
			((ConfigRowString^)(this->SelectedRows[0]->DataBoundItem))->Value = "";
			this->Refresh();
		}
		break;
	case Keys::Z:
		if (!e->Shift) {
			this->undoManager->undo();
			this->Refresh();
			break;
		} // else fallthrough
	case Keys::Y:
		this->undoManager->redo();
		this->Refresh();
		break;
	}
}

void ConfigTableView::tableViewEditingControlShowing(Object^ sender, DataGridViewEditingControlShowingEventArgs^ e) {
	DataGridViewTextBoxEditingControl^ tb = dynamic_cast<DataGridViewTextBoxEditingControl^>(e->Control);
	if (tb != nullptr) {
		tb->KeyDown -= gcnew KeyEventHandler(this, &ConfigTableView::tableViewKeyDown);
		tb->KeyDown += gcnew KeyEventHandler(this, &ConfigTableView::tableViewKeyDown);
	}
}

void ConfigTableView::tableViewMouseDown(Object^ sender, MouseEventArgs^ e) {
	DataGridView::HitTestInfo^ hit = this->HitTest(e->X, e->Y);

	if (hit->Type == DataGridViewHitTestType::Cell) {
		rowToDrag = this->Rows[hit->RowIndex];

		System::Drawing::Size dragSize = SystemInformation::DragSize;
		dragBoxFromMouseDown = System::Drawing::Rectangle(System::Drawing::Point(e->X - (dragSize.Width / 2), e->Y - (dragSize.Height / 2)), dragSize);
	}
	else {
		rowToDrag = nullptr;
		dragBoxFromMouseDown = System::Drawing::Rectangle::Empty;
	}
}

void ConfigTableView::tableViewMouseMove(Object^ sender, MouseEventArgs^ e) {
	if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
		if (dragBoxFromMouseDown != System::Drawing::Rectangle::Empty && !dragBoxFromMouseDown.Contains(e->X, e->Y)) {
			if (rowToDrag != nullptr) {
				this->DoDragDrop(((ConfigRowString^)(rowToDrag->DataBoundItem))->Value, DragDropEffects::Copy);
			}
		}
	}
}

void ConfigTableView::tableViewDragEnter(Object^ sender, DragEventArgs^ e) {
	if (e->Data->GetDataPresent(System::String::typeid)) {
		e->Effect = DragDropEffects::Copy;
	}
}

void ConfigTableView::tableViewDragDrop(Object^ sender, DragEventArgs^ e) {
	System::Drawing::Point coords = this->PointToClient(System::Drawing::Point(e->X, e->Y));
	DataGridView::HitTestInfo^ hit = this->HitTest(coords.X, coords.Y);
	if (hit->Type == DataGridViewHitTestType::Cell) {
		if (e->Data->GetDataPresent(System::String::typeid)) {
			if (e->Effect == DragDropEffects::Copy) {
				((ConfigRowString^)(this->Rows[hit->RowIndex]->DataBoundItem))->Value = dynamic_cast<String^>(e->Data->GetData(System::String::typeid));
				this->Refresh();
			}
		}
	}
}

