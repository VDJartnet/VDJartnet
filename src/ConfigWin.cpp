//
//  ConfigWin.cpp
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

#include "ConfigWin.hpp"

ConfigWinTool::ConfigWinTool(CVDJartnet* vdjArtnetTMP) {
    vdjArtnet = vdjArtnetTMP;

    window = gcnew ConfigWinWindow(vdjArtnet);
    presetWindow = gcnew ConfigWinPresetWindow(vdjArtnet);
    presetWindow->Location = System::Drawing::Point(window->Location.X + window->Width, window->Location.Y);
}


void ConfigWinTool::hide() {
    presetWindow->Hide();
    window->Hide();
    presetWindow = nullptr;
    window = nullptr;
}

void* createConfigWinTool(CVDJartnet* vdjArtnet) {
    return new ConfigWinToolNative(vdjArtnet);
}

void closeConfigWinTool(void* configTool) {
    ((ConfigWinToolNative*)configTool)->configTool->hide();
}