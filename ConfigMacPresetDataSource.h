//
//  ConfigMacPresetDataSource.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigMacPresetDataSource_h
#define ConfigMacPresetDataSource_h

#include <AppKit/AppKit.h>
#import "ConfigMacTableView.h"
#ifdef __cplusplus
#include "VDJartnet.hpp"
#endif

@interface ConfigPreset : NSObject

@property (strong) NSString* name;
@property (strong) NSString* preset;

- (id) initWithName:(NSString*)name preset:(NSString*)preset;
@end

@interface ConfigPresetDataSource : NSObject <NSTableViewDataSource, NSTableViewDelegate>

@property CVDJartnet* vdjArtnet;
@property (strong) NSMutableArray<ConfigPreset*>* presets;

- (id) initWithVDJartnet:(CVDJartnet*)vdjArtnet;
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;

- (BOOL)tableView:(NSTableView *)tableView writeRowsWithIndexes:(NSIndexSet *)rowIndexes toPasteboard:(NSPasteboard *)pboard;
@end

#endif /* ConfigMacPresetDataSource_h */
