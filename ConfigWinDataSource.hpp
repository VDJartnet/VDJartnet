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
#include "VDJartnet.hpp"

#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System.Windows.Forms;

ref class ConfigRowString : public Object {
    CVDJartnet* _vdjArtnet;
    int row;

    property String Value {
        String get() {
            return String(_vdjArtnet->channelCommands[row].c_str());
        }
        void set(String^ newVal) {
            _vdjArtnet->channelCommands[row] = msclr::interop::marshal_as<std::string>(newVal);
            _vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
        }
    }

    CStringRef(CVDJartnet* vdjArtnetTMP, int rowTMP) {
        _vdjArtnet = vdjArtnetTMP;
        row = rowTMP
    }
};

ref class ConfigDataSource {

public:
    CVDJartnet* _vdjArtnet;
    List<ConfigRowString>^ DataSource;

    ConfigDataSource(CVDJartnet* vdjArtnetTMP) {
        _vdjArtnet = vdjArtnetTMP;

        DataSource = gcnew List<ConfigRowString>(512);

        for (int row = 0; row < 512; row++) {
            DataSource->Add(gcnew ConfigRowString(_vdjArtnet, row));
        }
    }

protected:
}

#endif /* ConfigWinDataSource_hpp */
