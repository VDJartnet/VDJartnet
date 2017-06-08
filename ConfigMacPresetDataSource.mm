//
//  ConfigMacPresetDataSource.m
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
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
//{Corresponding Source for a non-source form of such a combination shall not
//include the source code for the parts of VirtualDJ used as well as that of the covered work.}

#import "ConfigMacPresetDataSource.h"

@implementation ConfigPreset

- (id) initWithName:(NSString*)name preset:(NSString*)preset {
    if ( self = [super init] ) {
        _name = name;
        _preset = preset;
        return self;
    } else {
        return nil;
    }
}

@end

@implementation ConfigPresetDataSource

- (id) initWithVDJartnet:(CVDJartnet*)vdjArtnet {
    if ( self = [super init] ) {
        _vdjArtnet = vdjArtnet;
        _presets = [NSMutableArray<ConfigPreset*> array];

        if (_vdjArtnet->presetFin->is_open()) {
            _vdjArtnet->presetFin->clear();
            _vdjArtnet->presetFin->seekg(0);
            std::string line;
            safeGetline(*(_vdjArtnet->presetFin), line);
            while (line != "") {
                int posOfDelim = (int)line.find('~'); //Convert unsigned long to int explicitly to stop compiler complaining
                NSString* name = [NSString stringWithCString:line.substr(0, posOfDelim).c_str() encoding:[NSString defaultCStringEncoding]];
                NSString* preset = [NSString stringWithCString:line.substr(posOfDelim + 1, std::string::npos).c_str() encoding:[NSString defaultCStringEncoding]];

                [_presets addObject:[[ConfigPreset alloc] initWithName:name preset:preset]];
                safeGetline(*(_vdjArtnet->presetFin), line);
            }
            _vdjArtnet->presetFin->clear();
            _vdjArtnet->presetFin->seekg(0);
        }
        return self;
    } else {
        return nil;
    }
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    return [_presets count];
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    return [_presets[row] name];
}

- (BOOL)tableView:(NSTableView *)tableView writeRowsWithIndexes:(NSIndexSet *)rowIndexes toPasteboard:(NSPasteboard *)pboard {
    [pboard declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [pboard setString:[_presets[[rowIndexes firstIndex]] preset] forType:NSStringPboardType];
    return YES;
}

@end
