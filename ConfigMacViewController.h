//
//  ConfigMacViewController.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigMacViewController_h
#define ConfigMacViewController_h

#include <AppKit/AppKit.h>
#import "ConfigMacTableView.h"
#ifdef __cplusplus
#include "VDJartnet.hpp"
#endif

@interface ConfigViewController : NSViewController <NSTableViewDataSource, NSTableViewDelegate, NSTextFieldDelegate, NSWindowDelegate>

@property CVDJartnet* vdjArtnet;
@property (strong) ConfigTableView* tableView;
@property (strong) NSTextField* ipAddress;
@property (strong) NSTextField* ipLabel;

- (void)loadView;
- (void)viewDidLayout;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;
- (void)tableView:(NSTableView *)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor;

- (BOOL)tableView:(NSTableView *)tableView writeRowsWithIndexes:(NSIndexSet *)rowIndexes toPasteboard:(NSPasteboard *)pboard;
- (NSDragOperation)tableView:(NSTableView *)tableView validateDrop:(id<NSDraggingInfo>)info proposedRow:(NSInteger)row proposedDropOperation:(NSTableViewDropOperation)dropOperation;
- (BOOL)tableView:(NSTableView *)tableView acceptDrop:(id<NSDraggingInfo>)info row:(NSInteger)row dropOperation:(NSTableViewDropOperation)dropOperation;

- (void)windowWillClose:(NSNotification *)notification;
@end

#endif /* ConfigMacViewController_h */
