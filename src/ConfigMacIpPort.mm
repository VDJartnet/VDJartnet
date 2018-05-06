//
//  ConfigMacIpPort.mm
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

#import "ConfigMacIpPort.h"

@implementation ConfigMacIpPort {
    CVDJartnet* vdjArtnet; /**< A pointer to the plugin */
}

- (id) initWithFrame:(CGRect)frame VDJartnet:(CVDJartnet*)vdjArtnetTMP {
    if ( self = [super initWithFrame:frame] ) {
        vdjArtnet = vdjArtnetTMP;
        
        [self setStringValue: [@(vdjArtnet->config->port) stringValue]];
        [self setEditable:YES];
        [self setDelegate:self];

        return self;
    } else {
        return nil;
    }
}

- (BOOL)control:(NSControl*)control textShouldEndEditing:(NSText*)fieldEditor {
    unsigned short value = [[NSNumber numberWithInteger:[[fieldEditor string] integerValue]] unsignedShortValue];
    if (value > 0) {
        vdjArtnet->config->port = value;
        vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
                
        return YES;
    } else {
        return NO;
    }
}

@end
