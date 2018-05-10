//
//  ConfigMacTableView.mm
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

#import "ConfigMacTableView.h"

@implementation ConfigMacTableView {
    Config* config; /**< A pointer to the config */
}

- (id)initWithConfig:(Config*)configTMP {
    if ( self = [super init] ) {
        config = configTMP;
        
        [self addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"Channel"]];
        [self addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"VDJscript"]];
        [[self tableColumns][0] setTitle:NSLocalizedString(@"Channel", @"DMX Channel")];
        [[self tableColumns][0] setEditable:NO];
        [[self tableColumns][1] setTitle:NSLocalizedString(@"VDJscript", @"")];
        [self setColumnAutoresizingStyle:NSTableViewSequentialColumnAutoresizingStyle];
        [[self tableColumns][0] setWidth:50];
        [self setDataSource:self];
        [self setDelegate:self];
        [self reloadData];
        [self setDraggingSourceOperationMask:NSDragOperationCopy forLocal:YES];
        [self setDraggingSourceOperationMask:NSDragOperationCopy forLocal:NO];
        [self registerForDraggedTypes:[NSArray arrayWithObject:NSStringPboardType]];

        return self;
    } else {
        return nil;
    }
}

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)localDestination {
    return NSDragOperationEvery;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView {
    return 512;
}

- (id)tableView:(NSTableView*)tableView objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
    if ([[tableColumn identifier]  isEqual: @"Channel"]) {
        return [[NSString alloc] initWithFormat:@"%ld", row + 1 ];
    } else if ([[tableColumn identifier]  isEqual: @"VDJscript"]) {
        return @(config->channelCommands[row].c_str());
    } else {
        return @"";
    }
}

- (void)tableView:(NSTableView*)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
    [[[[tableView window] undoManager] prepareWithInvocationTarget:self] tableView:tableView setObjectValue:@(config->channelCommands[row].c_str()) forTableColumn:tableColumn row:row];
    config->channelCommands[row] = std::string([object UTF8String]);
    config->saveConfig();
    [tableView reloadData];
}

- (BOOL)tableView:(NSTableView*)tableView writeRowsWithIndexes:(NSIndexSet*)rowIndexes toPasteboard:(NSPasteboard*)pboard {
    [pboard declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [pboard setString:[self tableView:tableView objectValueForTableColumn:[tableView tableColumns][1] row:(NSInteger)[rowIndexes firstIndex]] forType:NSStringPboardType];
    return YES;
}

- (NSDragOperation)tableView:(NSTableView*)tableView validateDrop:(id<NSDraggingInfo>)info proposedRow:(NSInteger)row proposedDropOperation:(NSTableViewDropOperation)dropOperation {
    return NSDragOperationCopy;
}

- (BOOL)tableView:(NSTableView*)tableView acceptDrop:(id<NSDraggingInfo>)info row:(NSInteger)row dropOperation:(NSTableViewDropOperation)dropOperation {
    [self tableView:tableView setObjectValue:[[info draggingPasteboard] stringForType:NSStringPboardType] forTableColumn:[tableView tableColumns][1] row:row];
    return YES;
}

@end
