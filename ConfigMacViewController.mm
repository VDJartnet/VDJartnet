//
//  ConfigMacViewController.m
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

#import "ConfigMacViewController.h"

@implementation ConfigViewController {
    NSScrollView* scrollView;
}

- (void)loadView {
    self.view = [[NSView alloc] init];

    _tableView = [[ConfigTableView alloc] init];
    [_tableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"Channel"]];
    [_tableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"VDJscript"]];
    [[_tableView tableColumns][0] setTitle:NSLocalizedString(@"Channel", @"DMX Channel")];
    [[_tableView tableColumns][0] setEditable:NO];
    [[_tableView tableColumns][1] setTitle:NSLocalizedString(@"VDJscript", @"")];
    [_tableView setDataSource:self];
    [_tableView setDelegate:self];
    [_tableView reloadData];
    [_tableView setDraggingSourceOperationMask:NSDragOperationCopy forLocal:YES];
    [_tableView setDraggingSourceOperationMask:NSDragOperationCopy forLocal:NO];
    [_tableView registerForDraggedTypes:[NSArray arrayWithObject:NSStringPboardType]];

    scrollView = [[NSScrollView alloc] initWithFrame:CGRectMake(0,0,100,100)];
    [scrollView setDocumentView:_tableView];
    [scrollView setHasVerticalScroller:YES];
    [[self view] addSubview:scrollView];

    _ipAddress = [[NSTextField alloc] initWithFrame:CGRectMake(50,100,100,30)];
    _ipAddress.stringValue = [NSString stringWithCString:_vdjArtnet->host.c_str() encoding:[NSString defaultCStringEncoding]];
    [_ipAddress setEditable:YES];
    [_ipAddress setDelegate:self];
    [[self view] addSubview:_ipAddress];

    _ipLabel = [[NSTextField alloc] initWithFrame:CGRectMake(0,100,100,30)];
    _ipLabel.stringValue = NSLocalizedString(@"IP address:", @"");
    [_ipLabel setEditable:NO];
    [_ipLabel setSelectable:NO];
    [_ipLabel setBordered:NO];
    [[self view] addSubview:_ipLabel];
}

- (void)viewDidLayout {
    [super viewDidLayout];

    CGPoint origin = [[[self view] window] contentRectForFrameRect:[[self view] frame]].origin;
    CGSize size = [[[self view] window] contentRectForFrameRect:[[[self view] window] frame]].size;
    [_ipLabel setFrame:CGRectMake(origin.x, origin.y + size.height - _ipAddress.frame.size.height - 30, _ipLabel.frame.size.width, _ipAddress.frame.size.height)];
    [_ipAddress setFrame:CGRectMake(origin.x + _ipLabel.frame.size.width, origin.y + size.height - _ipAddress.frame.size.height - 30, size.width - _ipLabel.frame.size.width, _ipAddress.frame.size.height)];
    [scrollView setFrame:CGRectMake(origin.x, origin.y, size.width, _ipAddress.frame.origin.y - origin.y)];
    [[_tableView tableColumns][0] setWidth:50];
    [[_tableView tableColumns][1] setWidth:size.width - [[_tableView tableColumns][0] width] - 10];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    return 512;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    if ([[tableColumn identifier]  isEqual: @"Channel"]) {
        return [[NSString alloc] initWithFormat:@"%ld", row + 1 ];
    } else if ([[tableColumn identifier]  isEqual: @"VDJscript"]) {
        return [NSString stringWithCString:_vdjArtnet->channelCommands[row].c_str() encoding:[NSString defaultCStringEncoding]];
    } else {
        return @"";
    }
}

- (void)tableView:(NSTableView *)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    [[[[tableView window] undoManager] prepareWithInvocationTarget:self] tableView:tableView setObjectValue:[NSString stringWithCString:_vdjArtnet->channelCommands[row].c_str() encoding:[NSString defaultCStringEncoding]] forTableColumn:tableColumn row:row];
    _vdjArtnet->channelCommands[row] = [object cStringUsingEncoding:[NSString defaultCStringEncoding]];
    _vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
    [tableView reloadData];
}

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor {
    _vdjArtnet->host = std::string([_ipAddress.stringValue cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    _vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);

    return YES;
}

- (BOOL)tableView:(NSTableView *)tableView writeRowsWithIndexes:(NSIndexSet *)rowIndexes toPasteboard:(NSPasteboard *)pboard {
    [pboard declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [pboard setString:[self tableView:tableView objectValueForTableColumn:[tableView tableColumns][1] row:(NSInteger)[rowIndexes firstIndex]] forType:NSStringPboardType];

    return YES;
}

- (NSDragOperation)tableView:(NSTableView *)tableView validateDrop:(id<NSDraggingInfo>)info proposedRow:(NSInteger)row proposedDropOperation:(NSTableViewDropOperation)dropOperation {
    return NSDragOperationCopy;
}

- (BOOL)tableView:(NSTableView *)tableView acceptDrop:(id<NSDraggingInfo>)info row:(NSInteger)row dropOperation:(NSTableViewDropOperation)dropOperation {
    [self tableView:tableView setObjectValue:[[info draggingPasteboard] stringForType:NSStringPboardType] forTableColumn:[tableView tableColumns][1] row:row];
    return YES;
}

- (void)windowWillClose:(NSNotification *)notification {
    CFRelease(_vdjArtnet->configWindow);
    _vdjArtnet->configWindow = nullptr;
}

@end
