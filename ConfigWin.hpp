//
//  ConfigWin.hpp
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

#ifndef ConfigWin_hpp
#define ConfigWin_hpp

#ifdef CLRFREE
extern void* createConfigWindow(CVDJartnet* vdjArtnet);
extern void closeConfigWindow(void* configWindow);
#else

#include <stdio.h>

#include "VDJartnet.hpp"

#include "ConfigWinDataSource.hpp"
#include "ConfigWinPresetDataSource.hpp"
#include "ConfigWinTableView.hpp"

#include "windows.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include <msclr\gcroot.h>

using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System;

ref class ConfigWindow {

public:
    CVDJartnet* _vdjArtnet;

    ConfigWindow(CVDJartnet* vdjArtnetTMP);
    void reLayout(Object^ sender, LayoutEventArgs^ e);
    void reLayoutPreset(Object^ sender, LayoutEventArgs^ e);
    void didClose(Object^ sender, FormClosedEventArgs^ e);
    void updateIPaddress(Object^ sender, EventArgs^ e);
    void ipKeyDown(Object^ sender, KeyEventArgs^ e);
    void tableViewKeyDown(Object^ sender, KeyEventArgs^ e);
//    void tableViewKeyDown(Message% msg, Keys keyData);
    void tableViewEditingControlShowing(Object^ sender, DataGridViewEditingControlShowingEventArgs^ e);
    void tableViewMouseDown(Object^ sender, MouseEventArgs^ e);
    void tableViewMouseMove(Object^ sender, MouseEventArgs^ e);
    void presetTableViewMouseDown(Object^ sender, MouseEventArgs^ e);
    void presetTableViewMouseMove(Object^ sender, MouseEventArgs^ e);
    void tableViewDragEnter(Object^ sender, DragEventArgs^ e);
    void tableViewDragDrop(Object^ sender, DragEventArgs^ e);
    void hide();
protected:
    Form^ window;
    Label^ ipLabel;
    TextBox^ ipAddress;
    DataGridView^ tableView;
    ConfigDataSource^ dataSource;

    Form^ presetWindow;
    DataGridView^ presetTableView;
    ConfigPresetDataSource^ presetDataSource;

private:
    DataGridViewRow^ rowToDrag;
    System::Drawing::Rectangle dragBoxFromMouseDown;

};

class ConfigWindowNative {
public:
    msclr::gcroot<ConfigWindow^> configWindow;

    ConfigWindowNative(CVDJartnet* vdjArtnet) : configWindow(gcnew ConfigWindow(vdjArtnet)) {}
};

void* createConfigWindow(CVDJartnet* vdjArtnet) {
    return new ConfigWindowNative(vdjArtnet);
}

void closeConfigWindow(void* configWindow) {
    ((ConfigWindowNative*)configWindow)->configWindow->hide();
}

#endif // CLRFREE
#endif /* ConfigWin_hpp */
