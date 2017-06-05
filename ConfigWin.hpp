//
//  ConfigWin.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifdef CLRFREE
extern void* createConfigWindow(CVDJartnet* vdjArtnet);
#else
#ifndef ConfigWin_hpp
#define ConfigWin_hpp

#include <stdio.h>

#include "windows.h" 

#using <mscorlib.dll> 
#using <System.dll> 
#using <System.Windows.Forms.dll> 
#using <System.Drawing.dll> 

#include <msclr\gcroot.h>

#include "ConfigWinDataSource.hpp"
#include "ConfigWinPresetDataSource.hpp"

using namespace System::Windows::Forms;
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

#endif /* ConfigWin_hpp */
#endif // CLRFREE
