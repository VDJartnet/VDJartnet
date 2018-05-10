//
//  ConfigMacPresetDataSource.mm
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

#import "ConfigMacPresetDataSource.h"

@implementation ConfigMacPresetDataSource {
    Config* config; /**< A pointer to the plugin */
}

- (id)initWithConfig:(Config*)configTMP {
    if ( self = [super init] ) {
        config = configTMP;
        return self;
    } else {
        return nil;
    }
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView {
    return (NSInteger)config->getPresets().size();
}

- (id)tableView:(NSTableView*)tableView objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
    return @(config->getPresets()[(unsigned long)row].name.c_str());
}

- (BOOL)tableView:(NSTableView*)tableView writeRowsWithIndexes:(NSIndexSet*)rowIndexes toPasteboard:(NSPasteboard*)pboard {
    [pboard declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [pboard setString:@(config->getPresets()[[rowIndexes firstIndex]].preset.c_str()) forType:NSStringPboardType];
    return YES;
}

@end
