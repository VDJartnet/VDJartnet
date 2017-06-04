//
//  ConfigWin.hpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 03/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigWin_hpp
#define ConfigWin_hpp

#include <stdio.h>
#include "ConfigWinDataSource.hpp"

using namespace System;
using namespace System.Windows.Forms;

ref class ConfigWindow : public Object {
    
public:
    ConfigWindow(CVDJartnet vdjArtnet);

protected:
    Form^ window;
    DataGridView^ tableView;
    ConfigDataSource^ dataSource;
}

#endif /* ConfigWin_hpp */
