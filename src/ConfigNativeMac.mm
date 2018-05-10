//
//  ConfigNativeMac.mm
//  VDJartnet
//
//  Created by Jonathan Tanner on 10/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#import "ConfigNativeMac.h"

ConfigNativeMac::ConfigNativeMac(Config* configTMP) {
    config = configTMP;
}

ConfigNativeMac::~ConfigNativeMac() {}

void ConfigNativeMac::presentConfigTool() {
    configMacTool = [[ConfigMacTool alloc] initWithConfig:config Destructor:[]() {}];
}

void ConfigNativeMac::presentText(std::string message, std::string information) {
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@(message.c_str())];
    [alert setInformativeText:@(information.c_str())];
    [alert setAlertStyle:NSAlertStyleInformational];
    [alert runModal];
}

int ConfigNativeMac::presentDialog(std::string message, std::string information, std::vector<std::string> responses, std::function<int()> callback) {
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@(message.c_str())];
    [alert setInformativeText:@(information.c_str())];
    [alert setAlertStyle:NSAlertStyleInformational];
    for (std::string response : responses) {
        [alert addButtonWithTitle:@(response.c_str())];
    }
    switch ([alert runModal]) {
        case NSAlertFirstButtonReturn:
            return 0;
        case NSAlertSecondButtonReturn:
            return 1;
        case NSAlertThirdButtonReturn:
            return 2;
        default:
            return -1;
    }
}
