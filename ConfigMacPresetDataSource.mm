//
//  ConfigMacPresetDataSource.m
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

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
