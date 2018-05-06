//
//  ConfigWinTableView.hpp
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

#ifndef ConfigWinTableView_hpp
#define ConfigWinTableView_hpp

#include <stdio.h>

#include "VDJartnet.hpp"

#include "ConfigWinDataSource.hpp"
#include "CppStep/CSUndoManager.hpp"

#include "windows.h" 

#using <mscorlib.dll> 
#using <System.dll> 
#using <System.Windows.Forms.dll> 
#include <msclr\marshal_cppstd.h>

using namespace System::Windows::Forms;
using namespace System;

delegate void ConfigWinTableViewKeyEventHandler(Message% msg, Keys keyData);  

interface struct IConfigWinTableView {  
public:  
    event ConfigWinTableViewKeyEventHandler^ ConfigTableViewKeyDown;
};

/** A list of commands */
ref class ConfigWinTableView : public DataGridView, public IConfigWinTableView {
public:
    virtual event ConfigWinTableViewKeyEventHandler^ ConfigTableViewKeyDown; /**< A key has been pressed */

    /** A process a key press */
    virtual bool ProcessCmdKey(Message% msg, Keys keyData) override {
        ConfigTableViewKeyDown(msg, keyData);
        return DataGridView::ProcessCmdKey(msg, keyData);
    }

    ConfigWinTableView(CVDJartnet* vdjArtnet); /**< Construct a list of commands with the given instance of the plugin */

    void tableViewKeyDown(Object^ sender, KeyEventArgs^ e); /**< A key has been pressed */
    void tableViewEditingControlShowing(Object^ sender, DataGridViewEditingControlShowingEventArgs^ e); /**< A cell is about to be shown */
    void tableViewMouseDown(Object^ sender, MouseEventArgs^ e); /**< The mouse has been clicked */
    void tableViewMouseMove(Object^ sender, MouseEventArgs^ e); /**< The mouse has been moved */
    void tableViewDragEnter(Object^ sender, DragEventArgs^ e); /**< A dragged object has entered the list */
    void tableViewDragDrop(Object^ sender, DragEventArgs^ e); /**< A dragged object has been dropped in the list */

private:
    DataGridViewRow ^ rowToDrag; /**< The row to drag */
    System::Drawing::Rectangle dragBoxFromMouseDown; /**< A box around the initial click point */

    CSUndoManager* undoManager; /**< A pointer to the undo manager */
};

#endif /* ConfigWinTableView_hpp */
