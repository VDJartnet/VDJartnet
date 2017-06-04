//
//  ConfigWinDataSource.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigWinDataSource_hpp
#define ConfigWinDataSource_hpp

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

ref class ConfigRowString : public Object {
public:
    CVDJartnet* _vdjArtnet;
    int row;

    property String^ Value {
        String^ get() {
            return gcnew String(_vdjArtnet->channelCommands[row].c_str());
        }
        void set(String^ newVal) {
            _vdjArtnet->channelCommands[row] = msclr::interop::marshal_as<std::string>(newVal);
            _vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
        }
    }

    ConfigRowString(CVDJartnet* vdjArtnetTMP, int rowTMP) {
        _vdjArtnet = vdjArtnetTMP;
        row = rowTMP;
    }
};

ref class ConfigDataSource {
public:
    CVDJartnet* _vdjArtnet;
    System::Collections::Generic::List<ConfigRowString^>^ DataSource;

    ConfigDataSource(CVDJartnet* vdjArtnetTMP) {
        _vdjArtnet = vdjArtnetTMP;

        DataSource = gcnew System::Collections::Generic::List<ConfigRowString^>(512);

        for (int row = 0; row < 512; row++) {
            DataSource->Add(gcnew ConfigRowString(_vdjArtnet, row));
        }
    }
};

#endif /* ConfigWinDataSource_hpp */
