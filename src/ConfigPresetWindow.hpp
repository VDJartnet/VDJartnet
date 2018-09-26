//
//  ConfigPresetWindow.hpp
//  VDJartnet
//
//  Copyright � 2017-18 Jonathan Tanner. All rights reserved.
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

#ifndef ConfigPresetWindow_hpp
#define ConfigPresetWindow_hpp

#include "Config.hpp"

#include "ConfigPresetTableViewDataSource.hpp"

#ifndef CLRFREE
#include "CppStep/src/CSWindow.hpp"
#include "CppStep/src/CSLabel.hpp"
#include "CppStep/src/CSTextField.hpp"
#include "CppStep/src/CSTableView.hpp"
#include "CppStep/src/CSAlignView.hpp"
#include "CppStep/src/CSMenu.hpp"
#include "CppStep/src/CSClipboard.hpp"
#else
class CSWindow;
#endif

/** A window containing a list of commands */
class ConfigPresetWindow {
public:
    ConfigPresetWindow(Config* config); /**< Construct a window with a list of commands with the given instance of the config */
    void show(); /**< Show the window */
    void hide(); /**< Hide the window */
    void didClose(); /**< The window has been closed */
    void copyRow();
private:
#ifndef CLRFREE
    Config* config; /**< A pointer to the config */
    CSWindow* window; /**< The window */
    CSTableView* tableView; /**< The list of presets */
#endif
};

#endif /* ConfigPresetWindow_hpp */
