//
//  ConfigWinTableView.hpp
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
//{Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of VirtualDJ used as well as that of the covered work.}
//
//If you modify this Program, or any covered work, by linking or
//combining it with the Visual C++ Runtime, the licensors of this Program grant you
//additional permission to convey the resulting work.
//{Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of the Visual C++ Runtime used as well as that of the covered work.}

#ifndef ConfigWinTableView_hpp
#define ConfigWinTableView_hpp

#include <stdio.h>

#include "windows.h" 

#using <mscorlib.dll> 
#using <System.dll> 
#using <System.Windows.Forms.dll> 
#include <msclr\marshal_cppstd.h>

using namespace System::Windows::Forms;
using namespace System;

delegate void ConfigTableViewKeyEventHandler(Message% msg, Keys keyData);  

interface struct IConfigTableView {  
public:  
	event ConfigTableViewKeyEventHandler^ ConfigTableViewKeyDown;
};

ref class ConfigTableView : public DataGridView, public IConfigTableView {
public:
	virtual event ConfigTableViewKeyEventHandler^ ConfigTableViewKeyDown;

	virtual bool ProcessCmdKey(Message% msg, Keys keyData) override {
		//if (ConfigTableViewKeyDown != nullptr) {
			ConfigTableViewKeyDown(msg, keyData);
		//}
		return DataGridView::ProcessCmdKey(msg, keyData);
    }	
};

#endif /* ConfigWinTableView_hpp */
