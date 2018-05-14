//
//  ConfigTool.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#include "ConfigNativeWin.hpp"

ConfigNativeWin::ConfigNativeWin(Config* configTMP) {
    config = configTMP;
}

void ConfigNativeWin::presentConfigTool() {
    configTool = gcnew ConfigWinTool(config);
}

void ConfigNativeWin::presentText(std::string message, std::string information) {
    System::Windows::Forms::MessageBox::Show(gcnew String(information.c_str()), gcnew String(message.c_str()));
}

int ConfigNativeWin::presentDialog(std::string message, std::string information, std::vector<std::string> responses, std::function<int()> callback) {
    return -2;
}
