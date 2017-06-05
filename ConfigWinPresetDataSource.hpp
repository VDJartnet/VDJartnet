//
//  ConfigWinPresetDataSource.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

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

        if (_vdjArtnet->presetFin->is_open()) {
            _vdjArtnet->presetFin->clear();
            _vdjArtnet->presetFin->seekg(0);
            std::string line;
            safeGetline(*(_vdjArtnet->presetFin), line);
            while (line != "") {
                int posOfDelim = (int)line.find('~'); //Convert unsigned long to int explicitly to stop compiler complaining
                String^ name = gcnew String(line.substr(0, posOfDelim).c_str());
                String^ preset = gcnew String(line.substr(posOfDelim + 1, std::string::npos).c_str());

				DataSource->Add(gcnew ConfigPresetRowString(name, preset));
                safeGetline(*(_vdjArtnet->presetFin), line);
            }
            _vdjArtnet->presetFin->clear();
            _vdjArtnet->presetFin->seekg(0);
        }
    }
};

#endif /* ConfigWinPresetDataSource_hpp */
