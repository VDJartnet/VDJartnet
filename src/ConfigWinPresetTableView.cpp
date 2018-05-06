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

#include "ConfigWinPresetTableView.hpp"

ConfigWinPresetTableView::ConfigWinPresetTableView(CVDJartnet* vdjArtnet) {
    this->Name = "Presets";
    this->AutoGenerateColumns = false;
    this->RowHeadersVisible = false;
    this->AllowDrop = false;
    this->AllowUserToAddRows = false;
    this->ClipboardCopyMode = DataGridViewClipboardCopyMode::EnableWithoutHeaderText;
    this->SelectionMode = DataGridViewSelectionMode::FullRowSelect;

    ConfigWinPresetDataSource^ dataSource = gcnew ConfigWinPresetDataSource(512);
    for (size_t i = 0; i < vdjArtnet->config->getPresets().size(); i++) {
        dataSource->Add(gcnew ConfigWinPresetRowString(vdjArtnet->config->getPresets()[i]));
    }
    this->DataSource = dataSource;

    DataGridViewTextBoxColumn^ presetColumn = gcnew DataGridViewTextBoxColumn();
    presetColumn->Name = "Presets";
    presetColumn->DataPropertyName = "Name";
    presetColumn->ReadOnly = true;
    this->Columns->Add(presetColumn);
    this->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

    this->MouseDown += gcnew MouseEventHandler(this, &ConfigWinPresetTableView::tableViewMouseDown);
    this->MouseMove += gcnew MouseEventHandler(this, &ConfigWinPresetTableView::tableViewMouseMove);
}

DataObject^ ConfigWinPresetTableView::GetClipboardContent() {
    if (this->SelectedRows->Count >= 1) {
        return gcnew DataObject(((ConfigWinPresetRowString^)(this->SelectedRows[0]->DataBoundItem))->Preset);
    } else {
        return nullptr;
    }
}

void ConfigWinPresetTableView::tableViewMouseDown(Object^ sender, MouseEventArgs^ e) {
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

void ConfigWinPresetTableView::tableViewMouseMove(Object^ sender, MouseEventArgs^ e) {
    if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
        if (dragBoxFromMouseDown != System::Drawing::Rectangle::Empty && !dragBoxFromMouseDown.Contains(e->X, e->Y)) {
            if (rowToDrag != nullptr) {
                this->DoDragDrop(((ConfigWinPresetRowString^)(rowToDrag->DataBoundItem))->Preset, DragDropEffects::Copy);
            }
        }
    }
}
