//
//  ConfigWinPresetDataSource.hpp
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

#ifndef ConfigWinPresetDataSource_hpp
#define ConfigWinPresetDataSource_hpp

#include <stdio.h>

#define NODLLEXPORT
#include "VDJartnet.hpp"

#include "windows.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#include <msclr\marshal_cppstd.h>

using namespace System::Windows::Forms;
using namespace System;

ref class ConfigPresetRowString : public Object {
public:
    property String^ Name {
        String^ get() {
            return _name;
        }
        void set(String^ newVal) {
            _name = newVal;
        }
    }

    property String^ Preset {
        String^ get() {
            return _preset;
        }
        void set(String^ newVal) {
            _preset = newVal;
        }
    }

    ConfigPresetRowString(String^ nameTMP, String^ presetTMP) {
        _name = nameTMP;
        _preset = presetTMP;
    }

    ConfigPresetRowString(const struct _Preset preset) {
        _name = gcnew String(preset.name.c_str());
        _preset = gcnew String(preset.preset.c_str());
    }

private:
    String^ _name;
    String^ _preset;
};

ref class ConfigPresetDataSource {
public:
    CVDJartnet* _vdjArtnet;
    System::Collections::Generic::List<ConfigPresetRowString^>^ DataSource;

    ConfigPresetDataSource(CVDJartnet* vdjArtnetTMP) {
        _vdjArtnet = vdjArtnetTMP;

        DataSource = gcnew System::Collections::Generic::List<ConfigPresetRowString^>(512);

        for (int i = 0; i < _vdjArtnet->config->getPresets().size(); i++) {
            DataSource->Add(gcnew ConfigPresetRowString(_vdjArtnet->config->getPresets()[i]));
        }
    }
};

#endif /* ConfigWinPresetDataSource_hpp */
