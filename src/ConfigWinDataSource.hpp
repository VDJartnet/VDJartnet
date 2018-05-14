//
//  ConfigWinDataSource.hpp
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

/** @file ConfigWinDataSource.hpp */

#ifndef ConfigWinDataSource_hpp
#define ConfigWinDataSource_hpp

#include <stdio.h>

#define NODLLEXPORT
#include "Config.hpp"

#include "CppStep/CSUndoManager.hpp"

#include "windows.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#include <msclr\marshal_cppstd.h>

using namespace System::Windows::Forms;
using namespace System;

/** A row in the list of commands in the config tool */
ref class ConfigWinRowString : public Object {
public:
    /** The value of the command. */
    property String^ Value {
        String^ get(); /**< Get the value of the command */
        void set(String^ newVal); /**< Set the value of the command */
    }

    ConfigWinRowString(Config* configTMP, int rowTMP, CSUndoManager* undoManagerTMP); /**< Construct a row with the given instance of the config, row number and undo manager */
private:
    Config* config; /**< A pointer to the plugin */
    int row; /**< The number of this row */
    CSUndoManager* undoManager; /**< A pointer to the undo manager */
};

typedef System::Collections::Generic::List<ConfigWinRowString^> ConfigWinDataSource; /**< The data source which shall provide the config rows. It is a list of the ConfigWinRowStrings. */

void setValue(gcroot<ConfigWinRowString^ const> const target, gcroot<String^ const > const newVal); /**< Set the value of a given row to a certain value in a native compatible way */

#endif /* ConfigWinDataSource_hpp */
