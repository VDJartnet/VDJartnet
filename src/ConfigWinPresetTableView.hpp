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

#ifndef ConfigWinPresetTableView_hpp
#define ConfigWinPresetTableView_hpp

#include <stdio.h>

#include "VDJartnet.hpp"

#include "ConfigWinPresetDataSource.hpp"

#include "windows.h" 

#using <mscorlib.dll> 
#using <System.dll> 
#using <System.Windows.Forms.dll> 
#include <msclr\marshal_cppstd.h>

using namespace System::Windows::Forms;
using namespace System;

/** A list of presets */
ref class ConfigWinPresetTableView : public DataGridView {
public:
    ConfigWinPresetTableView(CVDJartnet* vdjArtnet); /**< Construct a list of presets with the given instance of the plugin */

    DataObject^ GetClipboardContent() override; /**< Get the command to be copied */

    void tableViewMouseDown(Object^ sender, MouseEventArgs^ e); /**< The mouse has been clicked */
    void tableViewMouseMove(Object^ sender, MouseEventArgs^ e); /**< The mouse has been moved */
private:
    ConfigWinPresetDataSource ^ dataSource; /**< The data source for the list */

    DataGridViewRow ^ rowToDrag; /**< The row to drag */
    System::Drawing::Rectangle dragBoxFromMouseDown; /**< A box around the initial click point */
};

#endif /* ConfigWinPresetTableView_hpp */
